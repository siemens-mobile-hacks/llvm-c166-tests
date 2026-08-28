#!/usr/bin/env bash

c166_find_tasking_root() {
  local install_prefix="${TASKING_C166_INSTALL_PREFIX:-${TASKING_C166_WINEPREFIX:-$(c166_default_wine_prefix)}}"
  local candidate

  if [[ -n "${TASKING_C166_ROOT:-}" ]]; then
    printf '%s\n' "${TASKING_C166_ROOT}"
    return
  fi

  for candidate in \
    "${install_prefix}/drive_c/Program Files (x86)/TASKING/dc166 v8.6" \
    "${install_prefix}/drive_c/Program Files/TASKING/dc166 v8.6"; do
    if [[ -f "${candidate}/bin/cc166.exe" && -r "${candidate}/bin/cc166.exe" ]]; then
      printf '%s\n' "$candidate"
      return
    fi
  done

  c166_die "TASKING C166 v8.6 not found; set TASKING_C166_ROOT or TASKING_C166_INSTALL_PREFIX"
}

c166_toolchain_configure() {
  local llvm_build="$1"
  local tasking_root="$2"
  local -n tools_ref="$3"

  tools_ref=(
    [clang]="${llvm_build}/bin/clang"
    [lld]="${llvm_build}/bin/ld.lld"
    [llc]="${llvm_build}/bin/llc"
    [objcopy]="${llvm_build}/bin/llvm-objcopy"
    [objdump]="${llvm_build}/bin/llvm-objdump"
    [nm]="${llvm_build}/bin/llvm-nm"
    [readobj]="${llvm_build}/bin/llvm-readobj"
    [a166]="${tasking_root}/bin/a166.exe"
    [m166]="${tasking_root}/bin/m166.exe"
    [cc166]="${tasking_root}/bin/cc166.exe"
    [ihex166]="${tasking_root}/bin/ihex166.exe"
    [xfw166]="${tasking_root}/bin/xfw166.exe"
    [ihex_overlay]="${project_root}/tools/ihex-overlay"
    [result_checker]="${project_root}/tools/check-result-log"
    [symbol_checker]="${project_root}/tools/check-required-symbols"
    [tasking_map_symbols]="${project_root}/tools/tasking-map-symbols"
  )
}

c166_toolchain_verify() {
  local -n tools_ref="$1"
  local require_import_mapper="${2:-false}"
  local name

  for name in clang lld llc objcopy objdump nm readobj ihex_overlay \
              result_checker symbol_checker; do
    c166_require_executable "${tools_ref[$name]}"
  done
  c166_require_executable "$(command -v timeout || true)"

  for name in a166 m166 cc166 ihex166 xfw166; do
    c166_require_wine_program "${tools_ref[$name]}"
  done

  if [[ "$require_import_mapper" == true ]]; then
    c166_require_executable "${tools_ref[tasking_map_symbols]}"
  fi
}

c166_tasking_runtime_configure() {
  local tasking_root="$1"
  local model="$2"
  local wine_prefix="$3"
  local -n model_ref="$4"
  local -n case_ref="$5"
  local -n libraries_ref="$6"
  local -n link_flags_ref="$7"
  local input
  local library_dir
  local simulator_config
  local startup_source
  local tasking_include

  case "${case_ref[runtime_variant]}" in
    ext)
      library_dir=ext
      simulator_config=sim167.cfg
      startup_source=cstartx.asm
      case_ref[tasking_arch_flag]=""
      case_ref[tasking_asm_arch]=EXTEND
      ;;
    ext2)
      library_dir=ext2
      simulator_config=simsuper10bo.cfg
      startup_source=cstartx2.asm
      case_ref[tasking_arch_flag]=-x2
      case_ref[tasking_asm_arch]=EXTEND2
      ;;
    *)
      c166_die "invalid TASKING runtime variant: ${case_ref[runtime_variant]}"
      ;;
  esac
  case_ref[simulator_config]="$simulator_config"

  libraries_ref=()
  case "${case_ref[runtime_policy]}" in
    system)
      link_flags_ref=()
      libraries_ref=(
        "${tasking_root}/lib/${library_dir}/c166${model_ref[tasking_library]}.lib"
        "${tasking_root}/lib/${library_dir}/fp166${model_ref[tasking_library]}.lib"
        "${tasking_root}/lib/${library_dir}/rt166${model_ref[tasking_library]}.lib"
      )
      for input in "${libraries_ref[@]}"; do
        [[ -f "$input" ]] ||
          c166_die "missing TASKING ${model} system library: ${input}"
      done
      case_ref[startup_policy]=system
      case_ref[tasking_cstart]="${tasking_root}/lib/src/${startup_source}"
      tasking_include="${tasking_root}/include"
      [[ -f "${case_ref[tasking_cstart]}" ]] ||
        c166_die "missing TASKING startup: ${case_ref[tasking_cstart]}"
      [[ -d "$tasking_include" ]] ||
        c166_die "missing TASKING include directory: ${tasking_include}"
      case_ref[tasking_include_windows]="$(
        WINEPREFIX="$wine_prefix" winepath -w "$tasking_include"
      )"
      ;;
    none)
      link_flags_ref=(-nolib)
      case_ref[startup_policy]=minimal
      case_ref[tasking_cstart]=""
      case_ref[tasking_include_windows]=""
      ;;
    *)
      c166_die "invalid tasking_runtime policy: ${case_ref[runtime_policy]}"
      ;;
  esac
}

c166_record_tasking_runtime() {
  local run_dir="$1"
  local tasking_root="$2"
  local model="$3"
  local -n case_ref="$4"
  local -n libraries_ref="$5"

  if [[ "${case_ref[runtime_policy]}" == system ]]; then
    {
      printf 'policy=system\nvariant=%s\nmodel=%s\ntasking_root=%s\n' \
        "${case_ref[runtime_variant]}" "$model" "$tasking_root"
      sha256sum "${libraries_ref[@]}"
    } >"${run_dir}/tasking-libraries.txt"
    {
      printf 'policy=system\nvariant=%s\nmodel=%s\nsource=%s\n' \
        "${case_ref[runtime_variant]}" "$model" "${case_ref[tasking_cstart]}"
      sha256sum "${case_ref[tasking_cstart]}"
    } >"${run_dir}/tasking-startup.txt"
  fi
}

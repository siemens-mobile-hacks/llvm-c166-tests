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

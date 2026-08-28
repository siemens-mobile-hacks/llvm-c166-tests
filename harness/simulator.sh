#!/usr/bin/env bash

source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/common.sh"
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/compiler.sh"
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/imports.sh"
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/inputs.sh"
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/manifest.sh"
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/overlay.sh"
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/result.sh"
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/linker/model.sh"
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/toolchain.sh"
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/simulator/session.sh"
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/state.sh"
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/linker/image.sh"

c166_simulator_run_case() {
  local case_dir="$1"
  local case_name="$2"
  local optimization="${3:-O2}"
  local model="${4:-large}"
  local llvm_build="${LLVM_BUILD:-/tmp/codex/llvm-c166-mc-build}"
  local wine_prefix="${TASKING_C166_WINEPREFIX:-$(c166_default_wine_prefix)}"
  local manifest="${case_dir}/case.json"
  local source_case=""
  local source_dir="$case_dir"
  local category=""
  local llvm_entry="_llvm_add"
  local llvm_entry_source="${project_root}/harness/linker/llvm-entry.s"
  local tasking_model_flag="-Ml"
  local tasking_asm_model="LARGE"
  local tasking_model_is_medium=0
  local tasking_model_is_small=0
  local tasking_host="host.c"
  local result_protocol="scalar"
  local expected_result=""
  local expected_suite_id=""
  local expected_signature=""
  local failure_reducer=""
  local failure_reducer_path=""
  local runtime_builtins=false
  local tasking_runtime_policy="system"
  local tasking_startup_policy="system"
  local tasking_library_model="l"
  local simulator_timeout=30
  local tasking_root
  local run_dir
  local log
  local input
  local -a extra_clang_flags=()
  local -a common_c_defines=()
  local -a extra_ldflags=()
  local -a llvm_link_inputs=()
  local -a llvm_model_ldflags=()
  local -a llvm_mir_sources=()
  local -a llvm_sources=(llvm.c)
  local -a llvm_overlay_args=()
  local use_dpp_overlay=false
  local -a required_symbols=()
  local -a simulator_launcher=()
  local -a tasking_asm_sources=()
  local -a tasking_nodebug_sources=()
  local -a tasking_import_symbols=()
  local -a tasking_sources=()
  local -a case_inputs=()
  local -a tasking_model_flags=()
  local -a tasking_link_flags=(-nolib)
  local -a tasking_system_libraries=()
  local -A model_config=()
  local -A toolchain=()

  [[ "$optimization" =~ ^O(0|1|2|3|s|z)$ ]] ||
    c166_die "invalid optimization level: ${optimization}"
  [[ "$model" == large || "$model" == medium || "$model" == small ]] ||
    c166_die "invalid C166 memory model: ${model}"

  c166_manifest_validate_configuration "$manifest" "$optimization" "$model"

  mapfile -t llvm_sources < <(jq -er '.llvm_sources[]' "$manifest")
  mapfile -t llvm_mir_sources < <(jq -er '.llvm_mir_sources[]?' "$manifest")
  mapfile -t tasking_sources < <(jq -er '.tasking_sources[]' "$manifest")
  mapfile -t tasking_nodebug_sources < <(
    jq -er '.tasking_nodebug_sources[]?' "$manifest"
  )
  mapfile -t tasking_import_symbols < <(
    jq -er '.tasking_import_symbols[]?' "$manifest"
  )
  mapfile -t tasking_asm_sources < <(
    jq -er '.tasking_asm_sources[]?' "$manifest"
  )
  mapfile -t case_inputs < <(jq -er '.inputs[]?' "$manifest")
  mapfile -t required_symbols < <(
    jq -er --arg model "$model" '
      (if $model == "small" then
         (.required_symbols_by_model.small //
          .required_symbols_by_model.large // .required_symbols)
       else
         (.required_symbols_by_model[$model] // .required_symbols)
       end)[]' "$manifest"
  )
  mapfile -t extra_clang_flags < <(jq -er '.extra_clang_flags[]?' "$manifest")
  mapfile -t common_c_defines < <(jq -er '.defines[]?' "$manifest")
  source_case="$(jq -r '.source_case // ""' "$manifest")"
  if [[ -n "$source_case" ]]; then
    source_dir="$("${project_root}/tools/find-case" "$source_case")"
    [[ -d "$source_dir" ]] ||
      c166_die "missing source_case directory: ${source_dir}"
  fi
  mapfile -t extra_ldflags < <(
    jq -er --arg model "$model" '
      (if $model == "small" then
         (.extra_ldflags_by_model.small //
          .extra_ldflags_by_model.large // .extra_ldflags // [])
       else
         (.extra_ldflags_by_model[$model] // .extra_ldflags // [])
       end)[]' "$manifest"
  )
  # The first link needs placeholders until the located oracle map exists.
  c166_add_import_placeholders tasking_import_symbols extra_ldflags
  failure_reducer="$(jq -r '.failure_reducer // ""' "$manifest")"
  category="$(jq -er '.category' "$manifest")"
  tasking_host="$(jq -er '.tasking_host' "$manifest")"
  llvm_entry="$(jq -er '.llvm_entry' "$manifest")"
  # jq -e intentionally exits non-zero for the valid JSON boolean false.
  runtime_builtins="$(jq -r '.runtime_builtins // false' "$manifest")"
  tasking_runtime_policy="$(jq -r '.tasking_runtime // "system"' "$manifest")"
  if [[ "$tasking_runtime_policy" == system ]]; then
    tasking_startup_policy="system"
  else
    tasking_startup_policy="minimal"
  fi
  result_protocol="$(jq -er '.result.protocol' "$manifest")"
  simulator_timeout="$(jq -er '.simulator_timeout // 30' "$manifest")"
  if [[ "$result_protocol" == block ]]; then
    expected_suite_id="$(jq -er '.result.suite_id' "$manifest")"
    expected_signature="$(jq -er --arg model "$model" \
      '.result.expected_signature_by_model[$model] //
       .result.expected_signature' "$manifest")"
  else
    expected_result="$(jq -er '.result.expected_value' "$manifest")"
  fi

  c166_model_configure "$model" model_config
  c166_model_link_flags "$model" llvm_model_ldflags
  c166_model_overlay_args "$model" llvm_overlay_args
  llvm_entry_source="${project_root}/harness/linker/${model_config[entry_source]}"
  tasking_model_flag="${model_config[tasking_flag]}"
  tasking_asm_model="${model_config[tasking_asm]}"
  tasking_library_model="${model_config[tasking_library]}"
  tasking_model_is_medium="${model_config[is_medium]}"
  tasking_model_is_small="${model_config[is_small]}"
  use_dpp_overlay="${model_config[use_dpp_overlay]}"
  if [[ "$model" == medium ]]; then
    tasking_model_flags=(-Dllvm_entry_proxy=llvm_medium_entry_proxy)
  fi

  # Keep shared C fixtures model-aware in both compilers.  These are harness
  # feature-test macros, not target ABI spellings.
  common_c_defines+=(
    "TASKING_MODEL_IS_MEDIUM=${tasking_model_is_medium}"
    "TASKING_MODEL_IS_SMALL=${tasking_model_is_small}"
  )

  [[ "${model_config[text_start]}" =~ ^0x[0-9a-fA-F]+$ ]] ||
    c166_die "invalid LLVM text start: ${model_config[text_start]}"
  [[ "$simulator_timeout" =~ ^[1-9][0-9]*$ ]] ||
    c166_die "invalid simulator timeout: ${simulator_timeout}"
  [[ "$runtime_builtins" == true || "$runtime_builtins" == false ]] ||
    c166_die "invalid runtime_builtins value: ${runtime_builtins}"
  tasking_root="$(c166_find_tasking_root)"
  case "$tasking_runtime_policy" in
    system)
      tasking_link_flags=()
      tasking_system_libraries=(
        "${tasking_root}/lib/ext/c166${tasking_library_model}.lib"
        "${tasking_root}/lib/ext/fp166${tasking_library_model}.lib"
        "${tasking_root}/lib/ext/rt166${tasking_library_model}.lib"
      )
      for input in "${tasking_system_libraries[@]}"; do
        [[ -f "$input" ]] ||
          c166_die "missing TASKING ${model} system library: ${input}"
      done
      ;;
    none)
      tasking_link_flags=(-nolib)
      ;;
    *)
      c166_die "invalid tasking_runtime policy: ${tasking_runtime_policy}"
      ;;
  esac
  local tasking_cstart="${tasking_root}/lib/src/cstartx.asm"
  local tasking_include="${tasking_root}/include"
  local tasking_include_windows=""
  case "$tasking_startup_policy" in
    minimal)
      ;;
    system)
      [[ "$tasking_runtime_policy" == system ]] ||
        c166_die "system TASKING startup requires system runtime"
      [[ -f "$tasking_cstart" ]] ||
        c166_die "missing TASKING Classic startup: ${tasking_cstart}"
      [[ -d "$tasking_include" ]] ||
        c166_die "missing TASKING include directory: ${tasking_include}"
      tasking_include_windows="$(WINEPREFIX="$wine_prefix" winepath -w "$tasking_include")"
      ;;
    *)
      c166_die "invalid tasking_startup policy: ${tasking_startup_policy}"
      ;;
  esac
  if [[ -n "$failure_reducer" ]]; then
    failure_reducer_path="${project_root}/${failure_reducer}"
    c166_require_executable "$failure_reducer_path"
  fi

  run_dir="$(c166_new_simulator_run_dir "$case_name" "$optimization" "$model")"
  c166_state_initialize "$run_dir" "$case_dir" "$case_name" "$category" \
    "$model" "$optimization" "$source_case" "$tasking_runtime_policy"
  if [[ "$tasking_runtime_policy" == system ]]; then
    {
      printf 'policy=system\nmodel=%s\ntasking_root=%s\n' \
        "$model" "$tasking_root"
      sha256sum "${tasking_system_libraries[@]}"
    } >"${run_dir}/tasking-libraries.txt"
  fi
  if [[ "$tasking_startup_policy" == system ]]; then
    {
      printf 'policy=system\nmodel=%s\nsource=%s\n' \
        "$model" "$tasking_cstart"
      sha256sum "$tasking_cstart"
    } >"${run_dir}/tasking-startup.txt"
  fi

  c166_toolchain_configure "$llvm_build" "$tasking_root" toolchain
  local require_import_mapper=false
  ((${#tasking_import_symbols[@]})) && require_import_mapper=true
  c166_toolchain_verify toolchain "$require_import_mapper"
  c166_state_complete_stage "$run_dir" toolchains-verified
  local clang="${toolchain[clang]}"
  local lld="${toolchain[lld]}"
  local llc="${toolchain[llc]}"
  local objcopy="${toolchain[objcopy]}"
  local objdump="${toolchain[objdump]}"
  local nm="${toolchain[nm]}"
  local readobj="${toolchain[readobj]}"
  local ihex166="${toolchain[ihex166]}"
  local xfw166="${toolchain[xfw166]}"
  local result_checker="${toolchain[result_checker]}"
  local symbol_checker="${toolchain[symbol_checker]}"
  local tasking_map_symbols="${toolchain[tasking_map_symbols]}"
  c166_prepare_case_inputs "$source_dir" "$run_dir" "$tasking_host" \
    "$result_protocol" "$tasking_startup_policy" \
    llvm_sources llvm_mir_sources tasking_sources tasking_nodebug_sources \
    tasking_asm_sources case_inputs
  cp "$manifest" "${run_dir}/case.json"
  c166_prepare_simulator_session \
    "$case_dir" "$result_protocol" "$run_dir" "$tasking_root"
  c166_simulator_launcher simulator_launcher
  c166_state_complete_stage "$run_dir" inputs-prepared

  c166_build_llvm_objects "$clang" "$llc" "$model" "$optimization" \
    "$run_dir" "$llvm_entry_source" "$runtime_builtins" \
    llvm_sources llvm_mir_sources extra_clang_flags common_c_defines \
    llvm_link_inputs
  c166_link_llvm_image "$lld" "${run_dir}/llvm.elf" "$llvm_entry" \
    model_config llvm_model_ldflags extra_ldflags llvm_link_inputs
  c166_emit_llvm_evidence \
    "$objcopy" "$objdump" "$nm" "${run_dir}/llvm.elf" "$run_dir"
  c166_configure_overlay_from_elf "$objdump" "${run_dir}/llvm.elf" \
    "$model" llvm_overlay_args use_dpp_overlay
  if ((${#tasking_import_symbols[@]} == 0)); then
    c166_verify_llvm_image "$run_dir" "$case_dir" "$model" "$manifest" \
      model_config required_symbols "$symbol_checker"
    c166_state_complete_stage "$run_dir" llvm-linked
  fi

  c166_build_tasking_oracle "$run_dir" "$wine_prefix" \
    "$tasking_model_flag" "$tasking_asm_model" "$tasking_startup_policy" \
    "$tasking_cstart" "$tasking_include_windows" "$use_dpp_overlay" \
    "$tasking_host" toolchain tasking_model_flags common_c_defines \
    tasking_link_flags tasking_sources tasking_nodebug_sources \
    tasking_asm_sources "$tasking_model_is_medium" "$tasking_model_is_small"
  c166_state_complete_stage "$run_dir" tasking-linked

  (
    cd "$run_dir"
    if ((${#tasking_import_symbols[@]})); then
      c166_resolve_tasking_imports "$tasking_map_symbols" host.map \
        tasking-imports.ldflags tasking_import_symbols extra_ldflags
      c166_link_llvm_image "$lld" "${run_dir}/llvm.elf" "$llvm_entry" \
        model_config llvm_model_ldflags extra_ldflags llvm_link_inputs
      c166_emit_llvm_evidence \
        "$objcopy" "$objdump" "$nm" "${run_dir}/llvm.elf" "$run_dir"
      c166_verify_llvm_image "$run_dir" "$case_dir" "$model" "$manifest" \
        model_config required_symbols "$symbol_checker"
      c166_state_complete_stage "$run_dir" llvm-linked
    fi

    # Intel HEX cannot encode allocated SHT_NOBITS ranges; own and zero them.
    c166_add_nobits_overlay_ranges "$readobj" llvm.elf llvm_overlay_args
    c166_wine_cli "$wine_prefix" "$ihex166" \
      -i32 -O host.out -o tasking.hex
    "${project_root}/tools/ihex-overlay" \
      "${llvm_overlay_args[@]}" tasking.hex llvm.hex -o image.hex
    c166_state_complete_stage "$run_dir" image-overlaid
    c166_run_crossview simulator_launcher "$wine_prefix" "$xfw166" \
      "$simulator_timeout" "$run_dir"
    c166_state_complete_stage "$run_dir" simulator-complete
  )

  log="${run_dir}/session.log"
  c166_validate_simulator_result "$result_checker" "$log" \
    "$result_protocol" "$expected_result" "$expected_suite_id" \
    "$expected_signature" "$case_dir" "$optimization" \
    "$failure_reducer_path"
  c166_state_complete_stage "$run_dir" result-validated

  if [[ "$result_protocol" == block ]]; then
    echo "test_result=suite:${expected_suite_id},signature:${expected_signature}"
  else
    echo "simulator_result=${expected_result}"
  fi
  echo "optimization=${optimization}"
  echo "model=${model}"
  echo "artifacts=${run_dir}"
}

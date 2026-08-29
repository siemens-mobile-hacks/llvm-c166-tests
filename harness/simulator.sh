#!/usr/bin/env bash

harness_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "${harness_dir}/common.sh"
source "${harness_dir}/compiler.sh"
source "${harness_dir}/imports.sh"
source "${harness_dir}/inputs.sh"
source "${harness_dir}/manifest.sh"
source "${harness_dir}/overlay.sh"
source "${harness_dir}/result.sh"
source "${harness_dir}/linker/model.sh"
source "${harness_dir}/toolchain.sh"
source "${harness_dir}/simulator/session.sh"
source "${harness_dir}/linker/image.sh"
unset harness_dir

c166_simulator_run_case() {
  local case_dir="$1"
  local case_name="$2"
  local optimization="${3:-O2}"
  local model="${4:-large}"
  local runtime_variant="${5:-ext}"
  local llvm_build="${LLVM_BUILD:?set LLVM_BUILD to the C166 LLVM build directory}"
  local wine_prefix="$(c166_default_wine_prefix)"
  local manifest="${case_dir}/case.json"
  local source_dir="$case_dir"
  local tasking_root run_dir log failure_reducer_path
  local use_dpp_overlay=false
  local -A case_config=()
  local -A model_config=()
  local -A toolchain=()
  local -a llvm_sources=()
  local -a llvm_mir_sources=()
  local -a tasking_sources=()
  local -a tasking_nodebug_sources=()
  local -a tasking_import_symbols=()
  local -a tasking_required_symbols=()
  local -a tasking_asm_sources=()
  local -a case_inputs=()
  local -a required_symbols=()
  local -a extra_clang_flags=()
  local -a common_c_defines=()
  local -a extra_ldflags=()
  local -a llvm_link_inputs=()
  local -a llvm_model_ldflags=()
  local -a llvm_overlay_args=()
  local -a tasking_model_flags=()
  local -a tasking_link_flags=()
  local -a tasking_system_libraries=()
  local -a simulator_launcher=()

  [[ "$optimization" =~ ^O(0|1|2|3|s|z)$ ]] ||
    c166_die "invalid optimization level: ${optimization}"
  [[ "$model" == large || "$model" == medium || "$model" == small ]] ||
    c166_die "invalid C166 memory model: ${model}"
  [[ "$runtime_variant" == ext || "$runtime_variant" == ext2 ]] ||
    c166_die "invalid TASKING runtime variant: ${runtime_variant}"

  c166_manifest_validate_configuration "$manifest" "$optimization" "$model" \
    "$runtime_variant"
  c166_manifest_load "$manifest" "$model" "$runtime_variant" case_config \
    llvm_sources llvm_mir_sources tasking_sources tasking_nodebug_sources \
    tasking_import_symbols tasking_asm_sources case_inputs required_symbols \
    extra_clang_flags common_c_defines extra_ldflags \
    tasking_required_symbols

  if [[ -n "${case_config[source_case]}" ]]; then
    source_dir="$("${project_root}/tools/find-case" "${case_config[source_case]}")"
  fi
  if [[ -n "${case_config[failure_reducer]}" ]]; then
    failure_reducer_path="${project_root}/${case_config[failure_reducer]}"
    c166_require_executable "$failure_reducer_path"
  else
    failure_reducer_path=""
  fi

  c166_model_configure "$model" model_config
  c166_model_link_flags "$model" llvm_model_ldflags
  c166_model_overlay_args "$model" llvm_overlay_args
  use_dpp_overlay="${model_config[use_dpp_overlay]}"
  if [[ "$model" == medium ]]; then
    tasking_model_flags=(
      -Dllvm_entry_proxy=llvm_medium_entry_proxy
      -Dllvm_crt_init_proxy=llvm_medium_crt_init_proxy
    )
  fi
  common_c_defines+=(
    "TASKING_MODEL_IS_MEDIUM=${model_config[is_medium]}"
    "TASKING_MODEL_IS_SMALL=${model_config[is_small]}"
  )
  if [[ "${case_config[driver]}" == differential ]]; then
    common_c_defines+=(
      "C166_TEST_CASE_COUNT=${case_config[case_count]}"
      "C166_TEST_SEED=${case_config[test_seed]}UL"
      "C166_TEST_REFERENCE_SIGNATURE=${case_config[reference_signature]}UL"
      "C166_TEST_SUITE_ID=${case_config[expected_suite_id]}"
    )
  fi
  c166_add_import_placeholders tasking_import_symbols extra_ldflags

  [[ "${model_config[text_start]}" =~ ^0x[0-9a-fA-F]+$ ]] ||
    c166_die "invalid LLVM text start: ${model_config[text_start]}"
  [[ "${case_config[simulator_timeout]}" =~ ^[1-9][0-9]*$ ]] ||
    c166_die "invalid simulator timeout: ${case_config[simulator_timeout]}"
  [[ "${case_config[runtime_builtins]}" == true ||
     "${case_config[runtime_builtins]}" == false ]] ||
    c166_die "invalid runtime_builtins value: ${case_config[runtime_builtins]}"

  tasking_root="$(c166_tasking_root)"
  c166_tasking_configure "$tasking_root" "$model" "$wine_prefix" \
    model_config case_config tasking_system_libraries tasking_link_flags

  run_dir="$(c166_new_simulator_run_dir \
    "$case_name" "$optimization" "$model" "$runtime_variant")"
  c166_record_tasking_runtime "$run_dir" "$tasking_root" "$model" \
    case_config tasking_system_libraries

  c166_toolchain_configure "$llvm_build" "$tasking_root" toolchain
  local require_import_mapper=false
  ((${#tasking_import_symbols[@]} || ${#tasking_required_symbols[@]})) &&
    require_import_mapper=true
  c166_toolchain_verify toolchain "$require_import_mapper"
  c166_prepare_case_inputs "$source_dir" "$run_dir" \
    "${case_config[tasking_host]}" "${case_config[result_protocol]}" \
    "${case_config[runtime_variant]}" \
    llvm_sources llvm_mir_sources \
    tasking_sources tasking_nodebug_sources tasking_asm_sources case_inputs
  cp "$manifest" "${run_dir}/case.json"
  c166_prepare_simulator_session "$case_dir" \
    "${case_config[result_protocol]}" "$run_dir" "$tasking_root" \
    "${case_config[simulator_config]}"
  c166_simulator_launcher simulator_launcher
  c166_build_llvm_objects "${toolchain[clang]}" "${toolchain[llc]}" \
    "$model" "$optimization" "$run_dir" \
    "${project_root}/harness/linker/${model_config[entry_source]}" \
    "${case_config[runtime_builtins]}" llvm_sources llvm_mir_sources \
    extra_clang_flags common_c_defines llvm_link_inputs
  c166_link_llvm_image "${toolchain[lld]}" "${run_dir}/llvm.elf" \
    "${case_config[llvm_entry]}" model_config llvm_model_ldflags \
    extra_ldflags llvm_link_inputs
  c166_emit_llvm_evidence "${toolchain[objcopy]}" "${toolchain[objdump]}" \
    "${toolchain[nm]}" "${run_dir}/llvm.elf" "$run_dir"
  c166_configure_overlay_from_elf "${toolchain[objdump]}" \
    "${run_dir}/llvm.elf" "$model" llvm_overlay_args use_dpp_overlay
  model_config[use_dpp_overlay]="$use_dpp_overlay"
  if ((${#tasking_import_symbols[@]} == 0)); then
    c166_verify_llvm_image "$run_dir" "$case_dir" "$model" "$manifest" \
      model_config required_symbols "${toolchain[symbol_checker]}"
  fi

  c166_build_tasking_oracle "$run_dir" "$wine_prefix" case_config \
    model_config toolchain tasking_model_flags common_c_defines \
    tasking_link_flags tasking_sources tasking_nodebug_sources \
    tasking_asm_sources
  c166_verify_tasking_symbols "$run_dir" \
    "${toolchain[tasking_map_symbols]}" tasking_sources \
    tasking_nodebug_sources tasking_required_symbols
  (
    cd "$run_dir"
    if ((${#tasking_import_symbols[@]})); then
      c166_resolve_tasking_imports "${toolchain[tasking_map_symbols]}" \
        host.map tasking-imports.ldflags tasking_import_symbols extra_ldflags
      c166_link_llvm_image "${toolchain[lld]}" "${run_dir}/llvm.elf" \
        "${case_config[llvm_entry]}" model_config llvm_model_ldflags \
        extra_ldflags llvm_link_inputs
      c166_emit_llvm_evidence "${toolchain[objcopy]}" \
        "${toolchain[objdump]}" "${toolchain[nm]}" \
        "${run_dir}/llvm.elf" "$run_dir"
      c166_verify_llvm_image "$run_dir" "$case_dir" "$model" "$manifest" \
        model_config required_symbols "${toolchain[symbol_checker]}"
    fi

    c166_add_nobits_overlay_ranges "${toolchain[readobj]}" llvm.elf \
      llvm_overlay_args
    c166_wine_cli "$wine_prefix" "${toolchain[ihex166]}" \
      -i32 -O host.out -o tasking.hex
    "${toolchain[ihex_overlay]}" "${llvm_overlay_args[@]}" \
      tasking.hex llvm.hex -o image.hex
    c166_run_crossview simulator_launcher "$wine_prefix" \
      "${toolchain[xfw166]}" "${case_config[simulator_timeout]}" "$run_dir"
  )

  log="${run_dir}/session.log"
  c166_validate_simulator_result "${toolchain[result_checker]}" "$log" \
    "${case_config[result_protocol]}" "${case_config[expected_result]}" \
    "${case_config[expected_suite_id]}" "${case_config[expected_signature]}" \
    "$case_dir" "$optimization" "$failure_reducer_path"
  if [[ "${case_config[result_protocol]}" == block ]]; then
    echo "test_result=suite:${case_config[expected_suite_id]},signature:${case_config[expected_signature]}"
  else
    echo "simulator_result=${case_config[expected_result]}"
  fi
  echo "optimization=${optimization}"
  echo "model=${model}"
  echo "tasking_variant=${runtime_variant}"
  echo "artifacts=${run_dir}"
}

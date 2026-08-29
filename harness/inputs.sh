#!/usr/bin/env bash

c166_prepare_case_inputs() {
  local source_dir="$1"
  local run_dir="$2"
  local tasking_host="$3"
  local result_protocol="$4"
  local runtime_variant="$5"
  local -n llvm_sources_ref="$6"
  local -n mir_sources_ref="$7"
  local -n tasking_sources_ref="$8"
  local -n nodebug_sources_ref="$9"
  local -n asm_sources_ref="${10}"
  local -n case_inputs_ref="${11}"
  local input

  for input in "${llvm_sources_ref[@]}" "${mir_sources_ref[@]}" \
               "${tasking_sources_ref[@]}" \
               "${nodebug_sources_ref[@]}" "${asm_sources_ref[@]}" \
               "${case_inputs_ref[@]}"; do
    [[ "$input" =~ ^[A-Za-z0-9_.-]+$ ]] ||
      c166_die "case source must be a plain filename: ${input}"
    [[ -f "${source_dir}/${input}" ]] ||
      c166_die "missing test input: ${source_dir}/${input}"
    cp "${source_dir}/${input}" "${run_dir}/${input}"
  done

  if [[ "$tasking_host" == c166-differential-driver.c ]]; then
    cp "${project_root}/harness/runtime/${tasking_host}" "$run_dir/"
  else
    [[ "$tasking_host" =~ ^[A-Za-z0-9_.-]+$ ]] ||
      c166_die "case host must be a plain filename: ${tasking_host}"
    [[ -f "${source_dir}/${tasking_host}" ]] ||
      c166_die "missing test host: ${source_dir}/${tasking_host}"
    cp "${source_dir}/${tasking_host}" "${run_dir}/${tasking_host}"
  fi

  cp "${project_root}/harness/include/c166-test-result.h" "$run_dir/"
  cp "${project_root}/harness/include/c166-test-runtime.h" "$run_dir/"
  cp "${project_root}/harness/include/c166-address-spaces.h" "$run_dir/"
  cp "${project_root}/harness/include/c166-test-compat.h" "$run_dir/"
  cp "${project_root}/harness/include/c166-asm-model.inc" "$run_dir/"
  cp "${project_root}/harness/include/c166-asm-architecture-${runtime_variant}.inc" \
    "${run_dir}/c166-asm-architecture.inc"
  if [[ "$result_protocol" == block ]]; then
    cp "${project_root}/harness/runtime/c166-test-harness.c" "$run_dir/"
    tasking_sources_ref+=(c166-test-harness.c)
  fi
  cp "${project_root}/harness/simulator/proxy.asm" "$run_dir/"
  cp "${project_root}/harness/simulator/layout.ilo" "$run_dir/"
  cp "${project_root}/harness/runtime/test-startup.asm" "$run_dir/"
  cp "${project_root}/harness/runtime/dpp-overlay.asm" "$run_dir/"
  cp "${project_root}/harness/runtime/test-runner.c" "$run_dir/"
}

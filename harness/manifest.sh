#!/usr/bin/env bash

c166_manifest_validate_configuration() {
  local manifest="$1"
  local optimization="$2"
  local model="$3"

  [[ -f "$manifest" ]] || c166_die "missing case manifest: ${manifest}"
  c166_require_executable "$(command -v jq || true)"
  "${project_root}/tools/validate-manifest" "$manifest" ||
    c166_die "invalid case manifest: ${manifest}"
  jq -e --arg optimization "$optimization" \
    '.optimizations | index($optimization) != null' "$manifest" >/dev/null ||
    c166_die "optimization ${optimization} is not enabled by ${manifest}"
  jq -e --arg model "$model" \
    '(.models // ["large"]) | index($model) != null' "$manifest" >/dev/null ||
    c166_die "model ${model} is not enabled by ${manifest}"
}

c166_manifest_load() {
  local manifest="$1"
  local model="$2"
  local -n config_ref="$3"
  local -n llvm_sources_ref="$4"
  local -n llvm_mir_sources_ref="$5"
  local -n tasking_sources_ref="$6"
  local -n tasking_nodebug_sources_ref="$7"
  local -n tasking_import_symbols_ref="$8"
  local -n tasking_asm_sources_ref="$9"
  local -n inputs_ref="${10}"
  local -n required_symbols_ref="${11}"
  local -n clang_flags_ref="${12}"
  local -n defines_ref="${13}"
  local -n ldflags_ref="${14}"

  mapfile -t llvm_sources_ref < <(jq -er '.llvm_sources[]' "$manifest")
  mapfile -t llvm_mir_sources_ref < <(jq -er '.llvm_mir_sources[]?' "$manifest")
  mapfile -t tasking_sources_ref < <(jq -er '.tasking_sources[]' "$manifest")
  mapfile -t tasking_nodebug_sources_ref < <(
    jq -er '.tasking_nodebug_sources[]?' "$manifest"
  )
  mapfile -t tasking_import_symbols_ref < <(
    jq -er '.tasking_import_symbols[]?' "$manifest"
  )
  mapfile -t tasking_asm_sources_ref < <(
    jq -er '.tasking_asm_sources[]?' "$manifest"
  )
  mapfile -t inputs_ref < <(jq -er '.inputs[]?' "$manifest")
  mapfile -t required_symbols_ref < <(
    jq -er --arg model "$model" '
      (if $model == "small" then
         (.required_symbols_by_model.small //
          .required_symbols_by_model.large // .required_symbols)
       else
         (.required_symbols_by_model[$model] // .required_symbols)
       end)[]' "$manifest"
  )
  mapfile -t clang_flags_ref < <(jq -er '.extra_clang_flags[]?' "$manifest")
  mapfile -t defines_ref < <(jq -er '.defines[]?' "$manifest")
  mapfile -t ldflags_ref < <(
    jq -er --arg model "$model" '
      (if $model == "small" then
         (.extra_ldflags_by_model.small //
          .extra_ldflags_by_model.large // .extra_ldflags // [])
       else
         (.extra_ldflags_by_model[$model] // .extra_ldflags // [])
       end)[]' "$manifest"
  )

  config_ref=()
  config_ref[source_case]="$(jq -r '.source_case // ""' "$manifest")"
  config_ref[category]="$(jq -er '.category' "$manifest")"
  config_ref[tasking_host]="$(jq -er '.tasking_host' "$manifest")"
  config_ref[llvm_entry]="$(jq -er '.llvm_entry' "$manifest")"
  config_ref[failure_reducer]="$(jq -r '.failure_reducer // ""' "$manifest")"
  config_ref[runtime_builtins]="$(jq -r '.runtime_builtins // false' "$manifest")"
  config_ref[runtime_policy]="$(jq -r '.tasking_runtime // "system"' "$manifest")"
  config_ref[result_protocol]="$(jq -er '.result.protocol' "$manifest")"
  config_ref[simulator_timeout]="$(jq -er '.simulator_timeout // 30' "$manifest")"
  config_ref[expected_result]=""
  config_ref[expected_suite_id]=""
  config_ref[expected_signature]=""
  if [[ "${config_ref[result_protocol]}" == block ]]; then
    config_ref[expected_suite_id]="$(jq -er '.result.suite_id' "$manifest")"
    config_ref[expected_signature]="$(jq -er --arg model "$model" \
      '.result.expected_signature_by_model[$model] //
       .result.expected_signature' "$manifest")"
  else
    config_ref[expected_result]="$(jq -er '.result.expected_value' "$manifest")"
  fi
}

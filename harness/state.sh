#!/usr/bin/env bash

c166_state_initialize() {
  local run_dir="$1"
  local case_dir="$2"
  local case_name="$3"
  local category="$4"
  local model="$5"
  local optimization="$6"
  local source_case="$7"
  local runtime_policy="$8"

  jq -n \
    --arg case_dir "$case_dir" \
    --arg case_name "$case_name" \
    --arg category "$category" \
    --arg model "$model" \
    --arg optimization "$optimization" \
    --arg source_case "$source_case" \
    --arg runtime_policy "$runtime_policy" \
    '{
      schema_version: 1,
      case: $case_name,
      category: $category,
      model: $model,
      optimization: $optimization,
      source_case: (if $source_case == "" then $case_name else $source_case end),
      source_directory: $case_dir,
      tasking_runtime: $runtime_policy,
      stages: ["initialized"]
    }' >"${run_dir}/run-state.json"
}

c166_state_complete_stage() {
  local run_dir="$1"
  local stage="$2"
  local state="${run_dir}/run-state.json"
  local updated="${run_dir}/run-state.json.new"

  [[ "$stage" =~ ^[a-z][a-z0-9-]*$ ]] ||
    c166_die "invalid run stage name: ${stage}"
  [[ -f "$state" ]] || c166_die "missing run state: ${state}"
  jq -e --arg stage "$stage" \
    'if (.stages | index($stage)) == null
     then .stages += [$stage]
     else error("duplicate stage: " + $stage)
     end' "$state" >"$updated" || {
    rm -f "$updated"
    c166_die "cannot record run stage: ${stage}"
  }
  mv "$updated" "$state"
}

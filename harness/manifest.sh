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

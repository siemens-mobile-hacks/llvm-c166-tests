#!/usr/bin/env bash

c166_validate_simulator_result() {
  local result_checker="$1"
  local log="$2"
  local result_protocol="$3"
  local expected_result="$4"
  local expected_suite_id="$5"
  local expected_signature="$6"
  local case_dir="$7"
  local optimization="$8"
  local failure_reducer_path="$9"
  local pattern

  [[ -f "$log" ]] || c166_die "CrossView did not produce ${log}"
  if [[ "$result_protocol" == block ]]; then
    if ! "$result_checker" block "$log" "$expected_suite_id" \
        "$expected_signature"; then
      if [[ -n "$failure_reducer_path" ]]; then
        local reproducer_dir="${log%/*}/failure-reproducer"
        if "$failure_reducer_path" "$log" "$reproducer_dir" \
            "$case_dir" "$optimization"; then
          echo "failure_reproducer=${reproducer_dir}" >&2
        else
          echo "warning: failure reducer could not produce an artifact" >&2
        fi
      fi
      c166_die "invalid block result; log: ${log}"
    fi
  else
    "$result_checker" scalar "$log" "$expected_result" ||
      c166_die "invalid scalar result; log: ${log}"
  fi

  if [[ -f "${case_dir}/expected.log" ]]; then
    while IFS= read -r pattern; do
      pattern="${pattern%$'\r'}"
      [[ -n "$pattern" && "$pattern" != \#* ]] || continue
      rg -q -- "$pattern" "$log" ||
        c166_die "missing simulator log pattern '${pattern}'; log: ${log}"
    done <"${case_dir}/expected.log"
  fi
}

#!/usr/bin/env bash

c166_prepare_simulator_session() {
  local case_dir="$1"
  local result_protocol="$2"
  local run_dir="$3"
  local tasking_root="$4"
  local session_source="${case_dir}/session.cmd"
  local startup_count

  if [[ ! -f "$session_source" ]]; then
    if [[ "$result_protocol" == block ]]; then
      session_source="${project_root}/harness/simulator/session-block.cmd"
    else
      session_source="${project_root}/harness/simulator/session.cmd"
    fi
  fi
  [[ -f "$session_source" ]] || c166_die "missing simulator session fixture"
  cp "$session_source" "${run_dir}/session.cmd"

  startup_count="$(rg -c '^_CSTART[[:space:]]+gi$' \
    "${run_dir}/session.cmd" || true)"
  [[ "$startup_count" == 1 ]] ||
    c166_die "session must start _CSTART exactly once: ${session_source}"

  sed -i -E 's/^_CSTART([[:space:]]+)gi$/__CSTART\1gi/' \
    "${run_dir}/session.cmd"
  sed -i '/^N[[:space:]]/a cd\npd\ntd' "${run_dir}/session.cmd"

  cp "${tasking_root}/etc/sim167.cfg" "${run_dir}/simulator.cfg"
  cp "${tasking_root}/etc/xvw.ini" "${run_dir}/xvw.ini"
  "${project_root}/tools/configure-crossview-headless" "${run_dir}/xvw.ini"
}

c166_simulator_launcher() {
  local -n launcher_ref="$1"

  if [[ "${C166_TEST_VISIBLE:-0}" != 1 ]]; then
    c166_require_headless_x
  fi
  launcher_ref=("${project_root}/tools/with-noninteractive-wine" env)
}

c166_run_crossview() {
  local -n launcher_ref="$1"
  local wine_prefix="$2"
  local xfw166="$3"
  local simulator_timeout="$4"
  local run_dir="$5"

  (
    cd "$run_dir"
    timeout "$((simulator_timeout + 15))s" \
      "${launcher_ref[@]}" WINEPREFIX="$wine_prefix" \
        wine "$xfw166" --timeout="$simulator_timeout" -ini xvw.ini \
        -tcfg simulator.cfg -p session.cmd -R session.log
  )
}

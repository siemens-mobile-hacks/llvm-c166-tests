#!/usr/bin/env bash

project_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"

c166_die() {
  echo "error: $*" >&2
  exit 1
}

c166_require_executable() {
  [[ -x "$1" ]] || c166_die "required executable not found: $1"
}

c166_require_wine_program() {
  [[ -f "$1" && -r "$1" ]] || c166_die "required Wine program not found: $1"
}

c166_default_wine_prefix() {
  printf '%s\n' "${WINEPREFIX:-${HOME}/.wine}"
}

c166_require_headless_x() {
  [[ "${C166_HEADLESS_X_ACTIVE:-0}" == 1 ]] ||
    c166_die "headless CrossView must be launched through ./run"
  [[ -n "${DISPLAY:-}" ]] ||
    c166_die "headless CrossView has no virtual DISPLAY"
  [[ -n "${XAUTHORITY:-}" && -f "${XAUTHORITY}" ]] ||
    c166_die "headless CrossView has no private Xauthority file"
}

c166_wine_cli() {
  local wine_prefix="$1"
  shift

  "${project_root}/tools/with-noninteractive-wine" \
    env WINEPREFIX="$wine_prefix" wine "$@"
}

c166_new_run_dir() {
  local name="$1"
  local parent="${project_root}/build/${name}"
  mkdir -p "$parent"
  mktemp -d "${parent}/run.XXXXXXXX"
}

c166_new_simulator_run_dir() {
  local suite="$1"
  local optimization="$2"
  local model="${3:-large}"
  local runtime_variant="${4:-ext}"
  local parent

  [[ "$suite" =~ ^[A-Za-z0-9_.-]+$ ]] ||
    c166_die "invalid ISS suite name: $suite"
  [[ "$optimization" =~ ^O(0|1|2|3|s|z)$ ]] ||
    c166_die "invalid ISS optimization: $optimization"
  [[ "$model" == large || "$model" == medium || "$model" == small ]] ||
    c166_die "invalid ISS model: $model"
  [[ "$runtime_variant" == ext || "$runtime_variant" == ext2 ]] ||
    c166_die "invalid TASKING runtime variant: $runtime_variant"
  if [[ "$runtime_variant" == ext ]]; then
    parent="${project_root}/build/iss"
  else
    parent="${project_root}/build/iss/${runtime_variant}"
  fi
  if [[ "$model" != large ]]; then
    parent="${parent}/${model}"
  fi
  parent="${parent}/${suite}/${optimization}"
  mkdir -p "$parent"
  mktemp -d "${parent}/run.XXXXXXXX"
}

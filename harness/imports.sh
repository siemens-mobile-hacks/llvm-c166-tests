#!/usr/bin/env bash

c166_add_import_placeholders() {
  local -n symbols_ref="$1"
  local -n flags_ref="$2"
  local symbol

  for symbol in "${symbols_ref[@]}"; do
    if ! printf '%s\n' "${flags_ref[@]}" |
        rg -q --fixed-strings -- "--defsym=${symbol}="; then
      flags_ref+=("--defsym=${symbol}=0")
    fi
  done
}

c166_resolve_tasking_imports() {
  local mapper="$1"
  local map="$2"
  local output="$3"
  local -n symbols_ref="$4"
  local -n flags_ref="$5"
  local -a resolved=()
  local -a retained=()
  local flag
  local symbol
  local imported

  mapfile -t resolved < <("$mapper" "$map" "${symbols_ref[@]}")
  for flag in "${flags_ref[@]}"; do
    imported=false
    for symbol in "${symbols_ref[@]}"; do
      if [[ "$flag" == "--defsym=${symbol}="* ]]; then
        imported=true
        break
      fi
    done
    [[ "$imported" == true ]] || retained+=("$flag")
  done
  flags_ref=("${retained[@]}" "${resolved[@]}")
  printf '%s\n' "${resolved[@]}" >"$output"
}

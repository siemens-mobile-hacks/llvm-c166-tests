#!/usr/bin/env bash

c166_elf_has_nonempty_section() {
  local objdump="$1"
  local elf="$2"
  local section_pattern="$3"

  "$objdump" -h "$elf" | awk -v pattern="$section_pattern" '
    $2 ~ pattern && $3 !~ /^0+$/ { found = 1 }
    END { exit !found }
  '
}

c166_configure_overlay_from_elf() {
  local objdump="$1"
  local elf="$2"
  local model="$3"
  local -n overlay_args_ref="$4"
  local -n use_dpp_overlay_ref="$5"

  if [[ "$model" != medium ]] &&
      c166_elf_has_nonempty_section "$objdump" "$elf" \
        '^[.]c166[.]near[.](callers|text)$'; then
    overlay_args_ref=(
      --replace 0xb8000:0xbffff
      --fill 0xb8000:0xbffff
      "${overlay_args_ref[@]}"
    )
  fi
  if c166_elf_has_nonempty_section "$objdump" "$elf" \
       '^[.]c166[.](near|xnear)[.](data|rodata|bss)$'; then
    use_dpp_overlay_ref=true
    overlay_args_ref=(
      --replace 0x5000:0x5fff
      --replace 0x6000:0x6fff
      --fill 0x5000:0x5fff
      --fill 0x6000:0x6fff
      "${overlay_args_ref[@]}"
    )
  fi
}

c166_add_nobits_overlay_ranges() {
  local readobj="$1"
  local elf="$2"
  local -n overlay_args_ref="$3"
  local range

  while IFS= read -r range; do
    [[ -n "$range" ]] && overlay_args_ref+=(--poison "$range")
  done < <(
    "$readobj" --sections --elf-output-style=JSON "$elf" |
      jq -r '
        .[0].Sections[].Section |
        select(.Type.Name == "SHT_NOBITS" and .Size > 0) |
        select(any(.Flags.Flags[]?; .Name == "SHF_ALLOC")) |
        "\(.Address):\(.Address + .Size - 1)"
      '
  )
}

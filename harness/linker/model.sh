#!/usr/bin/env bash

c166_model_configure() {
  local model="$1"
  local -n config_ref="$2"

  config_ref=(
    [name]="$model"
    [tasking_flag]="-Ml"
    [tasking_asm]="LARGE"
    [tasking_library]="l"
    [is_medium]="0"
    [is_small]="0"
    [is_tiny]="0"
    [entry_source]="llvm-entry.s"
    [entry_symbol]="__c166_test_overlay_entry"
    [overlay_entry]="0x100000"
    [crt_entry_symbol]="__c166_crt_overlay_entry"
    [crt_entry]="0x100010"
    [crt_entry_section]=".c166_crt_entry"
    [text_start]="0x180000"
    [data_start]="0x400000"
    [near_callers_start]="0x0b8000"
    [near_text_start]="0x0bc000"
    [icall_start]="0x17ff00"
    [use_dpp_overlay]="false"
  )

  case "$model" in
    large)
      ;;
    medium)
      config_ref[tasking_flag]="-Mm"
      config_ref[tasking_asm]="MEDIUM"
      config_ref[tasking_library]="m"
      config_ref[is_medium]="1"
      config_ref[entry_source]="llvm-entry-medium.s"
      config_ref[entry_symbol]="__c166_test_medium_overlay_entry"
      config_ref[overlay_entry]="0xb000"
      config_ref[crt_entry_symbol]="__c166_crt_medium_overlay_entry"
      config_ref[crt_entry]="0xb010"
      config_ref[crt_entry_section]=".c166_crt_medium_entry"
      config_ref[near_callers_start]="0xc100"
      config_ref[near_text_start]="0xc800"
      config_ref[icall_start]="0xef00"
      ;;
    tiny)
      config_ref[tasking_flag]="-Mt"
      config_ref[tasking_asm]="TINY"
      config_ref[tasking_library]="t"
      config_ref[is_medium]="1"
      config_ref[is_small]="1"
      config_ref[is_tiny]="1"
      config_ref[entry_source]="llvm-entry-medium.s"
      config_ref[entry_symbol]="__c166_test_medium_overlay_entry"
      config_ref[overlay_entry]="0xb000"
      config_ref[crt_entry_symbol]="__c166_crt_medium_overlay_entry"
      config_ref[crt_entry]="0xb010"
      config_ref[crt_entry_section]=".c166_crt_medium_entry"
      config_ref[data_start]="0x5000"
      config_ref[text_start]="0xb020"
      config_ref[near_callers_start]="0xc100"
      config_ref[near_text_start]="0xc800"
      config_ref[icall_start]="0xef00"
      config_ref[use_dpp_overlay]="true"
      ;;
    small)
      config_ref[tasking_flag]="-Ms"
      config_ref[tasking_asm]="SMALL"
      config_ref[tasking_library]="s"
      config_ref[is_small]="1"
      config_ref[data_start]="0x5000"
      config_ref[use_dpp_overlay]="true"
      ;;
    huge)
      config_ref[tasking_flag]="-Mh"
      config_ref[tasking_asm]="HUGE"
      config_ref[tasking_library]="h"
      ;;
    *)
      echo "c166_model_configure: unsupported memory model: ${model}" >&2
      return 1
      ;;
  esac
}

c166_model_link_flags() {
  local model="$1"
  local -n flags_ref="$2"
  flags_ref=()

  if [[ "$model" == small || "$model" == tiny ]]; then
    flags_ref=(
      --section-start=.rodata=0x5400
      --section-start=.c166.small.data=0x5000
      --section-start=.c166.small.rodata=0x5400
      --section-start=.c166.small.far.data=0x4c0000
      --section-start=.c166.small.far.rodata=0x4c0400
      --section-start=.c166.small.huge.data=0x4d0000
      --section-start=.c166.small.huge.rodata=0x4d0400
      --section-start=.c166.small.shuge.data=0x4e0000
      --section-start=.c166.small.shuge.rodata=0x4e0400
    )
  fi
}

c166_model_overlay_args() {
  local model="$1"
  local -n args_ref="$2"

  case "$model" in
    large)
      args_ref=(
        --replace 0x100000:0x2fffff
        --replace 0x400000:0x5fffff
      )
      ;;
    medium)
      args_ref=(
        --replace 0xb000:0xefff
        --fill 0xb000:0xefff
        --replace 0x100000:0x2fffff
        --replace 0x400000:0x5fffff
      )
      ;;
    tiny)
      args_ref=(
        --replace 0x5000:0x6fff
        --fill 0x5000:0x6fff
        --replace 0xb000:0xefff
        --fill 0xb000:0xefff
      )
      ;;
    small)
      args_ref=(
        --replace 0x5000:0x6fff
        --fill 0x5000:0x6fff
        --replace 0x100000:0x2fffff
        --replace 0x400000:0x5fffff
      )
      ;;
    huge)
      args_ref=(
        --replace 0x100000:0x2fffff
        --replace 0x400000:0x5fffff
      )
      ;;
    *)
      echo "c166_model_overlay_args: unsupported memory model: ${model}" >&2
      return 1
      ;;
  esac
  args_ref+=(--replace 0xf600:0xf6ff)
}

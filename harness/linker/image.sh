#!/usr/bin/env bash

c166_link_llvm_image() {
  local lld="$1"
  local output="$2"
  local llvm_entry="$3"
  local -n config_ref="$4"
  local -n model_flags_ref="$5"
  local -n extra_flags_ref="$6"
  local -n inputs_ref="$7"

  "$lld" -m c166elf \
    "-Ttext=${config_ref[text_start]}" \
    "-Tdata=${config_ref[data_start]}" \
    "-Tbss=${config_ref[bss_start]}" \
    "${model_flags_ref[@]}" \
    --section-start=.c166.near.callers="${config_ref[near_callers_start]}" \
    --section-start=.c166.near.text="${config_ref[near_text_start]}" \
    --section-start=.c166.near.data=0x5000 \
    --section-start=.c166.near.rodata=0x5400 \
    --section-start=.c166.near.bss=0x5800 \
    --section-start=.c166.xnear.data=0x6000 \
    --section-start=.c166.xnear.rodata=0x6400 \
    --section-start=.c166.xnear.bss=0x6800 \
    --section-start=.c166.icall="${config_ref[icall_start]}" \
    --section-start=.c166_test_entry="${config_ref[overlay_entry]}" \
    --section-start=.c166_test_medium_entry="${config_ref[overlay_entry]}" \
    --defsym="__c166_test_target=${llvm_entry}" \
    --entry="${config_ref[entry_symbol]}" \
    "${extra_flags_ref[@]}" "${inputs_ref[@]}" -o "$output"
}

c166_emit_llvm_evidence() {
  local objcopy="$1"
  local objdump="$2"
  local nm="$3"
  local elf="$4"
  local output_dir="$5"

  "$objcopy" -O ihex "$elf" "${output_dir}/llvm.hex"
  "$objdump" -d "$elf" >"${output_dir}/llvm.dis"
  "$nm" -n "$elf" >"${output_dir}/llvm.nm"
}

c166_verify_llvm_image() {
  local run_dir="$1"
  local case_dir="$2"
  local model="$3"
  local manifest="$4"
  local -n config_ref="$5"
  local -n required_symbols_ref="$6"
  local symbol_checker="$7"
  local expected_nm="${case_dir}/expected.nm.${model}"
  local expected_dis="${case_dir}/expected.dis.${model}"
  local pattern

  if rg -q '[[:space:]]__icall$' "${run_dir}/llvm.nm"; then
    local llvm_icall_address
    printf -v llvm_icall_address '%08x' "$((config_ref[icall_start]))"
    rg -q "^${llvm_icall_address} [Tt] __icall$" "${run_dir}/llvm.nm" ||
      c166_die "__icall is not at ${config_ref[icall_start]}"
  fi

  local llvm_entry_address
  printf -v llvm_entry_address '%08x' "$((config_ref[overlay_entry]))"
  rg -q "^${llvm_entry_address} [Tt] ${config_ref[entry_symbol]}$" \
    "${run_dir}/llvm.nm" ||
    c166_die "LLVM trampoline is not at ${config_ref[overlay_entry]}"

  if ((${#required_symbols_ref[@]})); then
    "$symbol_checker" "${run_dir}/llvm.nm" "${required_symbols_ref[@]}" ||
      c166_die "required LLVM symbol check failed: ${run_dir}/llvm.nm"
  fi

  [[ -f "$expected_nm" ]] || expected_nm="${case_dir}/expected.nm"
  if [[ -f "$expected_nm" ]]; then
    while IFS= read -r pattern; do
      pattern="${pattern%$'\r'}"
      [[ -n "$pattern" && "$pattern" != \#* ]] || continue
      rg -q -- "$pattern" "${run_dir}/llvm.nm" ||
        c166_die "missing LLVM symbol pattern '${pattern}'"
    done <"$expected_nm"
  fi

  [[ -f "$expected_dis" ]] || expected_dis="${case_dir}/expected.dis"
  if [[ -f "$expected_dis" ]]; then
    cp "$expected_dis" "${run_dir}/expected.dis"
    while IFS= read -r pattern; do
      pattern="${pattern%$'\r'}"
      [[ -n "$pattern" && "$pattern" != \#* ]] || continue
      rg -q -- "$pattern" "${run_dir}/llvm.dis" ||
        c166_die "missing LLVM disassembly pattern '${pattern}'"
    done <"$expected_dis"
  fi

  while IFS= read -r record; do
    local count
    local minimum
    minimum="$(jq -er '.min' <<<"$record")"
    pattern="$(jq -er '.pattern' <<<"$record")"
    count="$(rg -c -- "$pattern" "${run_dir}/llvm.dis" || true)"
    count="${count:-0}"
    ((count >= minimum)) ||
      c166_die "disassembly pattern '${pattern}' count ${count} is below ${minimum}"
  done < <(jq -c '.disassembly_min_counts[]?' "$manifest")
}

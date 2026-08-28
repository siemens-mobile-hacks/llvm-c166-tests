#!/usr/bin/env bash

c166_build_llvm_objects() {
  local clang="$1"
  local llc="$2"
  local model="$3"
  local optimization="$4"
  local run_dir="$5"
  local entry_source="$6"
  local runtime_builtins="$7"
  local -n sources_ref="$8"
  local -n mir_sources_ref="$9"
  local -n clang_flags_ref="${10}"
  local -n defines_ref="${11}"
  local -n link_inputs_ref="${12}"
  local entry_object="${run_dir}/llvm-entry.o"
  local runtime_archive
  local source
  local object
  local post_mir
  local index=0

  "$clang" --target=c166-none-elf "-mcmodel=${model}" \
    -c "$entry_source" -o "$entry_object"
  link_inputs_ref+=("$entry_object")

  for source in "${sources_ref[@]}"; do
    object="${run_dir}/llvm-${index}.o"
    "$clang" --target=c166-none-elf "-mcmodel=${model}" "-${optimization}" \
      -mllvm -verify-machineinstrs "${clang_flags_ref[@]}" \
      "${defines_ref[@]/#/-D}" \
      -c "${run_dir}/${source}" -o "$object"
    link_inputs_ref+=("$object")
    index=$((index + 1))
  done

  index=0
  for source in "${mir_sources_ref[@]}"; do
    post_mir="${run_dir}/llvm-mir-${index}-post.mir"
    object="${run_dir}/llvm-mir-${index}.o"
    "$llc" -mtriple=c166-none-elf -run-pass=postrapseudos \
      -verify-machineinstrs -o "$post_mir" "${run_dir}/${source}"
    "$llc" -mtriple=c166-none-elf -start-after=postrapseudos \
      -verify-machineinstrs -filetype=obj -o "$object" "$post_mir"
    link_inputs_ref+=("$object")
    index=$((index + 1))
  done

  if [[ "$runtime_builtins" == true ]]; then
    runtime_archive="$("$clang" --target=c166-none-elf "-mcmodel=${model}" \
      --print-libgcc-file-name)"
    [[ -f "$runtime_archive" ]] ||
      c166_die "missing C166 compiler-rt archive: ${runtime_archive}"
    link_inputs_ref+=("$runtime_archive")
  fi
}

c166_build_tasking_oracle() {
  local run_dir="$1"
  local wine_prefix="$2"
  local -n case_ref="$3"
  local -n model_ref="$4"
  local -n tools_ref="$5"
  local -n model_flags_ref="$6"
  local -n defines_ref="$7"
  local -n link_flags_ref="$8"
  local -n sources_ref="$9"
  local -n nodebug_sources_ref="${10}"
  local -n asm_sources_ref="${11}"
  local -a objects=(proxy.obj)
  local source
  local object
  local preprocessed
  local index=0
  local -a architecture_flags=()

  if [[ -n "${case_ref[tasking_arch_flag]}" ]]; then
    architecture_flags+=("${case_ref[tasking_arch_flag]}")
  fi

  (
    cd "$run_dir"
    c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
      proxy.asm TO proxy.src NOPR \
      "DEFINE(TASKING_MODEL_IS_MEDIUM,${model_ref[is_medium]})" \
      "DEFINE(TASKING_MODEL_IS_SMALL,${model_ref[is_small]})"
    c166_wine_cli "$wine_prefix" "${tools_ref[a166]}" \
      proxy.src TO proxy.obj NOPR "${case_ref[tasking_asm_arch]}" \
      "MODEL(${model_ref[tasking_asm]})"
    c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
      layout.ilo TO layout.src NOPR \
      "DEFINE(TASKING_MODEL_IS_MEDIUM,${model_ref[is_medium]})" \
      "DEFINE(TASKING_MODEL_IS_SMALL,${model_ref[is_small]})"

    if [[ "${case_ref[startup_policy]}" == minimal ]]; then
      c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
        test-startup.asm TO test-startup.src NOPR \
        "DEFINE(TASKING_MODEL_IS_MEDIUM,${model_ref[is_medium]})" \
        "DEFINE(TASKING_MODEL_IS_SMALL,${model_ref[is_small]})"
      c166_wine_cli "$wine_prefix" "${tools_ref[a166]}" \
        test-startup.src TO test-startup.obj NOPR \
        "${case_ref[tasking_asm_arch]}" \
        "MODEL(${model_ref[tasking_asm]})"
      objects+=(test-startup.obj)
    else
      c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
        "${case_ref[tasking_cstart]}" TO cstart.src NOPR \
        "INCLUDEPATH('${case_ref[tasking_include_windows]}')" \
        "DEFINE(MODEL,${model_ref[tasking_asm]})"
      c166_wine_cli "$wine_prefix" "${tools_ref[a166]}" \
        cstart.src TO cstart.obj NOPR "${case_ref[tasking_asm_arch]}" \
        "MODEL(${model_ref[tasking_asm]})"
      objects+=(cstart.obj)
    fi

    if [[ "${model_ref[use_dpp_overlay]}" == true ]]; then
      c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
        dpp-overlay.asm TO dpp-overlay.src NOPR \
        "DEFINE(TASKING_MODEL_IS_SMALL,${model_ref[is_small]})"
      c166_wine_cli "$wine_prefix" "${tools_ref[a166]}" \
        dpp-overlay.src TO dpp-overlay.obj NOPR \
        "${case_ref[tasking_asm_arch]}" \
        "MODEL(${model_ref[tasking_asm]})"
      objects+=(dpp-overlay.obj)
    fi

    for source in "${asm_sources_ref[@]}"; do
      object="tasking-asm-${index}.obj"
      preprocessed="tasking-asm-${index}.src"
      c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
        "$source" TO "$preprocessed" NOPR \
        "DEFINE(TASKING_MODEL_IS_MEDIUM,${model_ref[is_medium]})" \
        "DEFINE(TASKING_MODEL_IS_SMALL,${model_ref[is_small]})"
      c166_wine_cli "$wine_prefix" "${tools_ref[a166]}" \
        "$preprocessed" TO "$object" NOPR \
        "${case_ref[tasking_asm_arch]}" \
        "MODEL(${model_ref[tasking_asm]})"
      objects+=("$object")
      index=$((index + 1))
    done

    index=0
    for source in "${sources_ref[@]}"; do
      object="tasking-${index}.obj"
      c166_wine_cli "$wine_prefix" "${tools_ref[cc166]}" \
        "${model_ref[tasking_flag]}" "${architecture_flags[@]}" \
        "${model_flags_ref[@]}" \
        "${defines_ref[@]/#/-D}" -g -tmp -c -o "$object" "$source"
      objects+=("$object")
      index=$((index + 1))
    done
    for source in "${nodebug_sources_ref[@]}"; do
      object="tasking-${index}.obj"
      c166_wine_cli "$wine_prefix" "${tools_ref[cc166]}" \
        "${model_ref[tasking_flag]}" "${architecture_flags[@]}" \
        "${model_flags_ref[@]}" \
        "${defines_ref[@]/#/-D}" -tmp -c -o "$object" "$source"
      objects+=("$object")
      index=$((index + 1))
    done
    c166_wine_cli "$wine_prefix" "${tools_ref[cc166]}" \
      "${model_ref[tasking_flag]}" "${architecture_flags[@]}" \
      "${model_flags_ref[@]}" \
      "${defines_ref[@]/#/-D}" \
      -g -ieee "${link_flags_ref[@]}" -tmp -Wo@layout.src -v \
      -o host.abs "${case_ref[tasking_host]}" "${objects[@]}"
  )
}

c166_verify_tasking_symbols() {
  local run_dir="$1"
  local symbol_mapper="$2"
  local -n sources_ref="$3"
  local -n nodebug_sources_ref="$4"
  local -n required_symbols_ref="$5"
  local source
  local symbol
  local -a generated_sources=()

  ((${#required_symbols_ref[@]})) || return 0
  for source in "${sources_ref[@]}" "${nodebug_sources_ref[@]}"; do
    source="${run_dir}/${source%.c}.src"
    [[ -f "$source" ]] ||
      c166_die "missing retained TASKING assembly source: ${source}"
    generated_sources+=("$source")
  done

  for symbol in "${required_symbols_ref[@]}"; do
    rg -q "(^|[^A-Za-z0-9_])${symbol}([^A-Za-z0-9_]|$)" \
      "${generated_sources[@]}" ||
      c166_die "TASKING compiler did not reference required symbol: ${symbol}"
  done
  "$symbol_mapper" "${run_dir}/host.map" "${required_symbols_ref[@]}" \
    >"${run_dir}/tasking-required-symbols.txt" ||
    c166_die "TASKING linker did not resolve every required symbol"
}

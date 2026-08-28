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
  local tasking_model_flag="$3"
  local tasking_asm_model="$4"
  local tasking_startup_policy="$5"
  local tasking_cstart="$6"
  local tasking_include_windows="$7"
  local use_dpp_overlay="$8"
  local tasking_host="$9"
  local -n tools_ref="${10}"
  local -n model_flags_ref="${11}"
  local -n defines_ref="${12}"
  local -n link_flags_ref="${13}"
  local -n sources_ref="${14}"
  local -n nodebug_sources_ref="${15}"
  local -n asm_sources_ref="${16}"
  local tasking_model_is_medium="${17}"
  local tasking_model_is_small="${18}"
  local -a objects=(proxy.obj)
  local source
  local object
  local preprocessed
  local index=0

  (
    cd "$run_dir"
    c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
      proxy.asm TO proxy.src NOPR \
      "DEFINE(TASKING_MODEL_IS_MEDIUM,${tasking_model_is_medium})" \
      "DEFINE(TASKING_MODEL_IS_SMALL,${tasking_model_is_small})"
    c166_wine_cli "$wine_prefix" "${tools_ref[a166]}" \
      proxy.src TO proxy.obj NOPR EXTEND "MODEL(${tasking_asm_model})"
    c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
      layout.ilo TO layout.src NOPR \
      "DEFINE(TASKING_MODEL_IS_MEDIUM,${tasking_model_is_medium})" \
      "DEFINE(TASKING_MODEL_IS_SMALL,${tasking_model_is_small})"

    if [[ "$tasking_startup_policy" == minimal ]]; then
      c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
        test-startup.asm TO test-startup.src NOPR \
        "DEFINE(TASKING_MODEL_IS_MEDIUM,${tasking_model_is_medium})" \
        "DEFINE(TASKING_MODEL_IS_SMALL,${tasking_model_is_small})"
      c166_wine_cli "$wine_prefix" "${tools_ref[a166]}" \
        test-startup.src TO test-startup.obj NOPR EXTEND \
        "MODEL(${tasking_asm_model})"
      objects+=(test-startup.obj)
    else
      c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
        "$tasking_cstart" TO cstart.src NOPR \
        "INCLUDEPATH('${tasking_include_windows}')" \
        "DEFINE(MODEL,${tasking_asm_model})"
      c166_wine_cli "$wine_prefix" "${tools_ref[a166]}" \
        cstart.src TO cstart.obj NOPR EXTEND \
        "MODEL(${tasking_asm_model})"
      objects+=(cstart.obj)
    fi

    if [[ "$use_dpp_overlay" == true ]]; then
      c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
        dpp-overlay.asm TO dpp-overlay.src NOPR \
        "DEFINE(TASKING_MODEL_IS_SMALL,${tasking_model_is_small})"
      c166_wine_cli "$wine_prefix" "${tools_ref[a166]}" \
        dpp-overlay.src TO dpp-overlay.obj NOPR EXTEND \
        "MODEL(${tasking_asm_model})"
      objects+=(dpp-overlay.obj)
    fi

    for source in "${asm_sources_ref[@]}"; do
      object="tasking-asm-${index}.obj"
      preprocessed="tasking-asm-${index}.src"
      c166_wine_cli "$wine_prefix" "${tools_ref[m166]}" \
        "$source" TO "$preprocessed" NOPR \
        "DEFINE(TASKING_MODEL_IS_MEDIUM,${tasking_model_is_medium})" \
        "DEFINE(TASKING_MODEL_IS_SMALL,${tasking_model_is_small})"
      c166_wine_cli "$wine_prefix" "${tools_ref[a166]}" \
        "$preprocessed" TO "$object" NOPR EXTEND \
        "MODEL(${tasking_asm_model})"
      objects+=("$object")
      index=$((index + 1))
    done

    index=0
    for source in "${sources_ref[@]}"; do
      object="tasking-${index}.obj"
      c166_wine_cli "$wine_prefix" "${tools_ref[cc166]}" \
        "$tasking_model_flag" "${model_flags_ref[@]}" \
        "${defines_ref[@]/#/-D}" -g -c -o "$object" "$source"
      objects+=("$object")
      index=$((index + 1))
    done
    for source in "${nodebug_sources_ref[@]}"; do
      object="tasking-${index}.obj"
      c166_wine_cli "$wine_prefix" "${tools_ref[cc166]}" \
        "$tasking_model_flag" "${model_flags_ref[@]}" \
        "${defines_ref[@]/#/-D}" -c -o "$object" "$source"
      objects+=("$object")
      index=$((index + 1))
    done
    c166_wine_cli "$wine_prefix" "${tools_ref[cc166]}" \
      "$tasking_model_flag" "${model_flags_ref[@]}" \
      "${defines_ref[@]/#/-D}" \
      -g -ieee "${link_flags_ref[@]}" -tmp -Wo@layout.src -v \
      -o host.abs "$tasking_host" "${objects[@]}"
  )
}

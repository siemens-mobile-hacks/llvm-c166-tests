#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "c166-address-spaces.h"
#include "types.h"
#include "vectors.inc"

extern double tasking_double_identity(double value);
extern double llvm_tasking_double_identity_proxy(double value);
extern volatile abi_double_words tasking_double_observed;
extern volatile abi_double_words llvm_tasking_double_observed;
extern abi_block8 _near tasking_near_source;
extern abi_block8 _near tasking_near_destination;
extern abi_block8 _far tasking_far_source;
extern abi_block8 _far tasking_far_destination;

extern void tasking_helper_load_store_near(void);
extern void tasking_helper_load_store_far(void);
extern void tasking_helper_ldnoal_near(void);
extern void tasking_helper_ldnoal_far(void);
extern void tasking_helper_cpnnb(void);
extern void tasking_helper_cpnnw(void);
extern void tasking_helper_cpnfb(void);
extern void tasking_helper_cpnfw(void);
extern void tasking_helper_cpfnb(void);
extern void tasking_helper_cpfnw(void);

static void check_words(abi_u16 base_case, abi_double_words *value,
                        abi_u16 w3, abi_u16 w2, abi_u16 w1, abi_u16 w0)
{
  c166_test_check_u32(base_case + 0U, w3, value->words[0]);
  c166_test_check_u32(base_case + 1U, w2, value->words[1]);
  c166_test_check_u32(base_case + 2U, w1, value->words[2]);
  c166_test_check_u32(base_case + 3U, w0, value->words[3]);
}

static void run_vector(abi_u16 vector_id, abi_u16 w3, abi_u16 w2,
                       abi_u16 w1, abi_u16 w0)
{
  abi_double_words input;
  abi_double_words direct;
  abi_double_words reverse;
  abi_u16 base_case = (abi_u16)((vector_id - 1U) * 16U + 1U);

  input.words[0] = w3;
  input.words[1] = w2;
  input.words[2] = w1;
  input.words[3] = w0;
  direct.value = tasking_double_identity(input.value);
  reverse.value = llvm_tasking_double_identity_proxy(input.value);
  check_words(base_case, &direct, w3, w2, w1, w0);
  check_words((abi_u16)(base_case + 4U),
              (abi_double_words *)&tasking_double_observed,
              w3, w2, w1, w0);
  check_words((abi_u16)(base_case + 8U),
              (abi_double_words *)&llvm_tasking_double_observed,
              w3, w2, w1, w0);
  check_words((abi_u16)(base_case + 12U), &reverse, w3, w2, w1, w0);
}

#define RUN_VECTOR(id, w3, w2, w1, w0) run_vector(id, w3, w2, w1, w0);

static void fill_block(abi_block8 *block, abi_u8 seed)
{
  abi_u16 index;
  for (index = 0; index != 8U; ++index)
    block->bytes[index] = (abi_u8)(seed + (abi_u8)(index * 17U));
}

static void clear_block(abi_block8 *block)
{
  abi_u16 index;
  for (index = 0; index != 8U; ++index)
    block->bytes[index] = 0U;
}

static void check_block(abi_u16 base_case, abi_block8 *block, abi_u8 seed)
{
  abi_u16 index;
  for (index = 0; index != 8U; ++index)
    c166_test_check_u32((abi_u16)(base_case + index),
                        (abi_u8)(seed + (abi_u8)(index * 17U)),
                        block->bytes[index]);
}

static void prepare_near(abi_u8 seed)
{
  fill_block(&tasking_near_source, seed);
  clear_block(&tasking_near_destination);
}

static void prepare_far(abi_u8 seed)
{
  fill_block(&tasking_far_source, seed);
  clear_block(&tasking_far_destination);
}

static void run_helper_matrix(void)
{
  prepare_near(0x11U);
  tasking_helper_load_store_near();
  check_block(161U, &tasking_near_destination, 0x11U);

  prepare_far(0x22U);
  tasking_helper_load_store_far();
  check_block(169U, &tasking_far_destination, 0x22U);

  prepare_near(0x33U);
  tasking_helper_ldnoal_near();
  check_block(177U, &tasking_near_destination, 0x33U);

  prepare_far(0x44U);
  clear_block(&tasking_near_destination);
  tasking_helper_ldnoal_far();
  check_block(185U, &tasking_near_destination, 0x44U);

  prepare_near(0x55U);
  tasking_helper_cpnnb();
  check_block(193U, &tasking_near_destination, 0x55U);

  prepare_near(0x66U);
  tasking_helper_cpnnw();
  check_block(201U, &tasking_near_destination, 0x66U);

  prepare_near(0x77U);
  clear_block(&tasking_far_destination);
  tasking_helper_cpnfb();
  check_block(209U, &tasking_far_destination, 0x77U);

  prepare_near(0x88U);
  clear_block(&tasking_far_destination);
  tasking_helper_cpnfw();
  check_block(217U, &tasking_far_destination, 0x88U);

  prepare_far(0x99U);
  clear_block(&tasking_near_destination);
  tasking_helper_cpfnb();
  check_block(225U, &tasking_near_destination, 0x99U);

  prepare_far(0xAAU);
  clear_block(&tasking_near_destination);
  tasking_helper_cpfnw();
  check_block(233U, &tasking_near_destination, 0xAAU);
}

void main(void)
{
  c166_test_begin(296, 0x16600128UL);
  ABI_TASKING_REVERSE_DOUBLE_VECTORS(RUN_VECTOR)
  run_helper_matrix();
  c166_test_finish();
  simulator_stop();
}


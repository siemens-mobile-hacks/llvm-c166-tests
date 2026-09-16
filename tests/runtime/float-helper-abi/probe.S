#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
        .section .c166.near.text,"ax",@progbits

        .macro begin_function name
        .globl  \name
        .type   \name,@function
        .c166_function near, \name
\name:
        .endm

        .macro call_function name
        calla   cc_uc, cof(\name)
        .endm

        .macro end_function name
        ret
        .size   \name, .-\name
        .endm
#else
        .text

        .macro begin_function name
        .globl  \name
        .type   \name,@function
        .c166_function huge, \name
\name:
        .endm

        .macro call_function name
        calls   seg(\name), sof(\name)
        .endm

        .macro end_function name
        rets
        .size   \name, .-\name
        .endm
#endif

        .p2align 1

        begin_function _helper_load_store_near
        mov     r4, #dpp2(_near_source)
        call_function __load8n
        mov     r4, #dpp2(_near_destination)
        call_function __store8n
        add     r0, #8
        end_function _helper_load_store_near

#if __C166_MEMORY_MODEL__ != 4
        begin_function _helper_load_store_far
        mov     r4, #pof(_far_source)
        mov     r5, #pag(_far_source)
        call_function __load8f
        mov     r4, #pof(_far_destination)
        mov     r5, #pag(_far_destination)
        call_function __store8f
        add     r0, #8
        end_function _helper_load_store_far
#endif

        begin_function _helper_ldnoal_near
        mov     r4, #dpp2(_near_source)
        mov     r10, #dpp2(_near_destination)
        call_function __ldnoal8n
        end_function _helper_ldnoal_near

#if __C166_MEMORY_MODEL__ != 4
        begin_function _helper_ldnoal_far
        mov     r4, #pof(_far_source)
        mov     r5, #pag(_far_source)
        mov     r10, #dpp2(_near_destination)
        call_function __ldnoal8f
        end_function _helper_ldnoal_far
#endif

        begin_function _helper_cpnnb
        mov     r4, #dpp2(_near_source)
        mov     r10, #dpp2(_near_destination)
        mov     r3, #8
        call_function __cpnnb
        end_function _helper_cpnnb

        begin_function _helper_cpnnw
        mov     r4, #dpp2(_near_source)
        mov     r10, #dpp2(_near_destination)
        mov     r3, #4
        call_function __cpnnw
        end_function _helper_cpnnw

#if __C166_MEMORY_MODEL__ != 4
        begin_function _helper_cpnfb
        mov     r4, #dpp2(_near_source)
        mov     r10, #pof(_far_destination)
        mov     r11, #pag(_far_destination)
        mov     r3, #8
        call_function __cpnfb
        end_function _helper_cpnfb

        begin_function _helper_cpnfw
        mov     r4, #dpp2(_near_source)
        mov     r10, #pof(_far_destination)
        mov     r11, #pag(_far_destination)
        mov     r3, #4
        call_function __cpnfw
        end_function _helper_cpnfw

        begin_function _helper_cpfnb
        mov     r4, #pof(_far_source)
        mov     r5, #pag(_far_source)
        mov     r10, #dpp2(_near_destination)
        mov     r3, #8
        call_function __cpfnb
        end_function _helper_cpfnb

        begin_function _helper_cpfnw
        mov     r4, #pof(_far_source)
        mov     r5, #pag(_far_source)
        mov     r10, #dpp2(_near_destination)
        mov     r3, #4
        call_function __cpfnw
        end_function _helper_cpfnw
#endif

        .section .tasking_float_entry,"ax",@progbits
        .p2align 1

        .globl  _tasking_runtime_load8n_entry
_tasking_runtime_load8n_entry:
        jmps    seg(__load8n), sof(__load8n)

        .globl  _tasking_runtime_ldnoal8n_entry
_tasking_runtime_ldnoal8n_entry:
        jmps    seg(__ldnoal8n), sof(__ldnoal8n)

        .globl  _tasking_runtime_store8n_entry
_tasking_runtime_store8n_entry:
        jmps    seg(__store8n), sof(__store8n)

        .globl  _tasking_runtime_load8f_entry
_tasking_runtime_load8f_entry:
        jmps    seg(__load8f), sof(__load8f)

        .globl  _tasking_runtime_ldnoal8f_entry
_tasking_runtime_ldnoal8f_entry:
        jmps    seg(__ldnoal8f), sof(__ldnoal8f)

        .globl  _tasking_runtime_store8f_entry
_tasking_runtime_store8f_entry:
        jmps    seg(__store8f), sof(__store8f)

        .globl  _tasking_runtime_cpnnb_entry
_tasking_runtime_cpnnb_entry:
        jmps    seg(__cpnnb), sof(__cpnnb)

        .globl  _tasking_runtime_cpnnw_entry
_tasking_runtime_cpnnw_entry:
        jmps    seg(__cpnnw), sof(__cpnnw)

        .globl  _tasking_runtime_cpnfb_entry
_tasking_runtime_cpnfb_entry:
        jmps    seg(__cpnfb), sof(__cpnfb)

        .globl  _tasking_runtime_cpnfw_entry
_tasking_runtime_cpnfw_entry:
        jmps    seg(__cpnfw), sof(__cpnfw)

        .globl  _tasking_runtime_cpfnb_entry
_tasking_runtime_cpfnb_entry:
        jmps    seg(__cpfnb), sof(__cpfnb)

        .globl  _tasking_runtime_cpfnw_entry
_tasking_runtime_cpfnw_entry:
        jmps    seg(__cpfnw), sof(__cpfnw)

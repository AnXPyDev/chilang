#include "lib/include.h"
#include "simulator/include.h"

int main() {
    std_streams_init();

    VmStack *stack = malloc(sizeof(VmStack));
    stack->fptr = stack->begin;

    VmInstance vm = {
        .os_out = os_stdout,
        .os_err = os_stderr,
        .os_log = os_stderr
    };

    VmProcedure *procedure = malloc(sizeof(VmProcedure) + sizeof(VmOperation) * 128);

    uint32_t x = 421;

    VmOperation *op = procedure->ops;
    op[0].opcode = VM_OPERATION_COPY_PTR;
    *(VmOperation_COPY_PTR*)op[0].data = (VmOperation_COPY_PTR) {
        .src = &x,
        .dest = 0,
        .n = sizeof(uint32_t)
    };
    op[1].opcode = VM_OPERATION_INCREMENT_U32;
    *(VmAddress*)op[1].data = 0;
    op[2].opcode = VM_OPERATION_PRINT_U32;
    *(VmAddress*)op[2].data = 0;
    op[3].opcode = VM_OPERATION_PRINT_ENDL;
    op[4].opcode = VM_OPERATION_SAYHI;
    op[5].opcode = VM_OPERATION_PRINT_ENDL;
    op[6].opcode = VM_OPERATION_RETURN;

    procedure->length = 7;

    VmExecutionCode res = VmInstance_execute_procedure(&vm, stack, procedure);
    
    fprintf(stderr, "\nvm code: %d\n", res);
    return 0;
}
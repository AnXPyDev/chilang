typedef enum {
    VM_EX_OK = 0,
    VM_EX_ERR
} VmExecutionCode;

VmExecutionCode VmInstance_execute_procedure(
    VmInstance *this,
    VmStack *stack,
    VmProcedure *procedure
) {
    char strbuffer[256];
    const Size opcnt = procedure->length; 
    Size opidx = 0;
    while (1) {
        if (opidx >= opcnt) {
            break;
        }

        VmOperation *op = &procedure->ops[opidx];
        opidx++;

        void *vdata = &op->data;
        switch (op->opcode) {
            case VM_OPERATION_NOP:;
                break;
            case VM_OPERATION_RETURN:;
                return VM_EX_OK;
            case VM_OPERATION_COPY: {
                VmOperation_COPY *data = vdata;
                void *src = VmStack_getPtr(stack, data->src);
                void *dest = VmStack_getPtr(stack, data->dest);
                memcpy(dest, src, data->n);
            }; break;
            case VM_OPERATION_COPY_PTR: {
                VmOperation_COPY_PTR *data = vdata;
                void *src = data->src;
                void *dest = VmStack_getPtr(stack, data->dest);
                memcpy(dest, src, data->n);
            }; break;
            case VM_OPERATION_PRINT_U32: {
                VmAddress *data = vdata;
                uint32_t *src = VmStack_getPtr(stack, *data);
                sprintf(strbuffer, "%u", *src);
                OutStream_puts(this->os_out, strbuffer);
            }; break;
            case VM_OPERATION_PRINT_ENDL: {
                OutStream_puts(this->os_out, "\n");
            }; break;
            case VM_OPERATION_INCREMENT_U32: {
                VmAddress *data = vdata;
                uint32_t *src = VmStack_getPtr(stack, *data);
                (*src)++;
            }; break;
            case VM_OPERATION_SAYHI: {
                OutStream_puts(this->os_out, "Ajooooj");
            }; break;
            default: {
                sprintf(strbuffer, "unsupported operation: %d", op->opcode);
                OutStream_puts(this->os_log, strbuffer);
                return VM_EX_ERR;
            }; break;
        }
    }

    return VM_EX_ERR;
}
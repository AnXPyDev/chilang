
#define AT(T, offset) ((T*)(frame + (offset)))

VmExitCode VmInstance_execute_procedure(
    VmInstance *this,
    VmProcedure *procedure,
    char *frame
) {
    char strbuffer[256];
    const Size opcnt = procedure->length; 
    Size opidx = 0;
    while (1) {
        if (opidx >= opcnt) {
            break;
        }

        VmOperation *op = &procedure->ops[opidx];
        void *vdata = &op->data;

        opidx++;

        switch (op->opcode) {
            case VM_OPERATION_NOP: break;

            case VM_OPERATION_RET: {
                VmOperation_RET *data = vdata;
            }; break;

            case VM_OPEARTION_CALL: {
                VmOpeartion_CALL *data = vdata;
                char *newFrame = AT(char, data->frame);
                VmExitCode code = VmInstance_execute_procedure(this, data->procedure, newFrame);
                if (code != VM_EXIT_OK) {
                    return code;
                }
            }; break;

            case VM_OPERATION_CPY: {
                VmOperation_CPY *data = vdata;
                memcpy(AT(void, data->dest), AT(void, data->src), data->n);
            }; break;

            case VM_OPERATION_READ: {
                VmOperation_READ *data = vdata;
                memcpy(AT(void, data->dest), data->src, data->n);
            }; break;

            case VM_OPERATION_WRITE: {
                VmOperation_WRITE *data = vdata;
                memcpy(data->dest, AT(void, data->src), data->n);
            }; break;

            case VM_OPERATION_JMP: {
                VmOpeartion_JMP *data = vdata;
                opidx = data->target;
            }; break;

            case VM_OPERATION_JMP_Z: {
                VmOperation_JMP *data = vdata;
                if (*AT(VmInt, data->cond) == 0) {
                    opidx = data->target;
                }
            }; break;
            
            case VM_OPERATION_JMP_L: {
                VmOperation_JMP *data = vdata;
                if (*AT(VmInt, data->cond) < 0) {
                    opidx = data->target;
                }
            }; break;
            
            case VM_OPERATION_JMP_G: {
                VmOperation_JMP *data = vdata;
                if (*AT(VmInt, data->cond) > 0) {
                    opidx = data->target;
                }
            }; break;
            
            case VM_OPERATION_JMP_ZL: {
                VmOperation_JMP *data = vdata;
                if (*AT(VmInt, data->cond) <= 0) {
                    opidx = data->target;
                }
            }; break;
            
            case VM_OPERATION_JMP_ZG: {
                VmOperation_JMP *data = vdata;
                if (*AT(VmInt, data->cond) >= 0) {
                    opidx = data->target;
                }
            }; break;
            
            case VM_OPERATION_SET: {
                VmOperation_SET *data = vdata;
                *AT(VmInt, data->dest) = *AT(VmInt, data->val);
            }; break;

            case VM_OPERATION_ADD: {
                VmOperation_GENERIC *data = vdata;
                *AT(VmInt, data->r) = *AT(VmInt, data->a) + *AT(VmInt, data->b);
            }; break;
            
            case VM_OPERATION_SUB: {
                VmOperation_GENERIC *data = vdata;
                *AT(VmInt, data->r) = *AT(VmInt, data->a) - *AT(VmInt, data->b);
            }; break;
            
            case VM_OPERATION_MUL: {
                VmOperation_GENERIC *data = vdata;
                *AT(VmInt, data->r) = *AT(VmInt, data->a) * *AT(VmInt, data->b);
            }; break;
            
            case VM_OPERATION_DIV: {
                VmOperation_GENERIC *data = vdata;
                *AT(VmInt, data->r) = *AT(VmInt, data->a) / *AT(VmInt, data->b);
            }; break;
            
            case VM_OPERATION_MOD: {
                VmOperation_GENERIC *data = vdata;
                *AT(VmInt, data->r) = *AT(VmInt, data->a) % *AT(VmInt, data->b);
            }; break;
            
            case VM_OPERATION_CMP: {
                VmOperation_GENERIC *data = vdata;
                VmInt x = *AT(VmInt, data->a) - *AT(VmInt, data->b);
                *AT(VmInt, data->r) = (x > 0) - (x < 0);
            }; break;

            case VM_OPERATION_PRINT: {
                VmOperation_GENERIC *data = vdata;
                sprintf(strbuffer, "%d", *AT(VmInt, data->a));
                OutStream_puts(this->os_out, strbuffer);
            }; break;

            case VM_OPERATION_PRINT_CHAR: {
                VmOperation_GENERIC *data = vdata;
                VmInt c = *AT(VmInt, data->a);
                OutStream_putc(this->os_out, (int)c);
            }; break;

            default: {
                sprintf(strbuffer, "\nVM: Unsupported operation %d\n", op->opcode);
                OutStream_puts(this->os_log, strbuffer);
                return VM_EXIT_ERR;
            }; break;
        }
    }

    return VM_EXIT_ERR;
}

#undef ADDR
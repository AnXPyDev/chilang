SimResult Expression_simulate(Expression this, Simulator *simulator, SimState state, SimValue **out_value);

SimResult GetExpression_simulate(GetExpression *this, Simulator *simulator, SimState state, SimValue **out_value) {
    SimResult result = SimFrame_getValue(state.frame, this->member, out_value);
    return result;
}

SimResult AssignmentExpression_simulate(AssignmentExpression *this, Simulator *simulator, SimState state, SimValue **out_value) {
    SimValue *value = NULL;
    SimResult result = Expression_simulate(this->expression, simulator, state, &value);
    if (!SimResult_isSuccess(result)) {
        goto cleanup;
    }

    result = SimFrame_setValue(state.frame, this->member, value);
    if (!SimResult_isSuccess(result)) {
        goto cleanup;
    }

    *out_value = value;

    cleanup:;
    return result;
}

SimResult LiteralExpression_simulate(LiteralExpression *this, Simulator *simulator, SimState state, SimValue **out_value) {
    TypeInfo ti = Type_info(this->type);
    SimValue *value = Allocator_malloc(simulator->allocator, sizeof(SimValue) + ti.size);
    value->type = this->type;
    memcpy(value->data, this->data, ti.size);
    *out_value = value;
    return SimResult_OK;
}

SimResult PrintExpression_simulate(PrintExpression *this, Simulator *simulator, SimState state, SimValue **out_value) {
    SimResult result = SimResult_OK;

    SimValue *value = NULL;

    result = Expression_simulate(this->expression, simulator, state, &value);

    if (value == NULL) {
        OutStream_puts(simulator->os_stdout, "(null)\n");
        goto cleanup;
    }

    TypeInfo ti = Type_info(value->type);

    char buf[32];

    OutStream_puts(simulator->os_stdout, "(");
    Type_repr(value->type, simulator->os_stdout);
    OutStream_puts(simulator->os_stdout, ")");

    for (Size i = 0; i < ti.size; i++) {
        sprintf(buf, "%02X", (unsigned char)value->data[i]);
        OutStream_puts(simulator->os_stdout, buf);
    }

    OutStream_puts(simulator->os_stdout, "\n");
    
    cleanup:;
    return result;
}

SimResult FrameExpression_simulate(FrameExpression *this, Simulator *simulator, SimState state, SimValue **out_value) {
    SimResult result = SimResult_OK;
    SimFrame frame;
    SimFrame_create(&frame, simulator, &this->frame, state.frame);

    result = Expression_simulate(this->expression, simulator, (SimState) {
        .frame = &frame
    }, out_value);

    if (!SimResult_isSuccess(result)) {
        goto cleanup;
    }

    cleanup:; 
    SimFrame_destroy(&frame, simulator);
    return SimResult_OK;
}

SimResult SequenceExpression_simulate(SequenceExpression *this, Simulator *simulator, SimState state, SimValue **out_value) {
    SimResult result = SimResult_OK;

    SimValue *retVal = NULL;

    for (Size i = 0; i < this->size; i++) {
        result = Expression_simulate(this->items[i], simulator, state, &retVal);
        if (!SimResult_isSuccess(result)) {
            goto cleanup;
        }
    }

    *out_value = retVal;

    cleanup:;
    return result;
}



SimResult Expression_simulate(Expression this, Simulator *simulator, SimState state, SimValue **out_value) {
    switch (this.interface->id) {
        case EXPRESSION_FRAME:
            return FrameExpression_simulate(this.object, simulator, state, out_value);
        case EXPRESSION_LITERAL:
            return LiteralExpression_simulate(this.object, simulator, state, out_value);
        case EXPRESSION_GET:
            return GetExpression_simulate(this.object, simulator, state, out_value);
        case EXPRESSION_ASSIGNMENT:
            return AssignmentExpression_simulate(this.object, simulator, state, out_value);
        case EXPRESSION_PRINT:
            return PrintExpression_simulate(this.object, simulator, state, out_value);
        case EXPRESSION_SEQUENCE:
            return SequenceExpression_simulate(this.object, simulator, state, out_value);
        default:
            return SimResult_create(SIM_UNSUPPORTED_EXPRESSION, NULL, NULL);
    }
    return SimResult_create(SIM_ERROR, NULL, NULL);
}
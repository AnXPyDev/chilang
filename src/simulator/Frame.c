void SimFrame_create(SimFrame *this, Simulator *sim, MemberList *memberList, SimFrame *parent) {
    this->parent = parent;
    Map_create(&this->members, sim->allocator, sizeof(SimValue*));

    Member *end = Vector_end(&memberList->members);
    for (Member *m = Vector_begin(&memberList->members); m < end; m++) {
        TypeInfo ti = Type_info(m->type);
        if (!ti.valid) {
            continue;
        }
        SimValue **value = Map_add(&this->members, asBufferView(m));
        *value = NULL;
    }
}

void SimFrame_destroy(SimFrame *this, Simulator *sim) {
}

SimResult SimFrame_setValue(SimFrame *this, Member *member, SimValue *value) {

    SimValue **valuep = Map_get(&this->members, asBufferView(member));

    if (valuep == NULL) {
        if (this->parent != NULL) {
            return SimFrame_setValue(this->parent, member, value);
        }
        return SimResult_create(SIM_FRAME_MEMBER_NOT_FOUND, NULL, NULL);
    }

    *valuep = value;
    return SimResult_OK;
}

SimResult SimFrame_getValue(SimFrame *this, Member *member, SimValue **out_value) {
    SimValue **valuep = Map_get(&this->members, asBufferView(member));

    if (valuep == NULL) {
        if (this->parent != NULL) {
            return SimFrame_getValue(this->parent, member, out_value);
        }
        return SimResult_create(SIM_FRAME_MEMBER_NOT_FOUND, NULL, NULL);
    }

    *out_value = *valuep;
    return SimResult_OK;
}
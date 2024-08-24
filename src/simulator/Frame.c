typedef struct SimFrame {
    struct SimFrame *parent;
    Map *members;
} SimFrame;

typedef enum {
    SIM_FRAME_OK = 0,
    SIM_FRAME_MEMBER_NOT_FOUND
} ESimFrameResult;

void SimFrame_create(SimFrame *this, Simulator *sim, MemberList *memberList, SimFrame *parent) {
    this->parent = parent;
    Map_create(&this->values, sim->allocator, sizeof(SimValue*));

    Member *end = Vector_end(&memberList->members);
    for (Member *m = Vector_begin(&memberList->members); m < end; m++) {
        SimValue **value = Map_add(&this->values, asBufferView(m));
        *value = NULL;
    }
}

void SimFrame_destroy(SimFrame *this, Simulator *sim) {
}

SimResult SimFrame_setValue(SimFrame *this, Member *member, SimValue *value) {

    SimValue **valuep = Map_get(&this->values, asBufferView(member));

    if (value == NULL) {
        if (this->parent != NULL) {
            return SimFrame_setValue(this->parent, member, value);
        }
        return SIM_FRAME_MEMBER_NOT_FOUND;
    }

    *valuep = value;
    return SIM_OK;
}

SimResult SimFrame_getValue(SimFrame *this, Member *member, SimValue **out_value) {
    SimValue **valuep = Map_get(&this->values, asBufferView(member));

    if (valuep == NULL) {
        if (this->parent != NULL) {
            return SimFrame_getValue(this->parent, member, out_value);
        }
        return SIM_FRAME_MEMBER_NOT_FOUND;
    }

    *out_value = *valuep;
    return SIM_OK;
}
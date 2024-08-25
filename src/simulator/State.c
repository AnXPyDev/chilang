TYPE_DECL(struct, SimState);

struct SimState {
    SimFrame *frame;
};

const SimState SimState_NULL = {
    .frame = NULL
};
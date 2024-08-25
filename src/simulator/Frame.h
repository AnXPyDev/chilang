TYPE_DECL(struct, SimFrame);

struct SimFrame {
    SimFrame *parent;
    Map members;
};
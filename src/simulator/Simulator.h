TYPE_DECL(struct, Simulator);

struct Simulator {
    Allocator allocator;
    OutStream logStream;
    OutStream os_stdout;
    SimFrame globalFrame;
};
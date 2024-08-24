typedef struct {
    Allocator allocator;
    OutStream logStream;
    OutStream stdout;
    SimFrame globalFrame;
} Simulator;
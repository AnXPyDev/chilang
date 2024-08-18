typedef size_t Size;

#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define MIN2(a, b) ((a) < (b) ? (a) : (b))

#define ARRSIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define TEMP_LOCAL(T, expr) (((struct { T val; }) {.val = (expr)}).val)
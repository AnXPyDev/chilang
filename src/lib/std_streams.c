FileOutStream fos_stderr;
OutStream os_stderr;
FileOutStream fos_stdout;
OutStream os_stdout;
FileInStream fis_stdin;
InStream is_stdin;

void std_streams_init() {
    fos_stderr = FileOutStream_new(stderr);
    fos_stdout = FileOutStream_new(stdout);
    fis_stdin = FileInStream_new(stdin);

    os_stderr = FileOutStream_upcast(&fos_stderr);
    os_stdout = FileOutStream_upcast(&fos_stdout);
    is_stdin = FileInStream_upcast(&fis_stdin);
}
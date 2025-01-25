#ifndef SAY_IT_WITH_YOUR_CHESST_PRINTS_H
#define SAY_IT_WITH_YOUR_CHESST_PRINTS_H
#include <stdbool.h>

struct RedirectedStdout {
    int saved_stdout;
    int pipefd[2];
};

struct RedirectedStdout redirect_stdout_to_pipe();
bool assert_last_printed_line_is(struct RedirectedStdout redirectedStdout, const char* expected);

#endif //SAY_IT_WITH_YOUR_CHESST_PRINTS_H

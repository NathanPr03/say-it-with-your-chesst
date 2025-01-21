#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "prints.h"

struct RedirectedStdout redirect_stdout_to_pipe() {
    struct RedirectedStdout result;
    pipe(result.pipefd);
    result.saved_stdout = dup(STDOUT_FILENO);
    dup2(result.pipefd[1], STDOUT_FILENO);
    close(result.pipefd[1]);

    return result;
}

// Utility function to check if the last printed line is equal to the expected string
// Must first call redirect_stdout_to_pipe to redirect STDOUT to a captureable pipe
bool assert_last_printed_line_is(struct RedirectedStdout redirectedStdout, const char* expected) {
    fflush(stdout);
    dup2(redirectedStdout.saved_stdout, STDOUT_FILENO);
    close(redirectedStdout.saved_stdout);

    char buffer[4096];
    ssize_t count = read(redirectedStdout.pipefd[0], buffer, sizeof(buffer) - 1);
    close(redirectedStdout.pipefd[0]);

    if (count <= 0) {
        return false;
    }

    buffer[count] = '\0';

    // Tokenize over line breaks to isolate the last non-empty line
    char *line = strtok(buffer, "\r\n");
    char *last_line = line;

    while ((line = strtok(NULL, "\r\n")) != NULL) {
        // Each call to strtok moves us to the next line.
        last_line = line;
    }

    // If there's no output line at all, fail.
    if (!last_line) {
        return false;
    }

    // Compare that final extracted line to the expected string.
    return strcmp(last_line, expected) == 0;
}
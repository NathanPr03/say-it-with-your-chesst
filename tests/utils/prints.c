#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "prints.h"

struct RedirectedStdout redirect_stdout_to_pipe() {
    struct RedirectedStdout result;
    pipe(result.pipefd);
    result.saved_stdout = dup(STDOUT_FILENO);
    dup2(result.pipefd[1], STDOUT_FILENO);
    close(result.pipefd[1]);

    return result;
}

// Structure to hold the last line state
struct LastLineState {
    char last_chunk[16384];
    char last_line[16384];
    ssize_t total_read;
    bool found_line;
};

// Strip trailing newline characters from a string
static void strip_trailing_newlines(char* str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

// Process a single chunk of output and update the last line state
static void process_output_chunk(const char* chunk, ssize_t chunk_size, struct LastLineState* state) {
    if (chunk_size <= 0) return;

    // Keep track of the last chunk for handling incomplete lines
    strncpy(state->last_chunk, chunk, sizeof(state->last_chunk) - 1);
    state->last_chunk[sizeof(state->last_chunk) - 1] = '\0';

    // Create a copy for tokenization
    char* chunk_copy = strdup(chunk);
    if (!chunk_copy) return;

    // Process lines in this chunk
    char* line = strtok(chunk_copy, "\r\n");
    char* current_last_line = NULL;

    while (line != NULL) {
        current_last_line = line;
        line = strtok(NULL, "\r\n");
    }

    // If we found any lines in this chunk, update our overall last line
    if (current_last_line) {
        strncpy(state->last_line, current_last_line, sizeof(state->last_line) - 1);
        state->last_line[sizeof(state->last_line) - 1] = '\0';
        strip_trailing_newlines(state->last_line);  // Strip any trailing newlines
        state->found_line = true;
    }

    free(chunk_copy);
}

// Process the final chunk to handle any content after the last newline
static void process_final_chunk(struct LastLineState* state) {
    char* last_chunk_copy = strdup(state->last_chunk);
    if (!last_chunk_copy) return;

    char* line = strtok(last_chunk_copy, "\r\n");
    char* final_line = NULL;

    // Find the last line in the final chunk
    while (line != NULL) {
        final_line = line;
        line = strtok(NULL, "\r\n");
    }

    // If there was no newline at the end, the entire last chunk after the last newline
    // is our final line
    if (final_line) {
        size_t final_line_pos = final_line - last_chunk_copy;
        if (final_line_pos < strlen(state->last_chunk)) {
            strncpy(state->last_line, state->last_chunk + final_line_pos,
                    sizeof(state->last_line) - 1);
            state->last_line[sizeof(state->last_line) - 1] = '\0';
            strip_trailing_newlines(state->last_line);  // Strip any trailing newlines
            state->found_line = true;
        }
    }

    free(last_chunk_copy);
}

// Utility function to check if the last printed line is equal to the expected string
// Must first call redirect_stdout_to_pipe to redirect STDOUT to a captureable pipe
bool assert_last_printed_line_is(struct RedirectedStdout redirectedStdout, const char* expected) {
    fflush(stdout);
    dup2(redirectedStdout.saved_stdout, STDOUT_FILENO);
    close(redirectedStdout.saved_stdout);

    // Initialize state
    struct LastLineState state = {
            .last_chunk = {0},
            .last_line = {0},
            .total_read = 0,
            .found_line = false
    };

    char buffer[16384];
    bool found_more = true;

    // Read and process chunks until we're done
    while (found_more) {
        ssize_t count = read(redirectedStdout.pipefd[0], buffer, sizeof(buffer) - 1);

        if (count <= 0) {
            found_more = false;
            if (state.total_read == 0) {
                close(redirectedStdout.pipefd[0]);
                return false;
            }
            break;
        }

        state.total_read += count;
        buffer[count] = '\0';

        process_output_chunk(buffer, count, &state);

        // If we didn't fill the buffer, we're done
        if (count < sizeof(buffer) - 1) {
            found_more = false;
        }
    }

    // Handle any content after the last newline
    process_final_chunk(&state);

    close(redirectedStdout.pipefd[0]);

    // If we never found a line, fail
    if (!state.found_line) {
        return false;
    }

    // Compare the final extracted line to the expected string
    return strcmp(state.last_line, expected) == 0;
}

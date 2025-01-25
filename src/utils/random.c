#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include "random.h"

// Generates cryptographically secure "true" random numbers
unsigned int random_number(unsigned int upper_limit) {
    int fd = open("/dev/random", O_RDONLY);
    if (fd == -1) return 1;

    unsigned int randomValue;
    if (read(fd, &randomValue, sizeof(randomValue)) < 0) {
        close(fd);
        exit(1);
    }

    close(fd);

    return randomValue % (upper_limit + 1);
}

// Combine two 32-bit random numbers to create a 64-bit hash value, used for zobrist hashing
uint64_t generate_64_bit_hash_value() {
    uint64_t value = 0;
    value |= ((uint64_t)random_number(UINT_MAX)) << 32;
    value |= random_number(UINT_MAX);
    return value;
}
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "random.h"

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
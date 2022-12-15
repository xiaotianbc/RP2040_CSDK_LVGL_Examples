//
// Created by xiaotian on 2022/12/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <errno.h>
__attribute__((weak)) int _close(int fd) {
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

__attribute__((weak)) int _lseek(int fd, int ptr, int dir) {
    (void) fd;
    (void) ptr;
    (void) dir;

    errno = EBADF;
    return -1;
}
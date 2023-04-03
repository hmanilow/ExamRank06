#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <netinet/in.h>

int next_id = 0, max = 0, id[65536];
fd_set rdRead, rdWrite, active;
char str[42 * 4096], rdBuf[42 * 4096], wrBuf[42 * 4097];

void error(char *msg)
{
    write(2, msg, strlen(msg));
    exit(1);
}

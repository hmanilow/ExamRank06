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

void send_all(int cs)
{
    for (int i = 0; i <= max; i++)
        if (i!=cs && FD_ISSET(i, &rdWrite))
            send(i, &wrBuf, strlen(wrBuf), 0);
}


int main(int argc, char **argv)
{
    if (argc < 2)
        error("sfsdf\n");
    FD_ZERO(&active);

    bzero(id, sizeof(id));
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(2130706433);
    addr.sin_port = htons(atoi(argv[1]));
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1 || bind(sockfd, (const struct sockaddr *)&addr, len) !=0
            || listen(sockfd, 10) != 0)
        error("fatal\n");
    max = sockfd;
    FD_SET(sockfd, &active);
    while(1)
    {
        rdRead = rdWrite = active;
	if(select(max+1,&rdRead,&rdWrite,NULL,NULL)<0)
		continue;

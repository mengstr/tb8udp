#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PORT 2288

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen = 0;

    // Default the line-by-line delay to 100ms, but override
    // to the whole-second value at command line if specified
    unsigned long lineDly = 100UL;
    if (argc == 2) lineDly = atol(argv[1]) * 1000UL;
    struct timespec SLEEP_CHAR = {.tv_sec = 0, .tv_nsec = 2UL * 1000000UL};
    struct timespec SLEEP_LINE = {.tv_sec = lineDly / 1000UL, .tv_nsec = (lineDly * 1000000UL) % 1000000000UL};
    struct timespec SLEEP_END = {.tv_sec = 0, .tv_nsec = 250UL * 1000000UL};

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("\n Error : Connect Failed \n");
        exit(1);
    }

    while ((linelen = getline(&line, &linecap, stdin)) > 0) {
        for (int i = 0; i < linelen; i++) {
            if (line[i] == 10) line[i] = 13;
            sendto(sockfd, &(line[i]), 1, 0, (struct sockaddr *)NULL, sizeof(servaddr));
            if (line[i] == 13)
                nanosleep(&SLEEP_LINE, NULL);
            else
                nanosleep(&SLEEP_CHAR, NULL);
        }
    }
    nanosleep(&SLEEP_END, NULL);

    free(line);
    close(sockfd);
    exit(0);
}

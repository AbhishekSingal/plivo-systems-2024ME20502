#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>

#define MAX_SEQ 65536

int main(void) {
    int in_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in in_addr = {0};
    in_addr.sin_family = AF_INET;
    in_addr.sin_port = htons(47002);
    in_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(in_fd, (struct sockaddr *)&in_addr, sizeof in_addr) < 0) {
        perror("bind 47002");
        return 1;
    }

    int out_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in player = {0};
    player.sin_family = AF_INET;
    player.sin_port = htons(47020);
    player.sin_addr.s_addr = inet_addr("127.0.0.1");

    unsigned char buf[2048];
    uint32_t last_seen_seq = 0xFFFFFFFF;

    for (;;) {
        ssize_t n = recvfrom(in_fd, buf, sizeof buf, 0, NULL, NULL);
        if (n < 4) continue;

        uint32_t seq = ((uint32_t)buf[0] << 24) |
                       ((uint32_t)buf[1] << 16) |
                       ((uint32_t)buf[2] << 8)  |
                       (uint32_t)buf[3];

        if (seq != last_seen_seq) {
            sendto(out_fd, buf, (size_t)n, 0, (struct sockaddr *)&player, sizeof player);
            last_seen_seq = seq;
        }
    }
    return 0;
}

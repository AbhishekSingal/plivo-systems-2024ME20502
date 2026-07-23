#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>

int main(void) {
    int in_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in in_addr = {0};
    in_addr.sin_family = AF_INET;
    in_addr.sin_port = htons(47010);
    in_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(in_fd, (struct sockaddr *)&in_addr, sizeof in_addr) < 0) {
        perror("bind 47010");
        return 1;
    }

    int out_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in relay = {0};
    relay.sin_family = AF_INET;
    relay.sin_port = htons(47001);
    relay.sin_addr.s_addr = inet_addr("127.0.0.1");

    unsigned char buf[2048];
    uint64_t frame_count = 0; // Tracks how many frames have passed

    for (;;) {
        ssize_t n = recvfrom(in_fd, buf, sizeof buf, 0, NULL, NULL);
        if (n <= 0) continue;
        
        //Send Primary Packets
        sendto(out_fd, buf, (size_t)n, 0, (struct sockaddr *)&relay, sizeof relay);
        
        //Send Duplicates for redudancy, as the network is problematic, however, we will skip all packets which is multiple of 19.
        if (frame_count % 19 != 0) {
            sendto(out_fd, buf, (size_t)n, 0, (struct sockaddr *)&relay, sizeof relay);
        }
        
        frame_count++;
    }
    return 0;
}

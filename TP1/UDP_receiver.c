#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 5000

int main() {
    int soc = socket(AF_INET, SOCK_DGRAM, 0);
    if (soc < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr, client;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(soc, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }

    printf("Serveur UDP en écoute sur le port %d...\n", PORT);

    char buffer[1024];
    socklen_t len = sizeof(client);

    while (1) {
        int n = recvfrom(soc, buffer, sizeof(buffer)-1, 0,
                         (struct sockaddr *)&client, &len);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }
        buffer[n] = '\0';
        printf("Message reçu : %s\n", buffer);
    }

    close(soc);
    return 0;
}
 
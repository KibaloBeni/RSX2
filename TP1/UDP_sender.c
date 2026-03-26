#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 5000
#define SERVER_IP "127.0.0.1"



int main(int argc, char *argv[]){

    int soc = socket(AF_INET, SOCK_DGRAM, 0);
    if(soc < 0){
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);

    char buffer[1024];

    printf("Entrez le message à envoyer : ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Erreur de lecture.\n");
        return EXIT_FAILURE;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    int n = sendto(soc, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&addr, sizeof(addr));
    if(n < 0){
        perror("sendto");
        return EXIT_FAILURE;
    }

    printf("Message envoye : %s\n", buffer);
    close(soc);
    return EXIT_SUCCESS;
    
}
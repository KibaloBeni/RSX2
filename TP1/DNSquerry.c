#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>

#define DNS_PORT 53
#define BUFFER_SIZE 512

// Structure pour l'en-tête DNS
struct dns_header {
    uint16_t id;          // Identifiant
    uint16_t flags;       // Paramètres
    uint16_t qdcount;     // Nombre de questions
    uint16_t ancount;     // Nombre de réponses
    uint16_t nscount;     // Nombre d'autorités
    uint16_t arcount;     // Nombre d'additionnels
};

// Fonction pour encoder un nom de domaine au format DNS
int encode_dns_name(const char *hostname, unsigned char *dns_name) {
    int i = 0, j = 0, len = 0;
    
    // Copie du nom d'hôte
    char host_copy[256];
    strncpy(host_copy, hostname, 255);
    host_copy[255] = '\0';
    
    // Parcours et encodage
    char *token = strtok(host_copy, ".");
    while (token != NULL) {
        len = strlen(token);
        dns_name[j++] = len;  // Longueur du label
        
        for (i = 0; i < len; i++) {
            dns_name[j++] = token[i];
        }
        
        token = strtok(NULL, ".");
    }
    
    dns_name[j++] = 0;  // Fin de la chaîne DNS
    return j;
}

// Fonction pour créer une requête DNS (Question 2)
int create_dns_query(const char *hostname, unsigned char *buffer) {
    struct dns_header *header;
    unsigned char *qname;
    uint16_t *qtype, *qclass;
    int qname_len;
    
    // En-tête DNS
    header = (struct dns_header *)buffer;
    
    // Identifiant aléatoire
    srand(time(NULL));
    header->id = htons(rand() % 65536);
    
    // Flags: requête standard récursive (RD=1)
    // 0000 0001 0000 0000 = 0x0100
    header->flags = htons(0x0100);
    
    header->qdcount = htons(1);  // 1 question
    header->ancount = 0;
    header->nscount = 0;
    header->arcount = 0;
    
    // Question
    qname = buffer + sizeof(struct dns_header);
    qname_len = encode_dns_name(hostname, qname);
    
    qtype = (uint16_t *)(qname + qname_len);
    *qtype = htons(1);  // Type A (adresse IPv4)
    
    qclass = (uint16_t *)(qname + qname_len + 2);
    *qclass = htons(1);  // Classe IN (Internet)
    
    return sizeof(struct dns_header) + qname_len + 4;
}

// Fonction pour afficher un buffer en hexadécimal (Question 3)
void print_hex_dump(unsigned char *buffer, int size) {
    printf("\n=== Datagramme DNS reçu (format hexadécimal) ===\n");
    for (int i = 0; i < size; i++) {
        if (i % 16 == 0) {
            if (i != 0) printf("\n");
            printf("%04x: ", i);
        } else if (i % 8 == 0) {
            printf(" ");
        }
        printf("%02x ", buffer[i]);
    }
    printf("\n\n");
}

// Fonction pour décoder un nom DNS avec gestion des pointeurs
int decode_dns_name(unsigned char *buffer, int offset, char *output, int buffer_size) {
    int i = 0, jumped = 0, jumps = 0;
    int original_offset = offset;
    int count;
    
    output[0] = '\0';
    
    while (buffer[offset] != 0) {
        // Vérification des pointeurs (compression)
        if ((buffer[offset] & 0xC0) == 0xC0) {
            // C'est un pointeur
            if (!jumped) {
                original_offset = offset + 2;
            }
            
            // Calcul de l'offset du pointeur
            int pointer = ((buffer[offset] & 0x3F) << 8) | buffer[offset + 1];
            offset = pointer;
            jumped = 1;
            
            jumps++;
            if (jumps > 10) {  // Protection contre les boucles infinies
                return original_offset;
            }
        } else {
            // C'est une longueur
            count = buffer[offset++];
            
            if (i != 0) {
                output[i++] = '.';
            }
            
            for (int j = 0; j < count; j++) {
                output[i++] = buffer[offset++];
            }
        }
    }
    
    output[i] = '\0';
    
    if (!jumped) {
        original_offset = offset + 1;
    }
    
    return original_offset;
}

// Fonction pour analyser la réponse DNS (Question 4)
void parse_dns_response(unsigned char *buffer, int size) {
    struct dns_header *header = (struct dns_header *)buffer;
    
    printf("\n=== ANALYSE DE LA RÉPONSE DNS ===\n\n");
    
    // Analyse de l'en-tête
    printf("-- En-tête --\n");
    printf("Identifiant: 0x%04x\n", ntohs(header->id));
    
    uint16_t flags = ntohs(header->flags);
    printf("Flags: 0x%04x\n", flags);
    printf("  QR: %d (%s)\n", (flags >> 15) & 1, (flags >> 15) & 1 ? "Réponse" : "Requête");
    printf("  OPCODE: %d\n", (flags >> 11) & 0x0F);
    printf("  AA: %d (Authoritative Answer: %s)\n", (flags >> 10) & 1, (flags >> 10) & 1 ? "Oui" : "Non");
    printf("  TC: %d (Truncated: %s)\n", (flags >> 9) & 1, (flags >> 9) & 1 ? "Oui" : "Non");
    printf("  RD: %d (Recursion Desired: %s)\n", (flags >> 8) & 1, (flags >> 8) & 1 ? "Oui" : "Non");
    printf("  RA: %d (Recursion Available: %s)\n", (flags >> 7) & 1, (flags >> 7) & 1 ? "Oui" : "Non");
    printf("  RCODE: %d ", flags & 0x0F);
    
    switch (flags & 0x0F) {
        case 0: printf("(Pas d'erreur)\n"); break;
        case 1: printf("(Erreur de format)\n"); break;
        case 2: printf("(Problème serveur)\n"); break;
        case 3: printf("(Nom de domaine non trouvé)\n"); break;
        case 4: printf("(Requête non supportée)\n"); break;
        case 5: printf("(Refus de répondre)\n"); break;
        default: printf("(Code inconnu)\n");
    }
    
    printf("Questions: %d\n", ntohs(header->qdcount));
    printf("Réponses: %d\n", ntohs(header->ancount));
    printf("Autorités: %d\n", ntohs(header->nscount));
    printf("Additionnels: %d\n", ntohs(header->arcount));
    
    int offset = sizeof(struct dns_header);
    char name[256];
    
    // Analyse de la question
    printf("\n-- Question --\n");
    offset = decode_dns_name(buffer, offset, name, size);
    printf("Nom: %s\n", name);
    
    uint16_t qtype = ntohs(*(uint16_t *)(buffer + offset));
    offset += 2;
    uint16_t qclass = ntohs(*(uint16_t *)(buffer + offset));
    offset += 2;
    
    printf("Type: %d (%s)\n", qtype, qtype == 1 ? "A (IPv4)" : "Autre");
    printf("Classe: %d (%s)\n", qclass, qclass == 1 ? "IN (Internet)" : "Autre");
    
    // Analyse des réponses
    int num_answers = ntohs(header->ancount);
    for (int i = 0; i < num_answers; i++) {
        printf("\n-- Réponse %d --\n", i + 1);
        
        offset = decode_dns_name(buffer, offset, name, size);
        printf("Nom: %s\n", name);
        
        uint16_t type = ntohs(*(uint16_t *)(buffer + offset));
        offset += 2;
        
        uint16_t class = ntohs(*(uint16_t *)(buffer + offset));
        offset += 2;
        
        uint32_t ttl = ntohl(*(uint32_t *)(buffer + offset));
        offset += 4;
        
        uint16_t rdlength = ntohs(*(uint16_t *)(buffer + offset));
        offset += 2;
        
        printf("Type: %d ", type);
        if (type == 1) printf("(A - IPv4)\n");
        else if (type == 5) printf("(CNAME - Alias)\n");
        else if (type == 2) printf("(NS - Name Server)\n");
        else printf("(Autre)\n");
        
        printf("Classe: %d (%s)\n", class, class == 1 ? "IN" : "Autre");
        printf("TTL: %u secondes\n", ttl);
        printf("RDLength: %d octets\n", rdlength);
        
        if (type == 1 && rdlength == 4) {
            // Adresse IPv4
            printf("Adresse IPv4: %d.%d.%d.%d\n",
                   buffer[offset], buffer[offset + 1],
                   buffer[offset + 2], buffer[offset + 3]);
            printf("\n*** RÉPONSE: L'ADRESSE IPv4 EST %d.%d.%d.%d ***\n",
                   buffer[offset], buffer[offset + 1],
                   buffer[offset + 2], buffer[offset + 3]);
        } else if (type == 5) {
            // CNAME
            char cname[256];
            decode_dns_name(buffer, offset, cname, size);
            printf("Nom canonique: %s\n", cname);
        }
        
        offset += rdlength;
    }
    
    // Analyse des autorités
    int num_authorities = ntohs(header->nscount);
    for (int i = 0; i < num_authorities; i++) {
        printf("\n-- Autorité %d --\n", i + 1);
        
        offset = decode_dns_name(buffer, offset, name, size);
        printf("Nom: %s\n", name);
        
        uint16_t type = ntohs(*(uint16_t *)(buffer + offset));
        offset += 2;
        
        uint16_t class = ntohs(*(uint16_t *)(buffer + offset));
        offset += 2;
        
        uint32_t ttl = ntohl(*(uint32_t *)(buffer + offset));
        offset += 4;
        
        uint16_t rdlength = ntohs(*(uint16_t *)(buffer + offset));
        offset += 2;
        
        printf("Type: %d ", type);
        if (type == 2) {
            printf("(NS - Name Server)\n");
            char ns_name[256];
            decode_dns_name(buffer, offset, ns_name, size);
            printf("Serveur de noms: %s\n", ns_name);
        } else {
            printf("(Autre)\n");
        }
        
        offset += rdlength;
    }
    
    // Analyse des additionnels
    int num_additionals = ntohs(header->arcount);
    for (int i = 0; i < num_additionals; i++) {
        printf("\n-- Additionnel %d --\n", i + 1);
        
        offset = decode_dns_name(buffer, offset, name, size);
        printf("Nom: %s\n", name);
        
        uint16_t type = ntohs(*(uint16_t *)(buffer + offset));
        offset += 2;
        
        uint16_t class = ntohs(*(uint16_t *)(buffer + offset));
        offset += 2;
        
        uint32_t ttl = ntohl(*(uint32_t *)(buffer + offset));
        offset += 4;
        
        uint16_t rdlength = ntohs(*(uint16_t *)(buffer + offset));
        offset += 2;
        
        printf("Type: %d ", type);
        if (type == 1 && rdlength == 4) {
            printf("(A - IPv4)\n");
            printf("Adresse IPv4: %d.%d.%d.%d\n",
                   buffer[offset], buffer[offset + 1],
                   buffer[offset + 2], buffer[offset + 3]);
        } else {
            printf("(Autre)\n");
        }
        
        offset += rdlength;
    }
}

// Fonction pour extraire l'adresse IPv4 de la réponse
int extract_ipv4(unsigned char *buffer, int size, char *ip_address) {
    struct dns_header *header = (struct dns_header *)buffer;
    int num_answers = ntohs(header->ancount);
    
    int offset = sizeof(struct dns_header);
    
    // Sauter la question
    while (buffer[offset] != 0) {
        if ((buffer[offset] & 0xC0) == 0xC0) {
            offset += 2;
            break;
        }
        offset += buffer[offset] + 1;
    }
    if (buffer[offset] == 0) offset++;
    offset += 4;  // Sauter QTYPE et QCLASS
    
    // Parcourir les réponses
    for (int i = 0; i < num_answers; i++) {
        // Sauter le nom
        if ((buffer[offset] & 0xC0) == 0xC0) {
            offset += 2;
        } else {
            while (buffer[offset] != 0) {
                offset += buffer[offset] + 1;
            }
            offset++;
        }
        
        uint16_t type = ntohs(*(uint16_t *)(buffer + offset));
        offset += 2;
        offset += 2;  // Classe
        offset += 4;  // TTL
        
        uint16_t rdlength = ntohs(*(uint16_t *)(buffer + offset));
        offset += 2;
        
        if (type == 1 && rdlength == 4) {
            // Adresse IPv4 trouvée
            sprintf(ip_address, "%d.%d.%d.%d",
                    buffer[offset], buffer[offset + 1],
                    buffer[offset + 2], buffer[offset + 3]);
            return 1;
        }
        
        offset += rdlength;
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in dns_server;
    unsigned char query[BUFFER_SIZE], response[BUFFER_SIZE];
    int query_len, response_len;
    char *hostname;
    char *dns_ip = "1.1.1.1";  // DNS par défaut (Cloudflare)
    
    if (argc < 2) {
        printf("Usage: %s <hostname> [dns_server_ip]\n", argv[0]);
        printf("Exemple: %s www.google.com\n", argv[0]);
        printf("         %s www.google.com 8.8.8.8\n", argv[0]);
        exit(1);
    }
    
    hostname = argv[1];
    if (argc >= 3) {
        dns_ip = argv[2];
    }
    
    printf("Interrogation du serveur DNS %s pour le nom: %s\n", dns_ip, hostname);
    
    // Création de la socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Erreur création socket");
        exit(1);
    }
    
    // Configuration du timeout
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    // Configuration de l'adresse du serveur DNS
    memset(&dns_server, 0, sizeof(dns_server));
    dns_server.sin_family = AF_INET;
    dns_server.sin_port = htons(DNS_PORT);
    
    if (inet_pton(AF_INET, dns_ip, &dns_server.sin_addr) <= 0) {
        perror("Adresse DNS invalide");
        close(sockfd);
        exit(1);
    }
    
    // Création de la requête DNS (Question 2)
    query_len = create_dns_query(hostname, query);
    
    printf("\n=== Requête DNS créée (%d octets) ===\n", query_len);
    
    // Envoi de la requête
    int sent = sendto(sockfd, query, query_len, 0,
                      (struct sockaddr *)&dns_server, sizeof(dns_server));
    
    if (sent < 0) {
        perror("Erreur envoi");
        close(sockfd);
        exit(1);
    }
    
    printf("Requête envoyée au serveur DNS...\n");
    
    // Réception de la réponse (Question 3)
    response_len = recvfrom(sockfd, response, BUFFER_SIZE, 0, NULL, NULL);
    
    if (response_len < 0) {
        perror("Erreur réception (timeout?)");
        close(sockfd);
        exit(1);
    }
    
    printf("Réponse reçue (%d octets)\n", response_len);
    
    // Affichage hexadécimal (Question 3)
    print_hex_dump(response, response_len);
    
    // Analyse de la réponse (Question 4)
    parse_dns_response(response, response_len);
    
    // Extraction de l'adresse IPv4
    char ip_address[16];
    if (extract_ipv4(response, response_len, ip_address)) {
        printf("\n\n===========================================\n");
        printf("RÉSULTAT FINAL: %s -> %s\n", hostname, ip_address);
        printf("===========================================\n");
    } else {
        printf("\nAucune adresse IPv4 trouvée dans la réponse.\n");
    }
    
    close(sockfd);
    return 0;
}
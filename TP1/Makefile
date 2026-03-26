CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: udp_sender udp_receiver dns_query

udp_sender: udp_sender.c
	$(CC) $(CFLAGS) -o udp_sender udp_sender.c

udp_receiver: udp_receiver.c
	$(CC) $(CFLAGS) -o udp_receiver udp_receiver.c

dns_query: dns_query.c
	$(CC) $(CFLAGS) -o dns_query dns_query.c

clean:
	rm -f udp_sender udp_receiver dns_query

test_udp:
	@echo "Pour tester UDP:"
	@echo "1. Terminal 1: ./udp_receiver"
	@echo "2. Terminal 2: ./udp_sender 127.0.0.1 'Bonjour UDP'"

test_dns:
	@echo "Pour tester DNS:"
	@echo "./dns_query www.google.com"
	@echo "./dns_query www.univ-lille.fr 8.8.8.8"

.PHONY: all clean test_udp test_dns
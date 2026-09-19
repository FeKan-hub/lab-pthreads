CC = gcc
CFLAGS = -O2 -Wall -pthread
TARGETS = contacorrente_seq contacorrente_f1 contacorrente_f2 monte_carlo_pi soma_vetores matriz_vetor

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

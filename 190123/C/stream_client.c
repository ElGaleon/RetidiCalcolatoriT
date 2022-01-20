/* stream_client.c
 * Cliente invia numero targa e riceve
 * nomi e immagini riguardo il veicolo
 */

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LINE_LENGTH 256
#define DIMTARGA 7

int main(int argc, char* argv[]) {
    int port, nread, sd, nwrite;
    char targa[DIMTARGA+1], buffChar;
    struct hostent* host;
    struct sockaddr_in servaddr;

    /* CONTROLLO ARGOMENTI ---------------------------------- */
    if(argc != 3) {
        printf("Error:%s serverAddress serverPort\n", argv[0]);
        exit(1);
    }

    /* INIZIALIZZAZIONE INDIRIZZO SERVER -------------------------- */
    memset((char*)&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    host = gethostbyname(argv[1]);
    if(host == NULL) {
        printf("%s not found in /etc/hosts\n", argv[1]);
        exit(1);
    }

    nread = 0;
    while(argv[2][nread] != '\0') {
        if((argv[2][nread] < '0') || (argv[2][nread] > '9')) {
            printf("Secondo argomento non intero\n");
            exit(2);
        }
        nread++;
    }

    port = atoi(argv[2]);
    if(port < 1024 || port > 65535) {
        printf("Porta scorretta...");
        exit(2);
    }

    servaddr.sin_addr.s_addr = ((struct in_addr*)(host->h_addr))->s_addr;
    servaddr.sin_port = htons(port);

    /* CREAZIONE SOCKET ------------------------------------ */
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd < 0) {
        perror("apertura socket");
        exit(3);
    }
    printf("Client: creata la socket sd=%d\n", sd);

    /* Operazione di BIND implicita nella connect */
    if(connect(sd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr)) < 0) {
        perror("connect");
        exit(3);
    }
    printf("Client: connect ok\n");

    /* CORPO DEL CLIENT:
	 ciclo di accettazione di richieste da utente ------- */
    printf("Inserire numero di targa secondo questo formato [AA123BB], EOF per terminare: ");
    while(scanf("%s", targa) == 1 && strlen(targa) == DIMTARGA) {
        printf("Invio il numero della targa: %s\n", targa);
        nwrite = write(sd, targa, strlen(targa));

        // Lettura risposta dal server
        while( read(sd, &buffChar, sizeof(buffChar)) >0 ) {
            printf("Immagine da scaricare: %s\n", buffChar);
        }
        /*
        read(sd, &buffChar, sizeof(char));
        if(buffChar == 'S') {
            printf("Ricevo e stampo i file nel direttorio remoto:\n");
            while((nread = read(sd, &buffChar, sizeof(char))) > 0) {
                if(buffChar == '#') {
                    break;
                } else if(buffChar == '\0') {
                    write(1, &buffChar, sizeof(char));
                    printf("\n");
                } else {
                    write(1, &buffChar, sizeof(char));
                }
            }
        } else {
            printf("directory non presente sul server\n");
        }
        */
        printf("Inserire numero di targa secondo questo formato [AA123BB], EOF per terminare: ");
    }
    /* Chiusura socket in ricezione */
    close(sd);
    printf("\nClient: termino...\n");
    exit(0);
}
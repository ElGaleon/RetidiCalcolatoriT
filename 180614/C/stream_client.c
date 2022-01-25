/* stream_client.c
 * Cliente invia nome dir e riceve
 * nomi file in dir sec lvl
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

int main(int argc, char* argv[]) {
    int port, nread, sd, nwrite, fnLength, dimResult;
    char directoryName[LINE_LENGTH], buffChar;
    struct hostent* host;
    struct sockaddr_in servaddr;
    char fileName[LINE_LENGTH];

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
    printf("Inserire nome del direttorio, EOF per terminare: ");
    while(scanf("%s", directoryName) == 1) {
        printf("Invio il nome del direttorio: %s\n", directoryName);
        nwrite = write(sd, directoryName, sizeof(directoryName));
        //printf("DBG scritti al server %d char\n", nwrite);

        // Lettura risposta dal server
        dimResult = 0;
        // Lettura dimensione risultato
        read(sd, &dimResult, sizeof(int));
        if (dimResult == 0) {
            printf("Nessun file trovato!\n");
        } else {
        printf("Numero file trovati: %i\n", dimResult);

        for (int i = 0; i < dimResult; i++)
        {
            // Lettura dimensione fileName
                if (read(sd, &fnLength, sizeof(fnLength))<0) {
                    perror("Errore read fnLength");
                    continue;
                }
                //printf("Lunghezza fileName: %i caratteri\n", fnLength);

            // Lettura nome file
                if (read(sd, fileName, sizeof(char) *fnLength) < 0) {
                    perror("Errore read filename");
                    continue;
                }
                printf("%s\n", fileName);             
        }
        }
        printf("Nome del direttorio, EOF per terminare: ");
    }
    /* Chiusura socket in ricezione */
    close(sd);
    printf("\nClient: termino...\n");
    exit(0);
}
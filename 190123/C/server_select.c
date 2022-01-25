/**
  * server_select.c
  *  Il server discrimina due servizi con la select:
  *    + aggiornamento patente (UDP)
  *    + download immagini (TCP)
  **/

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <regex.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LINE_LENGTH 128
#define WORD_LENGTH 128
#define DIMTARGA 7
#define DIMPATENTE 5
#define N 50
#define max(a, b)  ((a > b) ? a : b)


/********************************************************/
void gestore(int signo) {
    int stato;
    printf("esecuzione gestore di SIGCHLD\n");
    wait(&stato);
}
/********************************************************/
typedef struct
{
    char targa[DIMTARGA+1];
    char patente[DIMPATENTE+1];
} ReqUDP;

typedef struct 
{
    char *targa;
    char *patente;
    char *veicolo;
    char *folder;
} Riga;

static Riga tab[N];    // tabella del server
static int inizializzato = 0;

void stampa_struttura() {
    printf("TARGA VEICOLO\tPATENTE DI GUIDA\tVEICOLO\tFOLDER IMMAGINI\n");
    for (int i = 0; i < N; i++) 
    {
        printf("%s\t%s\t%s\t%s\n", tab[i].targa, tab[i].patente, tab[i].veicolo, tab[i].folder);
    }
    
}

void inizializzazione() {
    if (inizializzato == 1) return;
    

    for (int i = 0; i < N; i++) {
        tab[i].targa = malloc(strlen("L") + 1);
        strcpy(tab[i].targa, "L");
        tab[i].patente = malloc(strlen("0") + 1);
        strcpy(tab[i].patente, "0");
        tab[i].veicolo = malloc(strlen("L") + 1);
        strcpy(tab[i].veicolo, "L");
        tab[i].folder = malloc(strlen("L") + 1);
        strcpy(tab[i].folder, "L");
    }

    // Riempimento valori fittizi
    tab[0].targa = (char *) malloc(DIMTARGA);
    strcpy(tab[0].targa, "AN745NL");
    tab[0].patente = (char *) malloc(DIMPATENTE);
    strcpy(tab[0].patente, "00003");
    tab[0].veicolo = (char *) malloc(strlen("auto") + 1);
    strcpy(tab[0].veicolo, "auto");

    tab[0].folder = (char *) malloc(strlen(tab[0].targa) + 5);
    strcpy(tab[0].folder, tab[0].targa);
    strcat(tab[0].folder, "_img");
    mkdir(tab[0].folder, 0777);

    tab[1].targa = (char *) malloc(DIMTARGA);
    strcpy(tab[1].targa, "FE457GF");
    tab[1].patente = (char *) malloc(DIMPATENTE);
    strcpy(tab[1].patente, "50006");
    tab[1].veicolo = (char *) malloc(strlen("auto") + 1);
    strcpy(tab[1].veicolo, "camper");
    tab[1].folder = (char *) malloc(strlen(tab[0].targa) + 5);
    strcpy(tab[1].folder, tab[1].targa);
    strcat(tab[1].folder, "_img");
    mkdir(tab[1].folder, 0777);


    inizializzato = 1;
    printf("Inizializzazione struttura dati terminata!\n");
    stampa_struttura();
    return;
}

int main(int argc, char** argv) {
    struct sockaddr_in cliaddr, servaddr;
    struct hostent *hostTcp, *hostUdp;
    int port, listen_sd, conn_sd, udp_sd, nread, maxfdp1, len;
    const int on = 1;
    struct dirent *de;
    fd_set rset;
    FILE *fileToSend;
    ReqUDP req;
    int dim, fts;
    char path[256], filePath[256];
    int fd_sorg_udp, fd_temp_udp, count_letters;
    char read_char, err[128], word_buffer[WORD_LENGTH], targa[DIMTARGA+1];
    char charBuff[256], newDir[LINE_LENGTH], fileNameTemp[LINE_LENGTH], fileName[LINE_LENGTH], dir[LINE_LENGTH];
    DIR *dir1, *dir2, *dir3;
    struct dirent *dd1, *dd2;

    /* CONTROLLO ARGOMENTI ---------------------------------- */
    if(argc != 2) {
        printf("Error: %s port \n", argv[0]);
        exit(1);
    } else {
        nread = 0;
        while(argv[1][nread] != '\0') {
            if((argv[1][nread] < '0') || (argv[1][nread] > '9')) {
                printf("Secondo argomento non intero\n");
                exit(2);
            }
            nread++;
        }
        port = atoi(argv[1]);
        if(port < 1024 || port > 65535) {
            printf("Porta scorretta...");
            exit(2);
        }
    }

    /* INIZIALIZZAZIONE INDIRIZZO SERVER ----------------------------------------- */
    memset((char*)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    /* INIZIALIZZAZIONE STRUTTURA DATI ----------------------------------------- */
    inizializzazione();

    /* CREAZIONE E SETTAGGI SOCKET TCP --------------------------------------- */
    listen_sd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sd < 0) {
        perror("creazione socket ");
        exit(3);
    }
    printf("Server: creata la socket d'ascolto per le richieste di ordinamento, fd=%d\n", listen_sd);

    if(setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        perror("set opzioni socket d'ascolto");
        exit(3);
    }
    printf("Server: set opzioni socket d'ascolto ok\n");

    if(bind(listen_sd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind socket d'ascolto");
        exit(3);
    }
    printf("Server: bind socket d'ascolto ok\n");

    if(listen(listen_sd, 5) < 0) {
        perror("listen");
        exit(3);
    }
    printf("Server: listen ok\n");

    /* CREAZIONE E SETTAGGI SOCKET UDP --------------------------------------- */
    udp_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(udp_sd < 0) {
        perror("apertura socket UDP");
        exit(4);
    }
    printf("Creata la socket UDP, fd=%d\n", udp_sd);

    if(setsockopt(udp_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        perror("set opzioni socket UDP");
        exit(4);
    }
    printf("Set opzioni socket UDP ok\n");

    if(bind(udp_sd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind socket UDP");
        exit(4);
    }
    printf("Bind socket UDP ok\n");

    signal(SIGCHLD, gestore);

    /* PULIZIA E SETTAGGIO MASCHERA DEI FILE DESCRIPTOR ------------------------- */
    FD_ZERO(&rset);
    maxfdp1 = max(listen_sd, udp_sd) + 1;

    /* CICLO DI RICEZIONE RICHIESTE --------------------------------------------- */
    for(;;) {
        FD_SET(listen_sd, &rset);
        FD_SET(udp_sd, &rset);

        if((nread = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
            if(errno == EINTR) {
                continue;
            } else {
                perror("select");
                exit(5);
            }
        }
        /* GESTIONE RICHIESTE UDP  ----------------------------- */
        if(FD_ISSET(udp_sd, &rset)) {
            printf("Ricevuta richiesta di UDP: aggiornamento patente\n");
            len = sizeof(struct sockaddr_in);

            if(recvfrom(udp_sd, &req, sizeof(req), 0, (struct sockaddr*)&cliaddr, &len) < 0) {
                perror("recvfrom ");
                continue;
            }

            printf("Operazione richiesta aggiornamento patente %s per il veicolo: %s\n", req.patente, req.targa);

            hostUdp = gethostbyaddr((char*)&cliaddr.sin_addr, sizeof(cliaddr.sin_addr), AF_INET);
            if(hostUdp == NULL) {
                printf("client host information not found\n");
            } else {
                printf("Operazione richiesta da: %s %i\n", hostUdp->h_name, (unsigned)ntohs(cliaddr.sin_port));
            }

            /* Verifico targa */
            int ris = -1;
            for (int i = 0; i < N; i++)
            {
                if (strcmp(req.targa, tab[i].targa) == 0) {
                    strcpy(tab[i].patente, req.patente);
                    ris = 0;
                    continue;
                }
            }
            
            if (ris == 0) {
                printf("Patente aggiornata con successo!\n");
                stampa_struttura();
            }
            // Send result to the client
            if(sendto(udp_sd, &ris, sizeof(int), 0, (struct sockaddr*)&cliaddr, len) < 0) {
                perror("sendto ");
                continue;
            }
        }
        /* GESTIONE RICHIESTE TCP  ----------------------------- */
        if(FD_ISSET(listen_sd, &rset)) {
            if (getcwd(path, sizeof(path)) == NULL) {
                perror("Errore path");
                exit(1);
            }
            printf("Path corrente: %s\n", path);
            printf("Ricevuta richiesta TCP: download immagini\n");
            len = sizeof(cliaddr);
            if((conn_sd = accept(listen_sd, (struct sockaddr*)&cliaddr, &len)) < 0) {
                if(errno == EINTR) {
                    perror("Forzo la continuazione della accept");
                    continue;
                } else
                    exit(6);
            }
            if(fork() == 0) {
                close(listen_sd);
                hostTcp = gethostbyaddr((char*)&cliaddr.sin_addr, sizeof(cliaddr.sin_addr), AF_INET);
                if(hostTcp == NULL) {
                    printf("client host information not found\n");
                    close(conn_sd);
                    exit(6);
                } else
                    printf("Server (figlio): host client e' %s \n", hostTcp->h_name);
               
                
                // Leggo la richiesta del client
                while((nread = read(conn_sd, targa, sizeof(targa))) > 0) {
                    printf("Server (figlio):letti %d char \n", nread);
                    printf("Server (figlio): targa richiesta: %s\n", targa);

                    for (int i = 0; i < N; i++)
                    {
                        /* code */
                        if (strcmp(targa, tab[i].targa) == 0) {
                            dir1 = opendir(tab[i].folder);  
                             
                            while ((de = readdir(dir1)) != NULL) {
                                if (strstr(de->d_name, ".jpg") != NULL || strstr(de->d_name, ".png") != NULL ||
                                    strstr(de->d_name, ".jpeg") != NULL) {
                                printf("Immagine: %s\n", de->d_name);
                                sprintf(filePath, "%s/%s/%s", path, tab[i].folder, de->d_name);
                                printf("%s\n", filePath);
                                // Trasferimento nome immagine
                                write(conn_sd, strcpy(charBuff, de->d_name), sizeof(de->d_name));

                                // Trasferimento immagine                                 
                                fileToSend = fopen(filePath, "rb"); 
                                if (fileToSend == NULL) {
                                    printf("Errore apertura immagine\n");
                                    exit(1);
                                }
                                
                                fseek(fileToSend, 0, 2);
                                long size = ftell(fileToSend);
                                printf("Invio dimensione immagine: %d bytes\n", size);
                                fseek(fileToSend, 0, SEEK_SET);
                                //Send Picture Size
                                write(conn_sd, &size, sizeof(size));   
                                char ch;
                                
                                printf("Dimensione inviata! Ora procedo ad inviare l'immagine\n");
                                while(fread(charBuff, sizeof(char), sizeof(charBuff), fileToSend) > 0) {
                                    write(conn_sd, charBuff, sizeof(charBuff));
                                }
                                printf("Immagine inviata!\n");
                                    fclose(fileToSend);
                                }
                            }
                        }
                        closedir(dir1); 
                    }
                }  //while read req
                

                // Libero risorse
                printf("Figlio TCP terminato, libero risorse e chiudo. \n");
            }
                close(conn_sd);
        }

        }
}
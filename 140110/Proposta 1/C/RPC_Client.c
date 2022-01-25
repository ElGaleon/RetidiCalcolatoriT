/*
 * Christian Galeone - Matricola 924387
 * RPC_Client.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <rpc/rpc.h>
#include "RPC_xFile.h"

#define DIM 256
// #define N 4      

int main(int argc, char ** argv) {
    char * host;    // nome host
    CLIENT *cl;     // gestore del traposto

    int *ris;
    Evento *evento;
    Input *input;
    char ok[DIM];

    // Inizializzazione input
    input = malloc(sizeof(Input));
    input->descrizione = (char*) malloc(DIM);
    input->numero_biglietti = malloc(sizeof(int));

    // Inizializzazione evento
    evento = malloc(sizeof(Evento));
    evento->descrizione = (char*) malloc(DIM);
    evento->tipo = (char*) malloc(DIM);
    evento->data = (char*) malloc(DIM);
    evento->luogo = (char*) malloc(DIM);
    evento->prezzo = malloc(sizeof(int));
    evento->disponibilita = malloc(sizeof(int));

    // Controllo numero argomenti
    if (argc != 2) {
        printf("usage: %s server_host\n", argv[0]);
        exit(1);
    }
    host = argv[1];

    // Creazione gestore del trasporto
    cl = clnt_create(host, TICKETTWO, TICKETTWOVERSION, "udp");
    if (cl == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
    printf("Inserire \nI) per inserire evento\tA) per acquistare biglietti\t^D per terminare\n");
     while(gets(ok)) {
         if (strcmp(ok, "I") == 0) {
             printf("Inserisci descrizione evento: ");
                gets(ok);
                evento->descrizione = malloc(strlen(ok)+1);
                strcpy(evento->descrizione, ok);
             
             printf("Inserisci tipo evento: ");
                gets(ok);
                evento->tipo = malloc(strlen(ok)+1);
                strcpy(evento->tipo, ok);
            
             printf("Inserisci data evento: ");
                gets(ok);
                evento->data = malloc(strlen(ok)+1);
                strcpy(evento->data, ok);
            
             printf("Inserisci luogo evento: ");
                gets(ok);
                evento->luogo = malloc(strlen(ok)+1);
                strcpy(evento->luogo, ok);
            
             printf("Inserisci disponibilità biglietti evento: ");
                scanf("%i", &(evento->disponibilita));
            
             printf("Inserisci prezzo biglietti evento: ");
                 scanf("%i", &(evento->prezzo));
            
             
             printf("Invio richiesta al server!\n");
             printf("Evento inserito: %s\n", evento->descrizione);
             printf("Tipo inserito: %s\n", evento->tipo);
             printf("Data inserita: %s\n", evento->data);
             printf("Luogo inserito: %s\n", evento->luogo);
             printf("Disponibilità inserita: %i\n", evento->disponibilita);
             printf("Prezzo inserito: %i\n", evento->prezzo);
             
             ris = inserimento_evento_1(evento, cl);

              // Controllo risultato
             if (ris == NULL){ // Errore di RPC
                 clnt_perror(cl, host);
                 exit(1);
             }
            if (*ris < 0) {
                // Eventuale errore di logica del programma
                printf("Errore: la struttura è piena!\n");
            } else if (* ris == 0) {
                printf("Operazione di inserimento effettuata con successo \n");
            }
            
         } // IF I
         else if (strcmp(ok, "A") == 0) {
            printf("Inserisci descrizione evento: ");
            gets(ok);
            input->descrizione = malloc(strlen(ok)+1);
                strcpy(input->descrizione, ok);
            printf("Evento inserito: %s\n", input->descrizione);
            printf("Inserisci numero biglietti da acquistare: ");
            scanf("%d", &(input->numero_biglietti));
            printf("Numero biglietti richiesto: %d\n",input->numero_biglietti);

            printf("Invio richiesta al server!\n");
            // Invocazione remota Funzione2
             ris = acquista_biglietti_1(input, cl);
              // Controllo risultato
             if (ris == NULL){ // Errore di RPC
                 clnt_perror(cl, host);
                 exit(1);
             }
            if (*ris < 0) {
                // Eventuale errore di logica del programma
                printf("Errore: l'evento cercato non esiste o non vi è disponibilità di biglietti!\n");
            } else if (* ris == 0) {
                printf("Operazione di acquisto effettuata con successo \n");
            }
         } // IF A 
         else {
             printf("Argomento di ingresso errato!\n");
         }
           printf("\nInserire \nI) per inserire evento\tA) per acquistare biglietti\t^D per terminare\n");
     }

     // Libero le risorse, distruggendo il gestore di trasporto
     clnt_destroy(cl);
     exit(0);
}
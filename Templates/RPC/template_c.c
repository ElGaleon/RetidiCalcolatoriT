/*
 * Christian Galeone - Matricola 924387
 * template_c.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <rpc/rpc.h>
// #include "template.h"

#define DIM 256
// #define N 4      

int main(int argc, char ** argv) {
    char * host;    // nome host
    CLIENT *cl;     // gestore del traposto

    int *ris;
    char ok[DIM];
    //Input *input;

    // Controllo numero argomenti
    if (argc != 2) {
        printf("usage: %s server_host\n", argv[0]);
        exit(1);
    }
    host = argv[1];

    // Creazione gestore del trasporto
    //cl = clnt_create(host, TEMPLATE, TEMPLATEVERSION, "udp");
    if (cl == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
    // printf("Inserire \nG) per vedere classifica giudici\tV) per esprimere il voto\t^D per terminare\n");
     while(gets(ok)) {
         if (strcmp(ok, "") == 0) {

             // Invocazione remota Funzione1
             ris = Funzione1(cl);

             // Controllo risultato
             if (ris == NULL){ // Errore di RPC
                 clnt_perror(cl, host);
                 exit(1);
             }
            if (*ris < 0) {
                // Eventuale errore di logica del programma
                printf("Errore!\n");
            } else if (* ris == 0) {
                printf("Operazione effettuata con successo \n");
            }
         } // IF Funzione1
         else if (strcmp(ok, "") == 0) {

            // Invocazione remota Funzione2
             ris = classifica_giudici_1(cl);

              // Controllo risultato
             if (ris == NULL){ // Errore di RPC
                 clnt_perror(cl, host);
                 exit(1);
             }
            if (*ris < 0) {
                // Eventuale errore di logica del programma
                printf("Errore!\n");
            } else if (* ris == 0) {
                printf("Operazione effettuata con successo \n");
            }
         } // IF Funzione2 
         else {
             printf("Argomento di ingresso errato!\n");
         }
          // printf("Inserire \nG) per vedere classifica giudici\tV) per esprimere il voto\t^D per terminare\n");
     }

     // Libero le risorse, distruggendo il gestore di trasporto
     clnt_destroy(cl);
     exit(0);
}
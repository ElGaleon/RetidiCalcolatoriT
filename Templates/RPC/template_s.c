/*
 * Christian Galeone - Matricola 924387
 * implementazione procedure remote: "classifica_giudici" e "esprimi_voto"
 * + inizializzazione struttura
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpc/rpc.h>
// #include "template.h"

// #define N

/* STATO SERVER */
// Input *input;

/* VARIABILI GLOBALI STATICHE */
static int inizializzato = 0;

/* Funzione inizializzazione server*/
void inizializza() {
    int i, j;

        if (inizializzato == 1) return;
       
        printf("Inizializzo struttura dati!\n");
        // inizializzazione struttura dati
        
        // Eventuale riempimento altri posti
    
        inizializzato = 1;
        printf("Terminata inizializzazione struttura dati!\n");
}   // funzione1

void* funzione1_svc(Input *input, struct svc_req *reqstp){
    inizializza();
    return;
}   // funzione1_svc

 void * funzione2_svc(Input * input, struct svc_req *rqstp) {
    inizializza();
    return;
} // esprimi_voto

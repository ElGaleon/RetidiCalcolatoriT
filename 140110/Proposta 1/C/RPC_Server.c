/*
 * Christian Galeone - Matricola 924387
 * implementazione procedure remote: "inserimento_evento" e "acquista_biglietti"
 * + inizializzazione struttura
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include "RPC_xFile.h"

#define N 20
#define MAXS 256

/* STATO SERVER */

/* VARIABILI GLOBALI STATICHE */
static int inizializzato = 0;
static Evento tab[N];

void stampa() {
    printf("DESCRIZIONE\tTIPO\t\tDATA\t\tLUOGO\t\tDISPONIBILITA\tPREZZO\n");
    for (int i = 0; i < N; i++)
    {
        /* code */
        printf("%s\t\t%s\t\t%s\t\t%s\t\t%i\t\t%i\n", 
        tab[i].descrizione, tab[i].tipo, tab[i].data, 
        tab[i].luogo, tab[i].disponibilita, tab[i].prezzo);
    }
    
}

/* Funzione inizializzazione server*/
void inizializza() {
    int i, j;

        if (inizializzato == 1) {
            printf("Struttura già inizializzata!\n");
            return;
        }
       else {
        printf("Inizializzo struttura dati!\n");
        // inizializzazione struttura dati
        for (int i = 0; i < N; i++)
        {
            /* code */
            tab[i].descrizione = malloc(strlen("L")+1);
            strcpy(tab[i].descrizione, "L");

            tab[i].tipo = malloc(strlen("L")+1);
            strcpy(tab[i].tipo, "L");

            tab[i].data = malloc(strlen("L")+1);
            strcpy(tab[i].data, "L");

            tab[i].luogo = malloc(strlen("L")+1);
            strcpy(tab[i].luogo, "L");

            tab[i].disponibilita =-1;

            tab[i].prezzo = -1;
        }   
        // Eventuale riempimento altri posti
            free(tab[1].descrizione);
            tab[1].descrizione = malloc(strlen("Sting")+1);
            strcpy(tab[1].descrizione, "Sting");
            free(tab[1].tipo);
            tab[1].tipo = malloc(strlen("Concerto")+1);
            strcpy(tab[1].tipo, "Concerto");
            free(tab[1].data);
            tab[1].data = malloc(strlen("11/04/2014")+1);
            strcpy(tab[1].data, "11/04/2014");
            free(tab[1].luogo);
            tab[1].luogo = malloc(strlen("Verona")+1);
            strcpy(tab[1].luogo, "Verona");
            tab[1].disponibilita = 40;
            tab[1].prezzo =40;

            free(tab[2].descrizione);
            tab[2].descrizione = malloc(strlen("Milan-Inter")+1);
            strcpy(tab[2].descrizione, "Milan-Inter");
            free(tab[2].tipo);
            tab[2].tipo = malloc(strlen("Calcio")+1);
            strcpy(tab[2].tipo, "Calcio");
            free(tab[2].data);
            tab[2].data = malloc(strlen("03/05/2014")+1);
            strcpy(tab[2].data, "03/05/2014");
            free(tab[2].luogo);
            tab[2].luogo = malloc(strlen("Milano")+1);
            strcpy(tab[2].luogo, "Milano");
            tab[2].disponibilita = 21;
            tab[2].prezzo = 150;
            
            free(tab[4].descrizione);
            tab[4].descrizione = malloc(strlen("GP Mugello")+1);
            strcpy(tab[4].descrizione, "GP Mugello");
            free(tab[4].tipo);
            tab[4].tipo = malloc(strlen("Formula 1")+1);
            strcpy(tab[4].tipo, "Formula 1");
            free(tab[4].data);
            tab[4].data = malloc(strlen("07/09/2014")+1);   
            strcpy(tab[4].data, "07/09/2014");
            free(tab[4].luogo);
            tab[4].luogo = malloc(strlen("Mugello")+1);
            strcpy(tab[4].luogo, "Mugello");
            tab[4].disponibilita = 10;
            tab[4].prezzo = 200;

        inizializzato = 1;
        printf("Terminata inizializzazione struttura dati!\n");
        stampa();
        return;
       }
}   // funzione1

int* inserimento_evento_1_svc(Evento *input, struct svc_req *reqstp){
    printf("Ricevuta richiesta per inserire un nuovo evento: %s\n", input->descrizione);
    inizializza();
    int result = -1;
        for (int i = 0; i < N && result == -1; i++)
        {
            if (strcmp(tab[i].descrizione, "L") == 0) {
                // Inserimento nuovo evento
            free(tab[i].descrizione);
            tab[i].descrizione = malloc(strlen(input->descrizione)+1);
            strcpy(tab[i].descrizione, input->descrizione);
            free(tab[i].tipo);
            tab[i].tipo = malloc(strlen(input->tipo)+1);
            strcpy(tab[i].tipo, input->tipo);
            free(tab[i].data);
            tab[i].data = malloc(strlen(input->data)+1);
            strcpy(tab[i].data, input->data);
            free(tab[i].luogo);
            tab[i].luogo = malloc(strlen(input->luogo)+1);
            strcpy(tab[i].luogo, input->luogo);
            tab[i].disponibilita = input->disponibilita;
            tab[i].prezzo = input->prezzo;
           
            printf("Inserimento evento avvenuto con successo!\n");
            result = 0;
            }
        }
        
    return result;
}   // inserisci_evento

 int * acquista_biglietti_1_svc(Input * input, struct svc_req *rqstp) {
     printf("Ricevuta richiesta per acquistare biglietti dell'evento %s\n", input->descrizione);
    inizializza();
    static int result = -1;
    for (int i = 0; i < N && result ==-1; i++)
    {
        if (strcmp(input->descrizione, tab[i].descrizione) == 0) {
            printf("Ho trovato l'evento richiesto! Controllo disponibilità biglietti...\n");
            int bigliettiRim = tab[i].disponibilita - input->numero_biglietti;
            printf("Biglietti rimasti: %i\tBiglietti richiesti: %i\n", tab[i].disponibilita, input->numero_biglietti);
            if (bigliettiRim >= 0) {
                printf("I biglietti sono disponibili! Aggiorno tabella...\n");
                // L'acquisto si può effettuare
                tab[i].disponibilita = bigliettiRim;
                stampa();
                printf("Acquisto completato!Invio risultato\n");
                result = 0;
                break;
            }
            else {
                printf("I biglietti richiesti sono più di quelli disponibili\n");
            }
        }
    }
        if (result ==-1)
        printf("Evento richiesto non trovato!\n");
    return (&result);
} // acquista_biglietti

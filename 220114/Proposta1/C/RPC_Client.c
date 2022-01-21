/*
 * xfactor_c.c
 */

#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RPC_xFile.h"

#define MAXS 256
#define MAXFILE 6	

int main (int argc, char *argv[])
{
	char *host; //nome host
  	CLIENT *cl; //gestore del trasporto

	int i, *ris, numVoti=-1;
  	char c;
	int * eliminaOccorrenze;
	Output *listaFileTesto;
	void *voidValue;

  	//per leggere stringhe da standard di input
  	char ok[MAXS];
	File * file;

	//MALLOC FILE
	file = malloc(sizeof(File));
	file->fileName = (char*)malloc(MAXS);
	//printf("ciao --------------------------ciao\n");

  /****************************************************************************************************************/

	//Controllo degli argomenti --> 2 di default...
  	if (argc != 2){
    		printf ("usage: %s server_host\n", argv[0]);
    		exit (1);
  	}
  	host = argv[1];

	//Creazione gestore del trasporto
	cl = clnt_create (host, RPC_XFILES, RPC_XFILESVERSION, "udp");
	if (cl == NULL){
		clnt_pcreateerror (host);
		exit (1);
	}

	//Interazione con l'utente
	printf("Inserire:\n1)\tEliminazione Occorrenze\n2)\tLista file di testo\n^D\tper terminare: ");

	while (scanf("%s",ok)==1){
		if( strcmp(ok,"1")==0 ){
			printf("Inserisci nome file di testo: ");
			scanf("%s", ok);
			strcpy(file->fileName, ok);

			// Invocazione remota
			eliminaOccorrenze = elimina_occorrenze_1(file, cl);
			
			//Controllo del risultato
			if(eliminaOccorrenze == NULL){
				//Errore di RPC
				clnt_perror(cl, host);
				exit(1);
			} 
			if (*eliminaOccorrenze < 0) {
				printf("Problemi nella ricerca del file\n");
			} else 
			printf("Nel file sono stati eliminati %d numeri!\n", *eliminaOccorrenze);

		} // if 1

		else if( strcmp(ok,"2")==0 ){
			void *v;
			printf("Inserisci nome direttorio: ");
			scanf("%s", ok);
			strcpy(file->fileName, ok);

			// Invocazione remota
			listaFileTesto = lista_filetesto_1(file, cl);

			//Controllo del risultato
			if(listaFileTesto->files == NULL){
				//Errore di RPC
				clnt_perror(cl, host);
				exit(1);
			} else {
				printf("File di testo trovati:\n");
				for (int i = 0; i < sizeof(listaFileTesto); i++)
				{
					/* code */
					if (listaFileTesto->files[i].fileName!= NULL && listaFileTesto->files[i].fileName != "-")
					printf("%s\n", listaFileTesto->files[i]);
				}
				
			}

			
		} 
		else printf("Operazione non disponibile\n");


		printf("Inserire:\n1)\tEliminazione Occorrenze\n2)\tLista file di testo\n^D\tper terminare: ");
	} // while

	// Libero le risorse, distruggendo il gestore di trasporto
	clnt_destroy (cl);
	exit(0);
}//main

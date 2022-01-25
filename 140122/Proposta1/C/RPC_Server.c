/*
 * RPC_Server.c
 * @Author: Galeone Christian - Matricola 924387
 */

#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "RPC_xFile.h"
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h>

#define MAXS 256
#define MAXFILE 6

/*STATO INTERNO PRIVATO DEL SERVER*/

//variabili globali statiche
static int inizializzato = 0;
File *file;


/*Stato interno parzialmente implementato*/
void inizializza(){
	int i;
	if (inizializzato == 1) return;

	

	inizializzato = 1;
	printf("Terminata inizializzazione struttura dati!\n");
}

//implementazione delle procedure definite nel file XDR
Output * lista_filetesto_1_svc(File *file, struct svc_req *reqstp){
	static Output result;
	inizializza();
		
		for (int i = 0; i < MAXFILE; i++) {
			result.files[i].fileName = (char*) malloc(MAXS);
			strcpy(result.files[i].fileName, "-");
		}

	 /* ALGORITMO LISTA FILE */
        DIR *dir;
        char temp[] = "temp_file.txt";
		char ch;
		struct dirent *files;
		printf("Nome directory ricevuto: %s\n", file->fileName);

		dir = opendir(file->fileName);
		char txtExt [5] = ".txt";
		char ext[5];
		int filesFound = 0;

		if (dir)
    {
        while ((files = readdir(dir)) != NULL)
        {
			int len = strlen(files->d_name);
			/* Creazione estensione */
			ext[0] = files->d_name[len-4];
			ext[1] = files->d_name[len-3];
			ext[2] = files->d_name[len-2];
			ext[3] = files->d_name[len-1];
			ext[4] = '\0';

			if (strcmp(txtExt, ext) == 0 && filesFound < MAXFILE) {
					// result.files[filesFound].fileName = malloc(strlen(files->d_name)+1);
					strcpy(result.files[filesFound].fileName, files->d_name);
					printf("%s è un file di testo\n",result.files[filesFound].fileName);
					filesFound++;
			}
			else 
				printf("%s non è un file di testo\n", files->d_name);
        }
        closedir(dir);
    }
	printf("File di testo trovati: %d\nProcedo ad inviare!\n", filesFound);
	return (&result);
}

int * elimina_occorrenze_1_svc(File *file, struct svc_req *reqstp){
	static int result;
	result = -1;
	int i, digits_deleted;

	// Controllo che il nome sia di un file e sia di testo
	// Elimino le occorrenze non scrivendole
	inizializza();

	 /* ALGORITMO ELIMINA OCCORRENZE */
        int fi, fout, nread;
        char temp[] = "temp_file.txt";
		char ch;
		printf("Nome file ricevuto: %s\n", file->fileName);

		fi = open(file->fileName, O_RDONLY);
		fout = open(temp, O_WRONLY|O_CREAT|O_TRUNC, 0777);

		if (fi < 0){
			printf("Errore lettura file originale %s\n", file->fileName);
			close(fi);
			return (&result);
		}
		printf("FD: %d\n", fi);
		if (fout < 0){
			printf("Errore apertura file temp, %s\n", temp);
			close(fi);
			close(fout);
			exit(1);
		}
		printf("FD temp: %d\n", fout);

		result = 0;

		while ( read(fi, &ch, sizeof(char)) > 0) {
			printf("Letto il carattere %c\n", ch);

			if (ch >= '0' && ch <= '9') {
				result++;
			} else {
				if (write(fout, &ch, 1) < 0) {	// write deve passare indirizzo
					perror("Errore scrittura nel file temp: ");
					close(fout);
					close(fi);
				}
			}
		}
		printf("Nel file sono stati eliminati %d numeri!\n", result);
		close(fi);
		close(fout);
		// Rinominazione file indicato
		rename(temp, file->fileName);

	return(&result);
}

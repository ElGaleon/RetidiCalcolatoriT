#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define DIM_BUFF 256

int main(int argc, char *argv[]){

	int sd, nread, port;
	char buff[DIM_BUFF], targa[DIM_BUFF], directory[DIM_BUFF];
	struct hostent *host;
	struct sockaddr_in servaddr;

	//controllo argomenti 
	if(argc!=3){
		printf("Error:%s serverAddress serverPort\n", argv[0]);
		exit(1);
	}
	printf("Client avviato\n");

	/* inizializzazione indirizzo server */
	memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	host = gethostbyname(argv[1]);
	if (host == NULL){
		printf("%s not found in /etc/hosts\n", argv[1]);
		exit(2);
	}
	
	nread = 0;
	while (argv[2][nread] != '\0'){
		if ((argv[2][nread] < '0') || (argv[2][nread] > '9')){
			printf("Secondo argomento non intero\n");
			exit(2);
		}
		nread++;
	}
	port = atoi(argv[2]);
	if (port < 1024 || port > 65535)
	{printf("Porta scorretta...");exit(2);}

	servaddr.sin_addr.s_addr=((struct in_addr*) (host->h_addr))->s_addr;
	servaddr.sin_port = htons(port);

	//creo la socket che permane fino alla file dell'esecuzione del client
	sd=socket(AF_INET, SOCK_STREAM, 0);
	if (sd <0){perror("apertura socket "); exit(3);}
	printf("Creata la socket sd=%d\n", sd);

	//Operazione di BIND implicita nella connect 
	if (connect(sd,(struct sockaddr *) &servaddr, sizeof(struct sockaddr))<0)
	{perror("Errore in connect"); exit(4);}
	printf("Connect ok\n");

	/*  ciclo di accettazione di richieste di file ------- */
	printf("Inserire la targa di cui si vogliono ottenere le immagini: ");
	while (gets(targa)){
		char fold[FILENAME_MAX], fileName[DIM_BUFF], path[DIM_BUFF];
		int dim, countFile=0, numFile=0;
		int countdim=0;
		int dimbuff=0;
		FILE* fd;

		if (write(sd, targa, (strlen(targa)))<0)
		{
			perror("write");
			printf("Inserire la targa di cui si vogliono ottenere le immagini: ");
			continue;
		}

		printf("Targa %s inviata... \n", targa);
		getcwd(path, sizeof(path));
		sprintf(fold, "%s_img", targa);
		
		int dir= mkdir(fold, 0777);
			
		printf("Cartella creata %s ... \n", fold);
		read(sd, &numFile, sizeof(int));
		printf("Numero file %d\n", numFile);

		//comincio a ricevere i dati: nome file
		while(countFile<numFile && read(sd, fileName, sizeof(fileName))>0 ){
			countFile++;
			printf("File name %s\n", fileName);
			sprintf(directory, "%s/%s/%s",path, fold , fileName);
			printf("directory file %s\n", directory);

			if((fd=fopen(directory, "wb"))==NULL){
                    perror("open new file");
			}else
				printf("File aperto con successo... \n");
			

			if(read(sd, &dim, sizeof(int))>0){
				printf("dimensione file %d\n", dim);
			}

			countdim=0;
			dimbuff=0;
			//ciclo che legge il  contenuto del file
			while((countdim+=dimbuff)<dim && read(sd, &dimbuff, sizeof(int))>0)
			{
				
				//printf("%d DIMBUFF", i);
				read(sd, buff, dimbuff);

				if(fwrite(buff, sizeof(char), dimbuff, fd)<0){
					perror("write");
					printf("Inserire un direttorio corretto: ");
					continue;
				}
			}
			fwrite(buff, sizeof(char), strlen(buff), fd);
			printf("COUNT: %d	", countdim);
			fclose(fd);
		}
		printf("Inserire la targa di cui si vogliono ottenere le immagini: ");

	}//while
	
	/*prima di terminare il client chiudo la connessione*/
	printf("Chiudo connessione\n");
	close(sd); //chiusura sempre DENTRO
	printf("\nClient: termino...\n");
	exit(0);
}

//Server Select 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define DIM_BUFF 256
#define NUM 10
#define DIM 100
#define max(a,b) ((a) > (b) ? (a) : (b))


 /********************************************************/
typedef struct {
	char targa[DIM];
	char patente[DIM];
	char tipo[DIM];
	char cartella[DIM];
 }Prenotazione;

typedef struct {
	char targa[DIM];
	char nuovaP[DIM];
 }Request;
/********************************************************/
void gestore(int signo){
	int stato;
	printf("esecuzione gestore di SIGCHLD\n");
	wait(&stato);
}
/********************************************************/

int main(int argc, char **argv){
	
	int  tcpfd, connfd, udpfd, fd_file, nready, maxfdp1;
	const int on = 1;
	char ch[2], buff[DIM_BUFF], targa[7];
	fd_set rset;
	int len, nread,  port, count=0;
	struct sockaddr_in cliaddr, servaddr;
	Request req;
	Prenotazione prenotazioni[NUM];

	/* controllo argomenti*/
	if(argc!=2){
		printf("Error: %s port\n", argv[0]);
		exit(1);
	}
	nread = 0;
	while (argv[1][nread] != '\0'){
		if ((argv[1][nread] < '0') || (argv[1][nread] > '9')){
			printf("Terzo argomento non intero\n");
			exit(2);
		}
		nread++;
	}
	port = atoi(argv[1]);
	if (port < 1024 || port > 65535){
		printf("Porta scorretta...");
		exit(2);
	}
	
	//inizializzazione struttura
	for(int i=0; i<NUM; i++){
		strcpy(prenotazioni[i].targa, "L");
		strcpy(prenotazioni[i].patente, "L");
		strcpy(prenotazioni[i].tipo, "L");
		strcpy(prenotazioni[i].cartella, "L");
	}

	strcpy(prenotazioni[0].targa, "AN745NL");
	strcpy(prenotazioni[0].patente, "00003");
	strcpy(prenotazioni[0].tipo, "auto");
	strcpy(prenotazioni[0].cartella, "AN745NL_img");

	strcpy(prenotazioni[1].targa, "FE457GF");
	strcpy(prenotazioni[1].patente, "43003");
	strcpy(prenotazioni[1].tipo, "auto");
	strcpy(prenotazioni[1].cartella, "FE457GF_img");

	strcpy(prenotazioni[2].targa, "LR897AH");
	strcpy(prenotazioni[2].patente, "68503");
	strcpy(prenotazioni[2].tipo, "camper");
	strcpy(prenotazioni[2].cartella, "LR897AH_img");

	strcpy(prenotazioni[3].targa, "MD506DW");
	strcpy(prenotazioni[3].patente, "05678");
	strcpy(prenotazioni[3].tipo, "auto");
	strcpy(prenotazioni[3].cartella, "MD506DW_img");

	//fine inizializzazione struttura


	/*inizializzo indirizzo server */
	memset ((char *)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	printf("Server avviato\n");

	/* creazione socket tcp*/
	tcpfd=socket(AF_INET, SOCK_STREAM, 0);
	if (tcpfd <0)
	{perror("apertura socket TCP "); exit(1);}
	printf("Creata la socket TCP d'ascolto, fd=%d\n", tcpfd);

	if (setsockopt(tcpfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
	{perror("set opzioni socket TCP"); exit(2);}
	printf("Set opzioni socket TCP ok\n");

	if (bind(tcpfd,(struct sockaddr *) &servaddr, sizeof(servaddr))<0)
	{perror("bind socket TCP"); exit(3);}
	printf("Bind socket TCP ok\n");

	if (listen(tcpfd, 5)<0)
	{perror("listen"); exit(4);}
	printf("Listen ok\n");

	/* creazione socket udp*/
	udpfd=socket(AF_INET, SOCK_DGRAM, 0);
	if(udpfd <0)
	{perror("apertura socket UDP"); exit(5);}
	printf("Creata la socket UDP, fd=%d\n", udpfd);

	if(setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
	{perror("set opzioni socket UDP"); exit(6);}
	printf("Set opzioni socket UDP ok\n");

	if(bind(udpfd,(struct sockaddr *) &servaddr, sizeof(servaddr))<0)
	{perror("bind socket UDP"); exit(7);}
	printf("Bind socket UDP ok\n");

	/* gestore per non avere figli zombie*/
	signal(SIGCHLD, gestore);

	/* pulizia e settaggio maschera dei file descriptor*/
	FD_ZERO(&rset);
	maxfdp1=max(tcpfd, udpfd)+1;

	/*ciclo ricezione eventi SELECT */
	for(;;){
		FD_SET(tcpfd, &rset);
		FD_SET(udpfd, &rset);

		if ((nready=select(maxfdp1, &rset, NULL, NULL, NULL))<0){
			if (errno==EINTR) continue;
			else {perror("select"); exit(8);}
		}

		/* GESTIONE RICHIESTE TCP */
		if (FD_ISSET(tcpfd, &rset)){
			printf("Ricevuta richiesta di get di un file\n");
			len = sizeof(struct sockaddr_in);
			if((connfd = accept(tcpfd,(struct sockaddr *)&cliaddr,&len))<0){
				if (errno==EINTR) continue;
				else {perror("accept"); exit(9);}
			}

			if (fork()==0){ /* processo figlio che serve la richiesta di operazione */
				close(tcpfd);
				printf("Dentro il figlio, pid=%i\n", getpid());
				char folder[DIM], path[DIM_BUFF], fileName[DIM_BUFF], directoryFile[DIM_BUFF];
				DIR *fd_dir;
				struct dirent *dd;
				char ch;
				int numFile=0, dim=-1, countdim=0;
				FILE* fd;
				//aspetto la targa
               getcwd(path, sizeof(path));
				while((read(connfd, targa, sizeof(targa))) > 0)
				{
					printf("Richiesta targa %s\n", targa);
					
					for(int i=0; i<NUM; i++){
						//printf("targa %s\n", prenotazioni[i].targa);
						if(strcmp(targa, prenotazioni[i].targa)==0){
							printf("Cartella %s\n", prenotazioni[i].cartella);
							strcat(folder, prenotazioni[i].cartella);}
					}

					sprintf(folder, "%s/%s_img", path, targa);
					//se non esiste torno un messaggio di errore
					if ((fd_dir= opendir(folder))!=NULL  )
					{
						while ((dd = readdir(fd_dir))!= NULL ) 
						{	//suppongo che dentro la cartella ci siano solo file non altre cartelle

							strcpy(fileName,  dd->d_name);
							if(fileName[0]!='.' && !strstr(fileName, ".txt")){
								numFile++;
							}
						}
						closedir(fd_dir);
					}
					if ((fd_dir= opendir(folder))!=NULL  ){
						write(connfd, &numFile, sizeof(int));
						//ciclo per ogni elemento dentro alla directory
						while ((dd = readdir(fd_dir))!= NULL ) 
						{	//suppongo che dentro la cartella ci siano solo file non altre cartelle
						countdim=0;
							strcpy(fileName,  dd->d_name);

							if(fileName[0]!='.' && !strstr(fileName, ".txt")){
							
							printf("Trovato il file %s\n", fileName);
								// invio il nome del file
								write(connfd, fileName, sizeof(fileName));

								//costruisco il path completo con il nome del file e la stampo
								sprintf(directoryFile, "%s/%s", folder, fileName);
								

								if( (fd= fopen(directoryFile, "rb"))==NULL){
									printf("File inesistente\n");
									continue;
								}
								//calcolo dimensione file
								fseek(fd, 0, 2);
								if((dim=ftell(fd))<=0){			
									printf("ERRORE\n");
								}								
								printf("DIM %d\n", dim);
								//mando la dimensione
								write(connfd, &dim, sizeof(int));

								//riparto all'inizio del file;
								fseek(fd, 0, 0);
								if(buff==NULL){
									printf("ERRORE\n");
								}
								countdim=0;
								int count=0;
								int i=0;
								while ((countdim+=count)<=dim && (count=fread(buff, sizeof(char), sizeof(buff), fd)) > 0) {
									//printf("%d DIMBUFF: %d	",i, count);
									write(connfd, &count, sizeof(int));
									write(connfd, buff,count);	
									i++;
								}
								
								//printf("count: %d	", countdim);
							fclose(fd);
							}// CONTROLLO SE NON È .. O .
					
						
					}// WHILE finchè ci sono file nel direttorio
						//write(connfd, "\0", 1);
						//shutdown(connfd, 1);
						printf("chiusura cartella\n");
						closedir(fd_dir);
					}// if esiste direttorio
					else {
						printf("cartella inesistente\n");
					}
					
				printf("In attesa di una targa...\n");
				}// while targhe

				printf("Figlio %i: chiudo connessione e termino\n", getpid());
				close(connfd);
				exit(0);
			}//figlio

			/* padre chiude la socket dell'operazione */
			close(connfd);
		} /* fine gestione richieste di file */

		 //GESTIONE RICHIESTE DI UDP

		if (FD_ISSET(udpfd, &rset)){
            int fdfd, success=-1;
            char buffer[DIM_BUFF], t[DIM_BUFF];
			printf("Server: ricevuta richiesta dell'aggiornamento della patente\n");
			len=sizeof(struct sockaddr_in);
            
           
			if (recvfrom(udpfd, &req, sizeof(Request), 0, (struct sockaddr *)&cliaddr, &len)<0)
			{perror("recvfrom"); continue;}
			
			printf("Targa di cui cambiare patente: %s\n", req.targa);

			for(int i=0; i<NUM; i++){
				if(strcmp(req.targa, prenotazioni[i].targa)==0){
					printf("Patente vecchia %s\n", prenotazioni[i].patente);
					strcpy(prenotazioni[i].patente, req.nuovaP);
					printf("Patente aggiornata %s\n", prenotazioni[i].patente);
					success=0;
				}
			}

			if (sendto(udpfd, &success, sizeof(success), 0, (struct sockaddr *)&cliaddr, len)<0)
			{perror("sendto"); continue;}
			
            success=-1;
			printf("Ho inviato l'esito dell'operazione, aspetto nuovi aggiornamenti\n");


		} //fine gestione richieste di aggiornamento
		
	} /* ciclo for della select */
}

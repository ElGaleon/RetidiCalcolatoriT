import java.io.*;
import java.net.*;

public class ServerCon {
    private static final int MAXEVENTS = 10;
    private static String [][] events = new String [MAXEVENTS][6];

    public static void main(String[] args) throws IOException {
        int port = -1;
        
        // Inizializzazione struttura dati
        for (int i = 0; i < MAXEVENTS; i++) {
            events[i][0] = "L";
            events[i][1] = "L";
            events[i][2] = "L";
            events[i][3] = "L";
            events[i][4] = "L";
            events[i][5] = "L";
        }

        // Inserimento dati fittizi
            events[1][0] = "Sting";
            events[1][1] = "Concerto";
            events[1][2] = "11/01/2014";
            events[1][3] = "Verona";
            events[1][4] = "40";
            events[1][5] = "40";

            events[2][0] = "Milan-Inter";
            events[2][1] = "Calcio";
            events[2][2] = "18/01/2014";
            events[2][3] = "Milano";
            events[2][4] = "21";
            events[2][5] = "150";

            events[9][0] = "GP Mugello";
            events[9][1] = "Formula 1";
            events[9][2] = "13/03/2014";
            events[9][3] = "Mugello";
            events[9][4] = "10";
            events[9][5] = "200";

        try {
            if (args.length == 1) {
                port = Integer.parseInt(args[0]);
                // controllo che la porta sia nel range consentito 1024-65535
                if (port < 1024 || port > 65535) {
                    System.out.println("Usage: java ServerCon [serverPort>1024]");
                    System.exit(1);
                }
            } else {
                System.out.println("Usage: java ServerCon port");
                System.exit(1);
            }
        }
        catch (Exception e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("Usage: java ServerCon port");
            System.exit(1);
        }

        ServerSocket serverSocket = null;
        Socket clientSocket = null;

        try {
            serverSocket = new ServerSocket(port);
            serverSocket.setReuseAddress(true);
            System.out.println("ServerCon: avviato ");
            System.out.println("Server: creata la server socket: " + serverSocket);
        } catch (Exception e) {
            System.err
                    .println("Server: problemi nella creazione della server socket: "+ e.getMessage());
            e.printStackTrace();
            serverSocket.close();
            System.exit(1);
        }

        try {
            while (true) {
                System.out.println("Server: in attesa di richieste...\n");

                try {
                    clientSocket = serverSocket.accept(); //bloccante!!!
                    System.out.println("Server: connessione accettata: " + clientSocket);
                } catch (Exception e) {
                    System.err
                            .println("Server: problemi nella accettazione della connessione: "
                                    + e.getMessage());
                    e.printStackTrace();
                    continue;
                }

                try {
                    new ServerThread(clientSocket, events).start();
                } catch (Exception e) {
                    System.err.println("Server: problemi nel server thread: "
                            + e.getMessage());
                    e.printStackTrace();
                    continue;
                }
            }// while true
        }
        catch (Exception e) {
            e.printStackTrace();
            System.out.println("Server: termino...");
            System.exit(2);
        }
    }
}

class ServerThread extends Thread {
    private Socket clientSocket = null;
    private String [][] events;

    public ServerThread(Socket clientSocket, String [][] events) {
        this.clientSocket = clientSocket;
        this.events = events;
    }

    public void run() {
        System.out.println("Attivazione figlio: "+ Thread.currentThread().getName());

        DataInputStream inSock;
        DataOutputStream outSock;

        try {
            inSock = new DataInputStream(clientSocket.getInputStream());
            outSock = new DataOutputStream(clientSocket.getOutputStream());
        } catch (IOException ioe) {
            System.out.println("Problemi nella creazione degli stream di input/output su socket: ");
            ioe.printStackTrace();
            return;
        }
        try {
            try {
                while(true) {
                String luogo, tipo;
                int op,  dim, nEventi = 0;
                String prezzo;
                String [][] result;
                long numeroByte;
                File fileCorr;
                FileOutputStream outFileCorr;

                while ((op= inSock.readInt()) != -1) {
                   if (op == 1) {       // Visualizzazione eventi in un det. luogo e tipo
                        System.out.println("Richiesta operazione 1");
                        tipo = inSock.readUTF();
                        System.out.println("Tipo ricerca: " + tipo);
                        luogo = inSock.readUTF();
                        dim = 0;
                        System.out.println("Luogo ricerca: " + luogo);
                        for(String [] e: events) {
                            if (e[1].equals(tipo) && e[3].equals(luogo))
                                dim++;
                        }
                        System.out.println("Eventi trovati: " + dim);
                        // result = new String[dim][6];
                        // Invio dimensione a client
                        outSock.writeInt(dim);
                        dim = 0;
                        System.out.println("Invio risultati");
                        for (String [] e: events) {
                            if (e[1].equals(tipo) && e[3].equals(luogo)) {
                                for (String p: e) {
                                    System.out.println("Invio " + p);
                                    outSock.writeUTF(p);
                                }
                            }
                        }
                        System.out.println("Invio terminato\n");
                   } // IF 1
                   else if (op == 2) {  // Visualizzazione eventi con prezzo inferiore o uguale a quello dato
                        System.out.println("Richiesta operazione 2");
                        prezzo = inSock.readUTF();
                        dim = 0;
                        System.out.println("Prezz ricerca: " + prezzo);
                        for(String [] e: events) {
                            if (e[5].compareTo("L") != 0 && Integer.parseInt(e[5]) <= Integer.parseInt(prezzo))
                                dim++;
                        }
                        System.out.println("Eventi trovati: " + dim);
                        // result = new String[dim][6];
                        // Invio dimensione a client
                        outSock.writeInt(dim);
                        dim = 0;
                        for (String [] e: events) {
                            if (e[5].compareTo("L") != 0 && Integer.parseInt(e[5]) <= Integer.parseInt(prezzo)) {
                                for (String p: e) {
                                    System.out.println("Invio " + p);
                                    outSock.writeUTF(p);
                                }
                            }
                        }
                        System.out.println("Invio terminato\n");

                   } // IF 2
                } // while
            }
            }catch (EOFException eof) {
                System.out.println("Raggiunta la fine delle ricezioni, chiudo...");
                clientSocket.close();
                System.out.println("PutFileServer: termino...");
                System.exit(0);
            } catch (SocketTimeoutException ste) {
                System.out.println("Timeout scattato: ");
                ste.printStackTrace();
                clientSocket.close();
                System.exit(1);
            } catch (Exception e) {
                System.out.println("Problemi, i seguenti : ");
                e.printStackTrace();
                System.out.println("Chiudo ed esco...");
                clientSocket.close();
                System.exit(2);
            }
        }catch (IOException ioe) {
            System.out.println("Problemi nella chiusura della socket: ");
            ioe.printStackTrace();
            System.out.println("Chiudo ed esco...");
            System.exit(3);
        }
    }

}//thread

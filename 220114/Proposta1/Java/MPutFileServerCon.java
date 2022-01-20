import java.io.*;
import java.net.*;

class PutFileServerThread extends Thread {
    private Socket clientSocket = null;

    public PutFileServerThread(Socket clientSocket) {
        this.clientSocket = clientSocket;
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
                int opRicevuta;
                String dirPath, line;
                Character ch;
                int occ, counter = 0, result = 0;
                BufferedReader br;
                File dirCorr;
                FileOutputStream outFileCorr;
                Long numeroByte;

                while ((opRicevuta = inSock.readInt()) == 1 || opRicevuta == 2) {
                    System.out.println("Ricevuta richiesta di operazione " +  opRicevuta + "\nAttendo argomenti...");
                    if (opRicevuta == 1) {
                            dirPath = inSock.readUTF();
                            dirCorr = new File(dirPath);
                            System.out.println("\n\nDirectory ricevuta: " +  dirPath);
                            ch = inSock.readChar();
                            System.out.println("Carattere ricevuto: " +  ch);
                            occ = inSock.readInt();
                            System.out.println("Numero occorrenze minime: " +  occ + "\n\n");

                            File files [] = dirCorr.listFiles();
                            for (File f: files) {
                                if (f.isFile() && f.getName().endsWith(".txt")) {
                                    System.out.println(f.getName() + " è un file di testo! Ora ne conto le occorrenze");
                                    br = new BufferedReader(new FileReader(f));
                                    while ((line = br.readLine()) != null) {
                                        counter = 0;
                                        if (Character.isUpperCase(line.charAt(0))) {
                                        for (int i = 0; i < line.length(); i++) {
                                            if (line.charAt(i) == ch)
                                                counter++;
                                            }
                                        }
                                        if (counter >= occ)
                                            result++;
                                    }
                                    br.close();
                                }
                            }
                        System.out.println("Numero occorrenze trovate all'interno della dir " + dirCorr + ": " + result
                        + "\nInvio risultato!");
                        outSock.writeInt(result);
                    } else {    // opRicevuta == 2
                            dirPath = inSock.readUTF();
                            dirCorr = new File(dirPath);
                            System.out.println("\n\nDirectory ricevuta: " +  dirPath);

                            File files [] = dirCorr.listFiles();
                            for (File f: files) {
                                if (f.isFile() && !f.getName().endsWith(".txt")) {
                                    System.out.println(f.getName() + " è un file binario!");
                                    System.out.println("Scrivo il file " + f.getName() + " di "+ f.length() + " byte");
                                    FileUtility.trasferisci_N_byte_file_binario(
                                        new DataInputStream(
                                                new FileInputStream(f.getAbsolutePath())),outSock, f.length());
                            }
                    }
                }
                    /*
                    fileCorr = new File(nomeFileRicevuto);
                    if (fileCorr.exists()) {
                        outSock.writeUTF("salta file");
                    } else {
                        outSock.writeUTF("attiva");
                        numeroByte = inSock.readLong();
                        System.out.println("Scrivo il file " + nomeFileRicevuto + " di "+ numeroByte + " byte");
                        outFileCorr = new FileOutputStream(nomeFileRicevuto);
                        FileUtility.trasferisci_N_byte_file_binario(inSock,
                                new DataOutputStream(outFileCorr), numeroByte);
                        outFileCorr.close();
                    }
                    */
                } // while
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


public class MPutFileServerCon {

    public static void main(String[] args) throws IOException {
        int port = -1;

        try {
            if (args.length == 1) {
                port = Integer.parseInt(args[0]);
                // controllo che la porta sia nel range consentito 1024-65535
                if (port < 1024 || port > 65535) {
                    System.out.println("Usage: java PutFileServerCon [serverPort>1024]");
                    System.exit(1);
                }
            } else {
                System.out.println("Usage: java PutFileServerCon port");
                System.exit(1);
            }
        }
        catch (Exception e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("Usage: java PutFileServerCon port");
            System.exit(1);
        }

        ServerSocket serverSocket = null;
        Socket clientSocket = null;

        try {
            serverSocket = new ServerSocket(port);
            serverSocket.setReuseAddress(true);
            System.out.println("PutFileServerCon: avviato ");
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
                    new PutFileServerThread(clientSocket).start();
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

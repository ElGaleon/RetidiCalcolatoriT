import java.net.*;
import java.io.*;

public class MPutFileClient {

    public static void main(String[] args) throws IOException {
        InetAddress addr = null;
        int port = -1;

        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
        String nomeFolder = null;
        String [] request; 
        Character ch;
        int occ;
        String op;
        int opReq;

        try {
            if (args.length == 2) {
                addr = InetAddress.getByName(args[0]);      // ottengo numero indirizzo
                port = Integer.parseInt(args[1]);           // ottengo numero porta
                if (port < 1024 || port > 65535) {
                    System.out.println("Usage: java MPutFileClient serverAddr serverPort");
                    System.exit(1);
                }
            } else {
                System.out.println("Usage: java MPutFileClient serverAddr serverPort");
                System.exit(1);
            }
        }
        catch (Exception e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("Usage: java MPutFileClient serverAddr serverPort");
            System.exit(2);
        }
        Socket socket = null;
        DataInputStream inSock = null;
        DataOutputStream outSock = null;

        try {
            socket = new Socket(addr, port);             // Creazione Socket
            socket.setSoTimeout(30000);
            System.out.println("Creata la socket: " + socket);
            inSock = new DataInputStream(socket.getInputStream());
            outSock = new DataOutputStream(socket.getOutputStream());
        } catch (IOException ioe) {
            System.out.println("Problemi nella creazione degli stream su socket: ");
            ioe.printStackTrace();
            System.out.print("\n^D(Unix)/^Z(Win)+invio per uscire, solo invio per continuare: ");
            System.exit(1);
        }

        System.out.print("\n^D(Unix)/^Z(Win)+invio per uscire, altrimenti immetti\n1)\tconta righe file di testo\n" +
        "2)\ttrasferimento file binari da cartella: ");
        while ((op = stdIn.readLine().trim()) != null) {
            opReq = Integer.parseInt(op);
            System.out.println("Operazione richiesta: " +  opReq);
            if (opReq == 1) {
                outSock.writeInt(1); // invio tipo di operazione
                try {
                    System.out.print("Inserisci nome direttorio: ");
                        if ((nomeFolder = stdIn.readLine()) != null) {
                            System.out.println("Nome direttorio inserito: " + nomeFolder);
                            File dirCorr = new File(nomeFolder);
                            if (dirCorr.exists() && dirCorr.isDirectory()) {
                                System.out.println("Invio path direttorio!");
                                outSock.writeUTF(dirCorr.getAbsolutePath());
                                System.out.println("Inserisci carattere da cercare e numero occorrenze minime separate da spazio: ");
                                    if ((request = stdIn.readLine().split(" ")) != null && request.length == 2) {
                                        ch = request[0].toCharArray()[0];
                                        outSock.writeChar(ch);
                                        occ = Integer.parseInt(request[1]);
                                        outSock.writeInt(occ);

                                        // Attesa risultato
                                        int result = inSock.readInt();
                                       if (result >= 0)
                                            System.out.println("Numero di occorrenze trovate nel direttorio " +  dirCorr.getName() + ": " +result);  
                                        }
                                    }
                            }else {
                                System.out.println("Direttorio insesistente");
                        } 
                    } catch(Exception e) {
                    System.out.println("Problemi nell'invio dei parametri\n");
                        e.printStackTrace();
                        System.exit(-2);
                }       
            } // if 1 - conta righe
            else if (opReq == 2) {
                outSock.writeInt(2);  // invio tipo di operazione
                try {
                    System.out.print("Inserisci nome direttorio: ");
                        if ((nomeFolder = stdIn.readLine()) != null) {
                            System.out.println("Nome direttorio inserito: " + nomeFolder);
                            File dirCorr = new File(nomeFolder);
                            if (dirCorr.exists() && dirCorr.isDirectory()) {
                                System.out.println("Invio path direttorio!");
                                outSock.writeUTF(dirCorr.getAbsolutePath());
                                // Attesa risultato
                                int result = inSock.readInt();
                                if (result >= 0)
                                System.out.println("Numero di occorrenze trovate nel direttorio " +  dirCorr.getName() + ": " +result);  
                            }else {
                                System.out.println("Direttorio insesistente");
                        } 
                        }
                    } catch(Exception e) {
                    System.out.println("Problemi nell'invio dei parametri\n");
                        e.printStackTrace();
                        System.exit(-2);
                }       
            } // if 2 - trasferimento file binari
            else {
                System.out.print("Operazione non disponibile!\n");
            }
            System.out.print("\n^D(Unix)/^Z(Win)+invio per uscire, altrimenti immetti\n1)\tconta righe" +
        "\n2)\ttrasferimento file binari da cartella: ");
        } // while 
        /*
        while ((nomeFolder = stdIn.readLine()) != null) {
            File dirCorr = new File(nomeFolder);
            if (dirCorr.exists() && dirCorr.isDirectory()) {
                File[] files = dirCorr.listFiles();
                for (int i = 0; i < files.length; i++) {
                    File fileCorr = files[i];
                    System.out.println("File con nome: "+ fileCorr.getName());
                    if (fileCorr.isFile() && dim_min<=fileCorr.length()) {
                        //I must send this file
                        outSock.writeUTF(fileCorr.getName());
                        String result = inSock.readUTF();
                        if (result.equals("attiva")){
                            System.out.println("Il file "+ fileCorr.getName()+ " NON e' presente sul server: inizio il trasferimento");
                            outSock.writeLong(fileCorr.length());
                            FileUtility.trasferisci_N_byte_file_binario(
                                    new DataInputStream(
                                            new FileInputStream(fileCorr.getAbsolutePath())),outSock, fileCorr.length());
                        }
                        else if (result.equals("salta file"))
                            System.out.println("Il file "+ fileCorr.getName()+ " era gia' presente sul server e non e' stato sovrascritto");
                        else {
                            System.out.println("MPutFileClient: violazione protocollo...");
                            System.exit(4);
                        }
                    } else System.out.println("File saltato");

                }
            }

            System.out.print("\n^D(Unix)/^Z(Win)+invio per uscire, altrimenti immetti il nome della cartella: ");
        }
        */

    }
}

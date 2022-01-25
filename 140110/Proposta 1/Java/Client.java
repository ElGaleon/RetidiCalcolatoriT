import java.net.*;
import java.io.*;

public class Client {

    public static void main(String[] args) throws IOException {
        InetAddress addr = null;
        int port = -1;

        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
        int op = 0, prezzo;
        int dim_min=0, dimResult;
        String tipo, luogo, buffIn;

        try {
            if (args.length == 2) {
                addr = InetAddress.getByName(args[0]);
                port = Integer.parseInt(args[1]);
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
            System.out.println("Usage: java MPutFileClient serverAddr serverPort minFileSize");
            System.exit(2);
        }
        Socket socket = null;
        DataInputStream inSock = null;
        DataOutputStream outSock = null;

        try {
            socket = new Socket(addr, port);
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

        System.out.println("\n^D(Unix)/^Z(Win)+invio per uscire, altrimenti immetti:\n" +
                        "1)Visualizza eventi in base a luogo e tipo\n" +
                        "2)Visualizza eventi in base a prezzo (min o uguale)");
        while ((op = Integer.parseInt(stdIn.readLine())) != 0) {
            if (op == 1) {
                // invio tipologia richiesta
                outSock.writeInt(1);
                System.out.println("Inserisci tipo evento: ");
                tipo = stdIn.readLine();
                System.out.println("Tipo evento: " + tipo);
                outSock.writeUTF(tipo);

                System.out.println("Inserisci luogo evento: ");
                luogo = stdIn.readLine();
                System.out.println("Luogo evento: " + luogo);
                outSock.writeUTF(luogo);

                // Ricezione dim risposta
                dimResult = inSock.readInt();

                if (dimResult == 0) {
                    System.out.println("Errore: nessun evento trovato con i parametri passati!");
                    continue;
                } else {
                    System.out.println("Ho trovato " + dimResult + " eventi");
                    System.out.println("Eventi trovati:");
                    for (int i = 0; i < dimResult; i++) {
                        for (int k = 0; k < 6; k++) {
                        buffIn = inSock.readUTF();
                        System.out.print(buffIn + "\t");
                        }
                        System.out.print("\n");
                    }
                }

            } else if (op == 2) {
                outSock.writeInt(2);
                System.out.println("Inserisci prezzo: ");
                prezzo = Integer.parseInt(stdIn.readLine());
                System.out.println("Prezzo evento: " + prezzo);
                outSock.writeUTF(Integer.toString(prezzo));


                // Ricezione dim risposta
                dimResult = inSock.readInt();

                if (dimResult == 0) {
                    System.out.println("Errore: nessun evento trovato con i parametri passati!");
                    continue;
                } else {
                    System.out.println("Eventi trovati:");
                    for (int i = 0; i < dimResult; i++) {
                        for (int k = 0; k < 6; k++) {
                        buffIn = inSock.readUTF();
                        System.out.print(buffIn + "\t");
                        }
                        System.out.print("\n");
                    }
                }
            } else {
                System.out.println("Operazione non disponibile!\n");
            }

            System.out.println("\n^D(Unix)/^Z(Win)+invio per uscire, altrimenti immetti:\n" +
            "1)Visualizza eventi in base a luogo e tipo\n" +
            "2)Visualizza eventi in base a prezzo (min o uguale)");
        }

    }
}

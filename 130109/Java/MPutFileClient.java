import java.net.*;
import java.io.*;

public class MPutFileClient {

    public static void main(String[] args) throws IOException {
        InetAddress addr = null;
        int port = -1;

        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
        String nomeFolder = null;
        int dim_min=0;

        try {
            if (args.length == 3) {
                addr = InetAddress.getByName(args[0]);
                port = Integer.parseInt(args[1]);
                if (port < 1024 || port > 65535) {
                    System.out.println("Usage: java MPutFileClient serverAddr serverPort minFileSize");
                    System.exit(1);
                }
                dim_min = Integer.parseInt(args[2]);
            } else {
                System.out.println("Usage: java MPutFileClient serverAddr serverPort minFileSize");
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

        System.out.print("\n^D(Unix)/^Z(Win)+invio per uscire, altrimenti immetti il nome della cartella: ");
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

    }
}

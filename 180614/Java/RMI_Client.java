/**
 * RMI_Client.java
 *
 */

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.NotBoundException;

class RMI_Client {

	public static void main(String[] args) {
		final int REGISTRYPORT = 1099;
		String registryHost = null; // host remoto con registry
		String serviceName = "";
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		// Controllo dei parametri della riga di comando
		if (args.length != 2) {
			System.out.println("Sintassi: RMI_Registry_IP ServiceName");
			System.exit(1);
		}
		registryHost = args[0];
		serviceName = args[1];

		System.out.println("Invio richieste a " + registryHost + " per il servizio di nome " + serviceName);

		// Connessione al servizio RMI remoto
		try {
			String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
			RMI_InterfaceFile serverRMI = (RMI_InterfaceFile) Naming.lookup(completeName);
			System.out.println("ClientRMI: Servizio \"" + serviceName + "\" connesso");

			System.out.println("\nRichieste di servizio fino a fine file");

			String service;
			System.out.print("Servizio (L=Lista file, N=Numerazione righe): ");

			/* ciclo accettazione richieste utente */
			while ((service = stdIn.readLine()) != null) {

				if (service.equals("L")) {
					String [] result;
					boolean ok = false; // stato [VALID|INVALID] della richiesta
					String directory = "";
					System.out.print("Nome directory: ");
					while (ok != true) {
						directory = stdIn.readLine();
						if (directory.isEmpty()) {
							System.out.println("Directory non valida");
							System.out.print("Nome directory: ");
							continue;
						} else
							ok = true; // gioranta inserita valida
					}
					ok = false;

					if ((result = serverRMI.lista_file(directory)) != null) {
						System.out.println("Lista dei file:");
						for (String fileName : result) {
							System.out.println(fileName);
						}
					}
					else
						System.out.println("Nessun file trovato ");
				} // L = Lista file

				else if (service.equals("N")) {
					String fileName = "";
					boolean ok = false;
					int res;
					System.out.print("Nome del file? ");

					while (ok != true) {
						fileName = stdIn.readLine();
						if (fileName.isEmpty()) {
							System.out.println("Filename non valido");
							System.out.print("Nome del file? ");
							continue;
						} else
							ok = true;
					}
					System.out.println("Ecco il programma: ");
					res = serverRMI.numerazione_righe(fileName);

					if (res < 0) {
						System.out.println("Errore: il file inserito non esiste o è una directory");
					} else {
						System.out.println("Numero righe contate: " + res);
					}
				} // P=Programma

				else
					System.out.println("Servizio non disponibile");

					System.out.print("Servizio (L=Lista file, N=Numerazione righe): ");
			} // while (!EOF), fine richieste utente

		} catch (NotBoundException nbe) {
			System.err.println("ClientRMI: il nome fornito non risulta registrato; " + nbe.getMessage());
			nbe.printStackTrace();
			System.exit(1);
		} catch (Exception e) {
			System.err.println("ClientRMI: " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
}
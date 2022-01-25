
/**
 * ClientCongresso.java
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
			System.out.println("Sintassi: RMI_Client ServiceName");
			System.exit(1);
		}
		registryHost = args[0];
		serviceName = args[1];

		System.out.println("Invio richieste a " + registryHost + " per il servizio di nome " + serviceName);

		// Connessione al servizio RMI remoto
		try {
			String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
			RMI_Server serverRMI = (RMI_Server) Naming.lookup(completeName);
			System.out.println("ClientRMI: Servizio \"" + serviceName + "\" connesso");

			System.out.println("\nRichieste di servizio fino a fine file");

			String service;
			System.out.print("Servizio (E=eliminazione, V=visualizza prenotazioni): ");

			/* ciclo accettazione richieste utente */
			while ((service = stdIn.readLine()) != null) {

				if (service.equals("E")) {
					boolean ok = false; // stato [VALID|INVALID] della richiesta
					String targa;
					System.out.print("Targa? ");
					while (ok != true) {
						targa= stdIn.readLine();
						if (targa.length() != 7) {
							System.out.println("Targa non valida");
							System.out.print("Targa? ");
							continue;
						} else
							ok = true; // gioranta inserita valida
					}
	
					if (serverRMI.elimina_prenotazione(targa))
						System.out.println(
								"Eliminazione " + speak + " effettuata con successo!");
					else
						System.out.println("Errore: targa non registrata!");
				} // E=Elimina

				else if (service.equals("V")) {
					String tipo;
					boolean ok = false;
					System.out.print("Tipo veicolo (auto o camper)? ");

					while (ok != true) {
						// TODO: check NumberFormatException
						tipo = stdIn.readLine();
						if (!tipo.equals("auto") && !tipo.equals("camper")) {
							System.out.println("Tipologia non valida!");
							System.out.print("Tipo veicolo (auto o camper)? ");
							continue;
						} else
							ok = true;
					}
					System.out.println("Ecco le prenotazioni: ");
					serverRMI.visualizza_prenotazioni(tipo);
				} // V=Visualizza

				else
					System.out.println("Servizio non disponibile");

				System.out.print("Servizio (R=Registrazione, P=Programma del congresso): ");
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
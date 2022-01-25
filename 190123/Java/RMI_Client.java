/**
 * Client.java
 * */

import java.rmi.*;
import java.io.*;

class RMI_Client {

	public static void main(String[] args) {
		int registryPort = 1099;
		String registryHost = null;
		String serviceName = "Server";
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		// Controllo parametri
		if (args.length != 1 && args.length != 2) {
			System.out.println("Sintassi: RMI_Client RegistryHost [registryPort]");
			System.exit(1);
		} else {
			registryHost = args[0];
			if (args.length == 2) {
				try {
					registryPort = Integer.parseInt(args[1]);
				} catch (Exception e) {
					System.out
					.println("Sintassi: RMI_Client NomeHost [registryPort], registryPort intero");
					System.exit(1);
				}
			}
		}

		// Connessione al servizio RMI remoto
		try {
			String completeName = "//" + registryHost + ":" + registryPort + "/"
					+ serviceName;
			RMI_InterfaceFile serverRMI = (RMI_InterfaceFile) Naming.lookup(completeName);
			System.out.println("Client RMI: Servizio \"" + serviceName
					+ "\" connesso");

			System.out.println("\nRichieste di servizio fino a fine file");

			String service;
			System.out.print("Servizio (E=elimina_prenotazione, V=visualizza_prenotazioni): ");

			while ((service = stdIn.readLine()) != null) {

				if (service.equals("E")) {

					String targa;
					System.out.print("Targa? ");
					targa = stdIn.readLine();

					// Invocazione remota
					try {
						int result = serverRMI.elimina_prenotazione(targa);
						if (result == 0)
							System.out.println("La prenotazione è stata eliminata con successo!");
						else {
							System.out.println("La targa indicata non esiste!");
						}
					} catch (RemoteException re) {
						System.out.println("Errore remoto: " + re.toString());
					}

				} // E

				else
					if (service.equals("V")) {

						String tipoVeicolo;
						System.out.print("Tipologia Veicolo (auto o camper)? ");
						tipoVeicolo = stdIn.readLine();

						while (!tipoVeicolo.equals("auto") && !tipoVeicolo.equals("camper")){
							System.out.print("Tipologia errata! Inserisci tipologia Veicolo (auto o camper): ");
						}

						Prenotazione [] res;
						try {
							res = serverRMI.visualizza_prenotazioni(tipoVeicolo);
							for (int i = 0; i < res.length; i++) {
								System.out.println(res[i]);
							}
						} catch (RemoteException re) {
							System.out.println("Errore remoto: " + re.toString());
						}
					} // S

					else System.out.println("Servizio non disponibile");

				System.out.print("Servizio (C=conta, E=Elimina): ");
			} // !EOF

		} catch (Exception e) {
			System.err.println("ClientRMI: " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
}
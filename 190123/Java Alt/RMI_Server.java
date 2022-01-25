
/**
 * ServerCongressoImpl.java
 * 		Implementazione del server
 * */

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RMI_Server extends UnicastRemoteObject implements RMI_InterfaceFile {
	Prenotazioni table;

	// Costruttore
	public RMI_Server() throws RemoteException {
		super();
	}

	// Elimina una prenotazione
	public synchronized int elimina_prenotazione(String targa) throws RemoteException {

		int result = -1;
		File fileToDelete;
		if (targa == null) {
			return result;
		}

		return this.elimina_prenotazione(targa);
	}

	/**
	 * @return --> riferimenti corrispondenti ad un nome logico, oppure false
	 */
	public String[][] visualizza_prenotazioni(String tipoVeicolo) throws RemoteException {
		int cont = 0;
		int size;
		if (tipoVeicolo == null) {
			return new String[0][0];
		}
		for (int i = 0; i < 50; i++) {
			if (tipoVeicolo.equals(table[i][3])
				&& ((this.table[i][0].charAt() == 'E' && table[i][0].charAt(1) >= 'D') 
				|| table[i][0].charAt(0) > 'E')) {
				cont++;
			}
		}
		size = (cont > 6) ? 6 : cont;
		String[][] risultato = new String[size][4];
		// Ora lo uso come indice per il riempimento
		cont = 0;
		for (int i = 0; i < 50 ; i++) {
			if (tipoVeicolo.equals(table[i][3])) {
				risultato[cont][0] = table[i][0];
				risultato[cont][1] = table[i][1];
				risultato[cont][2] = table[i][2];
				risultato[cont][3] = table[i][3];
				cont++;
			}
		}
		return risultato;
	}

	// Avvio del Server RMI
	public static void main(String[] args) {

		// creazione tabella
		table = new Prenotazioni();
		final int REGISTRYPORT = 1099;
		String registryHost = "localhost";
		String serviceName = "ServerCongresso"; // lookup name...

		// Registrazione del servizio RMI
		String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
		try {
			RMI_Server serverRMI = new RMI_Server();
			Naming.rebind(completeName, serverRMI);
			System.out.println("Server RMI: Servizio \"" + serviceName + "\" registrato");
		} catch (Exception e) {
			System.err.println("Server RMI \"" + serviceName + "\": " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
}
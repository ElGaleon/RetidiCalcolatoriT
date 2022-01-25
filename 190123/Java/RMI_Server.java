
/**
 * ServerImple.java
 * 	Implementa i servizi dichiarati in RemOp.
 * */

import java.io.*;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;



public class RMI_Server extends UnicastRemoteObject implements RMI_InterfaceFile {
		static int tableSize = 50;
		// Table [0: Targa Veicolo, 1: Numero Patente, 2: Tipologia Veicolo, 3: Folder]
		static Prenotazione[] table = new Prenotazione[tableSize];
		static int inizializzato = 0;

	// Costruttore
	public RMI_Server() throws RemoteException {
		super();

		
	}

	public static void inizializza() {

		if (inizializzato == 1) return;
		System.out.println("Inizializzo struttura dati");

		for (int i = 0; i < tableSize; i++) {
			table[i].setTarga("L");
			table[i].setPatente("0");
			table[i].setTipologia("L");
			table[i].setFolder("L");
		}

		// Inserimento dati
			table[0].setFolder("AN745NL");
			table[0].setPatente("00003");
			table[0].setTipologia("auto");
			table[0].setFolder(table[0].getTarga() + "_img");
			File dir = new File(table[0].getFolder());
			dir.mkdir();

			table[1].setFolder("FE547GF");
			table[1].setPatente("50006");
			table[1].setTipologia("camper");
			table[1].setFolder(table[1].getTarga() + "_img");
			dir = new File(table[1].getFolder());
			dir.mkdir();

			table[5].setFolder("NU547PL");
			table[5].setPatente("40063");
			table[5].setTipologia("auto");
			table[5].setFolder(table[5].getTarga() + "_img");
			dir = new File(table[5].getFolder());
			dir.mkdir();

			table[7].setFolder("LR897AH");
			table[7].setPatente("56832");
			table[7].setTipologia("camper");
			table[7].setFolder(table[7].getTarga() + "_img");
			dir = new File(table[7].getFolder());
			dir.mkdir();

			table[8].setFolder("MD506DW");
			table[8].setPatente("00100");
			table[8].setTipologia("camper");
			table[8].setFolder(table[8].getTarga() + "_img");
			dir = new File(table[8].getFolder());
			dir.mkdir();

			System.out.println("Struttura dati inizializzata");
			inizializzato = 1;
			stampa_tabella();
	}

	public synchronized int elimina_prenotazione(String targa) throws RemoteException {
		int result = -1;
		File fileToDelete;
		if (targa == null) {
			return result;
		}

		for (int i = 0; i < 50 && result < 0; i++) {
			if (targa.equals(table[i])) {
				// eliminazione
				fileToDelete = new File(table[i].getFolder());
				fileToDelete.delete();
				// pulizia da tabella
				table[i].setTarga("L");
				table[i].setPatente("0");
				table[i].setTipologia("L");
				table[i].setFolder("L");
				result = 0;			
			}
		}
		return result;
	}
	/**
	 * @return --> riferimenti corrispondenti ad un nome logico, oppure false
	 */
	public Prenotazione[] visualizza_prenotazioni(String tipoVeicolo) throws RemoteException {
		int cont = 0;
		int size;
		if (tipoVeicolo == null) {
			return new Prenotazione[0];
		}
		for (int i = 0; i < tableSize && i < 6; i++) {
			if (tipoVeicolo.equals(table[i].getTipologia())
				&& ((table[i].getTarga().charAt(0) == 'E' && table[i].getTarga().charAt(1) >= 'D') 
				|| table[i].getTarga().charAt(0) > 'E')) {
				cont++;
			}
		}
		size = (cont > 6) ? 6 : cont;
		Prenotazione[] risultato = new Prenotazione[size];
		// Ora lo uso come indice per il riempimento
		cont = 0;
		for (int i = 0; i < tableSize && i < 6; i++) {
			if (tipoVeicolo.equals(table[i].getTipologia())) {
				risultato[cont++] = table[i];
			}
		}
		return risultato;
	}

	public static void stampa_tabella() {
		System.out.println("TABELLA PRENOTAZIONI");
		System.out.println("TARGA VEICOLO\tPATENTE\tTIPO VEICOLO\tFOLDER");
		for (Prenotazione p: table) {
			System.out.println(p.getTarga()+"\t\t"+p.getPatente()+"\t\t"+
			p.getTipologia()+"\t\t"+p.getFolder());
		}
		System.out.println("------------------------------------------------------");
	}

	public static void main(String[] args) {

		int registryPort = 1099;
		String registryHost = "localhost";
		String serviceName = "Server";

		// Controllo parametri
		if (args.length != 0 && args.length != 1) {
			System.out.println("Sintassi: RMI_Server [registryPort]");
			System.exit(1);
		}
		if (args.length == 1) {
			try {
				registryPort = Integer.parseInt(args[0]);
			} catch (Exception e) {
				System.out.println("Sintassi: RMI_Server [registryPort], registryPort intero");
				System.exit(2);
			}
		}

		// Registrazione del servizio RMI
		String completeName = "//" + registryHost + ":" + registryPort + "/" + serviceName;
		try {
			RMI_Server serverRMI = new RMI_Server();
			Naming.rebind(completeName, serverRMI);
			System.out.println("Server RMI: Servizio \"" + serviceName + "\" registrato");
			//inizializza();
		} catch (Exception e) {
			System.err.println("Server RMI \"" + serviceName + "\": " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
}
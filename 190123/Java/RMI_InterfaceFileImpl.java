
/**
 * Implementazione del RegistryRemoto
 */

import java.io.File;
import java.rmi.Naming;
import java.rmi.RMISecurityManager;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;


public class RMI_InterfaceFileImpl extends UnicastRemoteObject implements
		RMI_InterfaceFile {

	final int tableSize = 50;
	private static final String[] VALID_TTIPOVEICOLO = {
			"auto",
			"camper",
	};

	// Table [0: Targa Veicolo, 1: Numero Patente, 2: Tipologia Veicolo, 3: Folder]
	Object[][] table = new Object[tableSize][4];
	
	public RMI_InterfaceFileImpl() throws RemoteException {
		super();
		for (int i = 0; i < tableSize; i++) {
			table[i][0] = "L";
			table[i][1] = "0";
			table[i][2] = "L";
			table[i][3] = "L";
		}

		// Inserimento dati
			table[0][0] = "AN745NL";
			table[0][1] = "00003";
			table[0][2] = "auto";
			table[0][3] = table[0][0] + "_img";
			File dir = new File((String) table[0][3]);
			dir.mkdir();

			table[1][0] = "FE457GF";
			table[1][1] = "50006";
			table[1][2] = "camper";
			table[1][3] = table[1][0] + "_img";
			dir = new File((String) table[1][3]);
			dir.mkdir();

			table[5][0] = "NU547PL";
			table[5][1] = "40063";
			table[5][2] = "auto";
			table[5][3] = table[5][0] + "_img";
			dir = new File((String) table[5][3]);
			dir.mkdir();

			table[7][0] = "LR897AH";
			table[7][1] = "56832";
			table[7][2] = "camper";
			table[7][3] = table[7][0] + "_img";
			dir = new File((String) table[7][3]);
			dir.mkdir();

			table[8][0] = "MD506DW";
			table[8][1] = "00100";
			table[8][2] = "camper";
			table[8][3] = table[8][0] + "_img";
			dir = new File((String) table[8][3]);
			dir.mkdir();
	}

	/**
	 * @return --> riferimento remoto cercato, oppure false
	 */
	public synchronized int elimina_prenotazione(String targa) throws RemoteException {
		int result = -1;
		File fileToDelete;
		if (targa == null) {
			return result;
		}

		for (int i = 0; i < 50 && result < 0; i++) {
			if (targa.equals(table[i][0])) {
				// eliminazione
				fileToDelete = new File((String) table[i][3]);
				fileToDelete.delete();
				// pulizia da tabella
				table[i][0] = "L";
				table[i][1] = "L";
				table[i][2] = "L";
				table[i][3] = "L";
				result = 0;			
			}
		}
		return result;
	}

	/**
	 * @return --> riferimenti corrispondenti ad un nome logico, oppure false
	 */
	public Remote[] visualizza_prenotazioni(String tipoVeicolo) throws RemoteException {
		int cont = 0;
		if (tipoVeicolo == null) {
			return new Remote[0];
		}
		for (int i = 0; i < tableSize && i < 6; i++) {
			if (tipoVeicolo.equals((String) table[i][0])) {
				cont++;
			}
		}
		Remote[] risultato = new Remote[cont];
		// Ora lo uso come indice per il riempimento
		cont = 0;
		for (int i = 0; i < tableSize && i < 6; i++) {
			if (tipoVeicolo.equals((String) table[i][0])) {
				risultato[cont++] = (Remote) table[i][1];
			}
		}
		return risultato;
	}

		// Avvio del Server RMI
		public static void main(String[] args) {

			int registryRemotoPort = 1099;
			String registryRemotoHost = "localhost";
			String registryRemotoName = "RegistryRemoto";
	
			// Controllo dei parametri della riga di comando
			if (args.length != 0 && args.length != 1) {
				System.out.println("Sintassi: ServerImpl [registryPort]");
				System.exit(1);
			}
			if (args.length == 1) {
				try {
					registryRemotoPort = Integer.parseInt(args[0]);
				} catch (Exception e) {
					System.out
							.println("Sintassi: ServerImpl [registryPort], registryPort intero");
					System.exit(2);
				}
			}
	
			// Impostazione del SecurityManager
			// if (System.getSecurityManager() == null)
			// System.setSecurityManager(new RMISecurityManager());
	
			// Registrazione del servizio RMI
			String completeName = "//" + registryRemotoHost + ":" + registryRemotoPort
					+ "/" + registryRemotoName;
			try {
				RMI_InterfaceFileImpl serverRMI = new RMI_InterfaceFileImpl();
				Naming.rebind(completeName, serverRMI);
				System.out.println("Server RMI: Servizio \"" + registryRemotoName
						+ "\" registrato");
			} catch (Exception e) {
				System.err.println("Server RMI \"" + registryRemotoName + "\": "
						+ e.getMessage());
				e.printStackTrace();
				System.exit(1);
			}
		}
}

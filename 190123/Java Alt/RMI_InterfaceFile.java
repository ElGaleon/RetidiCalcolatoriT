
/**
 * Interfaccia remota di servizio
 */

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI_InterfaceFile extends Remote {
	int max = 6;
	/**
	 * @param targa = targa che si vuole eliminare
	 * @throws RemoteException = FileNotFound, IOException
	 * @return risultato [0: successo | -1: errore]
	 **/
	public int elimina_prenotazione (String targa) throws RemoteException;

	/**
	 * @param tipologiaVeicolo = tipologia veicolo da cercare
	 * @throws RemoteException = = FileNotFound, IOException, numero riga troppo grande
	 * @return: le prime N (N <=6) di quella tipologia di veicolo
	 **/
	public String[][] visualizza_prenotazioni (String tipologiaVeicolo) throws RemoteException;
}
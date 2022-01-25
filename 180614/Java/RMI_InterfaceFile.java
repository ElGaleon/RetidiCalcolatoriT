
/**
 * Interfaccia remota di servizio
 */

import java.io.IOException;
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI_InterfaceFile extends Remote {

	String[] lista_file(String dirName) throws RemoteException;

	int numerazione_righe(String fileName) throws RemoteException, IOException;

}
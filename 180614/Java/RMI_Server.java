
/**
 * RMI_Server.java
 * 		Implementazione del server
 * */

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.rmi.Naming;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.regex.Pattern;

public class RMI_Server extends UnicastRemoteObject implements RMI_InterfaceFile {

	// Costruttore
	public RMI_Server() throws RemoteException {
		super();
	}

	@Override
	public String[] lista_file(String dirName) throws RemoteException {
		if (dirName == null)  {
			throw new RemoteException("Inviato parametro null");
		}
		File dir;
		String [] result = null;
		int dim = 0;
		dir = new File(dirName);
		String vowels =  "aeiouAEIOU";
		char [] name;
		boolean twoVowels;

		if (dir.exists() && dir.isDirectory()) {
			for (File subFile: dir.listFiles()) {
				if (subFile.isDirectory()) {
					System.out.println(subFile.getName() + " è una directory");
					for (File subSubFile: subFile.listFiles()) {
						if (subSubFile.isFile()) {
							twoVowels = false;
							System.out.println(subSubFile.getName() + " è un file");
							name = subSubFile.getName().toCharArray();
							for (int i = 0; i < name.length-1 && !twoVowels; i++) {
								if (	vowels.indexOf(name[i]) != -1 && vowels.indexOf(name[i+1]) != -1	)
										twoVowels = true;
							}
							if (twoVowels)
								dim++;
								
						}
					}
				} 
				else {
					System.out.println(subFile.getName() + " è un file");
					twoVowels = false;
					name = subFile.getName().toCharArray();
					for (int i = 0; i < name.length-1 && !twoVowels; i++) {
						if (	vowels.indexOf(name[i]) != -1 && vowels.indexOf(name[i+1]) != -1	)
								twoVowels = true;
						}
						if (twoVowels)
								dim++;	
					}
				}

				
		} else {
			throw new RemoteException("Il file indicato non è una directory!");
		}
		System.out.println("File trovati: " + dim);
		result = new String[dim];
		dim = 0;

		System.out.println("Riempimento struttura dati...");
		// Riempimento result
		for (File subFile: dir.listFiles()) {
			if (subFile.isDirectory()) {
				for (File subSubFile: subFile.listFiles()) {
					if (subSubFile.isFile()) {
						twoVowels = false;
						name = subSubFile.getName().toCharArray();
						for (int i = 0; i < name.length-1 && !twoVowels; i++) {
							if (	vowels.indexOf(name[i]) != -1 && vowels.indexOf(name[i+1]) != -1	)
									twoVowels = true;
						}
						if (twoVowels)
							result[dim++] = subSubFile.getName();
							
					}
				}
			} 
			else {
				twoVowels = false;
				name = subFile.getName().toCharArray();
				for (int i = 0; i < name.length-1 && !twoVowels; i++) {
					if (	vowels.indexOf(name[i]) != -1 && vowels.indexOf(name[i+1]) != -1	)
							twoVowels = true;
					}
					if (twoVowels)
					result[dim++] = subFile.getName();	
				}
			}
		System.out.println("Struttura dati riempita!\nInvio result\n");
		return result;
	}


	@Override
	public int numerazione_righe(String fileName) throws RemoteException, IOException {
		int result = 0;
		if (fileName == null) 
			return -1;
		
		File file = new File(fileName);
		File temp = new File("temp.txt");

		if (!file.exists() || !file.isFile()) 
			return -1;

		
		BufferedReader br = new BufferedReader(new FileReader(file));
		BufferedWriter bw = new BufferedWriter(new FileWriter(temp));
		String line;
		int lineNum = 0;

		while ((line = br.readLine()) != null) {
			bw.append(Integer.toString(++lineNum));
			bw.append(line + "\n");
		}

		br.close();
		bw.close();

		temp.renameTo(file);
		System.out.println("Operazione effettuata con successo");
		return lineNum;
	}

	
	// Avvio del Server RMI
	public static void main(String[] args) {

		final int REGISTRYPORT = 1099;
		String registryHost = "localhost";
		String serviceName = "RMIServer"; // lookup name...

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
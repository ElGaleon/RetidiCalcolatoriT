
/** 
 * Programma.java
 * 	Serializable -> deve essere restituita da un metodo
 * 		remoto.
 * 	Costruttore = inizializza (a "") tutto il programma.
 * 	Stampa = metodo della classe di appoggio per visualizzare il programma.
 * 	Registra = metodo per registrare un snome in una sessione. Ritorna 0 se OK.
 */

import java.io.*;

public class Prenotazioni implements Serializable {
	static int tableSize = 50;
	public static String table[][] = new String[50][4];

	public Prenotazioni() {
		for (int i = 0; i < tableSize; i++) {
				table[i][0] = "L";
				table[i][1] = "0";
				table[i][2] = "L";
				table[i][3] = "L";
		}

		// Inserimento dati
		table[0][1] =  "AN745NL";
		table[0][2] = "00003";
		table[0][3] = "auto";
		table[0][4] = table[0][0] + "_img";
		File dir = new File(table[0][4]);
		dir.mkdir();

		table[1][1] ="FE547GF";
		table[1][2] =("50006");
		table[1][3] = ("camper");
		table[1][4] = (table[1][0] + "_img");
		dir = new File(table[1][4]);
		dir.mkdir();

		table[5][0] = ("NU547PL");
		table[5][1] = ("40063");
		table[5][2] = ("auto");
		table[5][3] = (table[5][0] + "_img");
		dir = new File(table[5][4]);
		dir.mkdir();

		/*
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
		*/
		System.out.println("Struttura dati inizializzata");
		stampa_tabella();
	}

	public synchronized int elimina(String targa) {
		System.out.println("Programma: eliminazione di " + targa);
		File fileToDelete;
		int result = -1;

		for (int i = 0; i < tableSize && result < 0; i++) {
			if (targa.equals(table[i][0])) {
				// eliminazione
				fileToDelete = new File(table[i][4]);
				fileToDelete.delete();
				// pulizia da tabella
				table[i][0] = ("L");
				table[i][1] = ("0");
				table[i][2] =("L");
				table[i][3] = ("L");
				result = 0;			
			}
		}
		return result;
	}
	public void stampa_tabella() {
		System.out.println("TABELLA PRENOTAZIONI");
		System.out.println("TARGA VEICOLO\tPATENTE\tTIPO VEICOLO\tFOLDER");
		for (int i = 0; i < tableSize; i++) {
			System.out.println(table[i][0] + "\t\t" + table[i][1] + "\t\t" + table[i][2] + "\t\t" + table[i][3] + "\t\t" + table[i][4] + "\n");		
			}
		System.out.println("------------------------------------------------------");
	}
}
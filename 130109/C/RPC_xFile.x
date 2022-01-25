/* Christian Galeone - Matricola 924387 */

#define MAXS 256
#define N

struct Noleggio {
    String id;
    int giorno;
    int mese;
    int anno;
    int giorni;
    String modello;
    int costo;
};

struct Input{
    String id;
    String modello;
    int costo;
};

struct InputNoleggio{
    String id;
    int giorno;
    int mese; 
    int anno;
    int giorniNoleggio;
};

program NOLEGGIASCI {
    version NOLEGGIASCIVERSION {
        int INSERISCI_SCI(Input) = 1;
        int NOLEGGIA_SCI(InputNoleggio) = 2;
    } = 1;
} = 0x20000013;
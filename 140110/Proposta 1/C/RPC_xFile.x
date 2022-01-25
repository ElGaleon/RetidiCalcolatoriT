/* Christian Galeone - Matricola 924387 */

#define MAXS 256

struct Input{
    string descrizione<MAXS>;
    int numero_biglietti;
};

struct Evento{
    string descrizione<MAXS>;
    string tipo<MAXS>;
    string data<MAXS>;
    string luogo<MAXS>;
    int disponibilita;
    int prezzo;
};

program TICKETTWO {
    version TICKETTWOVERSION {
        int INSERIMENTO_EVENTO(Evento) = 1;
        int ACQUISTA_BIGLIETTI(Input) = 2;
    } = 1;
} = 0x20000013;
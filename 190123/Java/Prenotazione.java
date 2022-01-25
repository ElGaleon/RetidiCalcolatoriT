public class Prenotazione {
    private String targa;
    private String patente; 
    private String tipologia;
    private String folder;


    public Prenotazione(String targa, String patente, String tipologia, String folder) {
        this.targa = targa;
        this.patente = patente;
        this.tipologia = tipologia;
        this.folder = folder;
    }

    public String getTarga() {
        return this.targa;
    }

    public void setTarga(String targa) {
        this.targa = targa;
    }

    public String getPatente() {
        return this.patente;
    }

    public void setPatente(String patente) {
        this.patente = patente;
    }

    public String getTipologia() {
        return this.tipologia;
    }

    public void setTipologia(String tipologia) {
        this.tipologia = tipologia;
    }

    public String getFolder() {
        return this.folder;
    }

    public void setFolder(String folder) {
        this.folder = folder;
    }

    @Override
    public String toString() {
        return "{" +
            " targa='" + getTarga() + "'" +
            ", patente='" + getPatente() + "'" +
            ", tipologia='" + getTipologia() + "'" +
            ", folder='" + getFolder() + "'" +
            "}";
    }


}
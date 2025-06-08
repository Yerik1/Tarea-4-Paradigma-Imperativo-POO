package Model;
import java.util.List;

public class Data {
    private int[][] matriz;
    private int vidas1;
    private int vidas2;
    private int puntaje;
    private int juego;

    public Data() {
        this.matriz = new int[28][20]; // Java automáticamente la llena con ceros
        this.vidas1 = 3;
        this.vidas2 = 3;
        this.puntaje = 0;
        this.juego = 0;
    }

    // Getters y Setters
    public int[][] getMatriz() {
        return matriz;
    }

    public void setMatriz(int[][] matriz) {
        this.matriz = matriz;
    }

    public int getVidas1() {
        return vidas1;
    }

    public void setVidas1(int vidas1) {
        this.vidas1 = vidas1;
    }

    public int getJuego() {
        return juego;
    }

    public void setJuego(int juego) {
        this.juego = juego;
    }


    public int getVidas2() {
        return vidas2;
    }

    public void setVidas2(int vidas2) {
        this.vidas2 = vidas2;
    }

    public int getPuntaje() {
        return puntaje;
    }

    public void setPuntaje(int puntaje) {
        this.puntaje = puntaje;
    }
}

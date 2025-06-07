package controller;

import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

public class GameViewController {

    @FXML
    private GridPane gameGrid;

    @FXML
    public void initialize() {
        for (int fila = 0; fila < 20; fila++) {
            for (int columna = 0; columna < 28; columna++) {
                Rectangle celda = new Rectangle(25, 25, Color.LIGHTBLUE);
                celda.setStroke(Color.DARKGRAY); // Bordes visibles
                gameGrid.add(celda, columna, fila);
            }
        }
    }

    @FXML
    private Label labelPopo;

    @FXML
    private Label labelNana;

    public void actualizarDatos(int puntosPopo, int vidasPopo, int puntosNana, int vidasNana) {
        labelPopo.setText("Popo - Puntos: " + puntosPopo + " | Vidas: " + vidasPopo);
        labelNana.setText("Nana - Puntos: " + puntosNana + " | Vidas: " + vidasNana);
    }


}


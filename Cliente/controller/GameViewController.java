package controller;

import javafx.fxml.FXML;
import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

public class GameViewController {

    @FXML
    private GridPane gameGrid;
    @FXML
    private Label labelPopo;

    @FXML
    private Label labelNana;

    @FXML
    private Label labelPuntos;

    @FXML
    public void initialize() {
        for (int fila = 0; fila < 28; fila++) {
            for (int columna = 0; columna < 20; columna++) {
                Rectangle celda = new Rectangle(25, 25, Color.LIGHTBLUE);
                celda.setStroke(Color.DARKGRAY); // Bordes visibles
                gameGrid.add(celda, columna, fila);
            }
        }
    }



    public void actualizarDatos(int[][] matriz, int puntos, int vidasPopo, int vidasNana) {
        gameGrid.getChildren().removeIf(node ->
                node instanceof Label
        );

        for (int fila = 0; fila < matriz.length; fila++) {
            for (int col = 0; col < matriz[fila].length; col++) {
                Label celda = new Label("");

                if (matriz[fila][col] != 0) {
                    celda.setText(String.valueOf(matriz[fila][col]));
                }

                celda.setAlignment(Pos.CENTER); // Centrar texto en el Label

                gameGrid.add(celda, col, fila); // Agregar al GridPane
            }
        }
        labelPuntos.setText("Puntos: " + puntos);
        labelPopo.setText("Popo Vidas: " + vidasPopo);
        labelNana.setText("Nana Vidas: " + vidasNana);
    }


}


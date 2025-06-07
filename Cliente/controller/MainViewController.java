package controller;

import cliente.ClienteJava;
import javafx.fxml.FXML;
import javafx.event.ActionEvent;

public class MainViewController {
    // Aquí van los métodos y @FXML campos conectados al FXML
    @FXML
    private void manejarBoton(ActionEvent event) {
        ClienteJava.iniciarClienteDesdeGUI(); // Aquí se llama al cliente
    }
}


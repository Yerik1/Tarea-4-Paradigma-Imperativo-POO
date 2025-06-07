package controller;

import cliente.ClienteJava;
import javafx.fxml.FXML;
import javafx.event.ActionEvent;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.scene.Node;
import javafx.stage.Window;

public class MainViewController {
    // Aquí van los métodos y @FXML campos conectados al FXML
    @FXML
    private void manejarBoton(ActionEvent event) {
        ClienteJava.iniciarClienteDesdeGUI(); // Aquí se llama al cliente

        try {
            // Cargar la nueva ventana
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/GameView.fxml"));
            Parent root = loader.load();

            Stage stage = new Stage();
            stage.setScene(new Scene(root));
            stage.setTitle("Ventana de Juego");
            stage.show();

            // Cerrar la ventana actual
            Window ventanaActual = ((Node) event.getSource()).getScene().getWindow();
            ventanaActual.hide();

        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}


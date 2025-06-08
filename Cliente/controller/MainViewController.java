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
        try {
            // Cargar el FXML y obtener el controlador
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/GameView.fxml"));
            Parent root = loader.load();

            // Obtener el controlador después del load()
            GameViewController controladorJuego = loader.getController();

            // Iniciar el cliente de red y pasarle el controlador
            ClienteJava.iniciarClienteDesdeGUI(controladorJuego);

            // Mostrar la nueva ventana
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

    @FXML
    private void observarBoton1(ActionEvent event) {
        try {
            // Cargar el FXML y obtener el controlador
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/GameView.fxml"));
            Parent root = loader.load();

            // Obtener el controlador después del load()
            GameViewController controladorJuego = loader.getController();

            // Iniciar el cliente de red y pasarle el controlador
            ClienteJava.iniciarClienteSoloRecepcion(controladorJuego,1);

            // Mostrar la nueva ventana
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
    @FXML
    private void observarBoton2(ActionEvent event) {
        try {
            // Cargar el FXML y obtener el controlador
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/GameView.fxml"));
            Parent root = loader.load();

            // Obtener el controlador después del load()
            GameViewController controladorJuego = loader.getController();

            // Iniciar el cliente de red y pasarle el controlador
            ClienteJava.iniciarClienteSoloRecepcion(controladorJuego,0);

            // Mostrar la nueva ventana
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


//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
// ClienteJava.java
import java.io.*;
import java.net.*;

public class ClienteJava {
    public static void main(String[] args) {
        String servidor = "localhost";
        int puerto = 12345;

        try (Socket socket = new Socket(servidor, puerto);
             OutputStream salida = socket.getOutputStream();
             InputStream entrada = socket.getInputStream()) {

            // Enviar mensaje al servidor
            String mensaje = "derecha";
            salida.write(mensaje.getBytes());
            salida.flush();

            // Leer respuesta
            byte[] buffer = new byte[1024];
            int leido = entrada.read(buffer);
            String respuesta = new String(buffer, 0, leido);
            System.out.println("Respuesta del servidor: " + respuesta);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
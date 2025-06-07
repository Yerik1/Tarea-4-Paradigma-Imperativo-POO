//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
// ClienteJava.java
package cliente;

import java.io.*;
import java.net.*;
import java.util.Scanner;

public class ClienteJava {

    public static void iniciarClienteDesdeGUI() {
        new Thread(() -> main(null)).start(); // llama a main() en un hilo nuevo
    }

    public static void main(String[] args) {
        String servidor = "localhost";
        int puerto = 12345;

        try (Socket socket = new Socket(servidor, puerto);
             OutputStream salida = socket.getOutputStream();
             InputStream entrada = socket.getInputStream();
             Scanner scanner = new Scanner(System.in)) {

            while (true) {
                // Leer respuesta del servidor
                byte[] buffer = new byte[1024];
                int leido = entrada.read(buffer);
                if (leido == -1) {
                    System.out.println("Conexión cerrada por el servidor.");
                    break;
                }
                String respuesta = new String(buffer, 0, leido);

                System.out.println("Respuesta del servidor: " + respuesta);
                System.out.println("Conectado al servidor. Escribe un comando:");
                System.out.println("Ejemplos:");
                System.out.println("  Crear Juego");
                System.out.println("  Observar Juego 1");
                System.out.println("  Mover Jugador 1 derecha 1");
                System.out.println("  Cerrar Juego");
                System.out.println("Escribe 'salir' para terminar.\n");
                System.out.print(">> ");
                String mensaje = scanner.nextLine();

                if (mensaje.equalsIgnoreCase("salir")) break;

                // Enviar comando al servidor
                salida.write(mensaje.getBytes());
                salida.flush();



            }

        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("Cliente finalizado.");
    }
}

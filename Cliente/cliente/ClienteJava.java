//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
// ClienteJava.java
package cliente;

import java.io.*;
import java.net.*;
import java.util.Scanner;

import Model.Data;
import controller.GameViewController;
import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

public class ClienteJava {

    public static Data info = new Data();

    public static void iniciarClienteDesdeGUI(GameViewController controller) {
        new Thread(() -> main(controller)).start();


    }

    public static void main(GameViewController controller) {
        String servidor = "localhost";
        int puerto = 12345;

        try (Socket socket = new Socket(servidor, puerto);
             OutputStream salida = socket.getOutputStream();
             InputStream entrada = socket.getInputStream();
             Scanner scanner = new Scanner(System.in)) {

            // Enviar comando automático al conectar
            String comandoInicial = "Crear Juego";
            salida.write(comandoInicial.getBytes());
            salida.flush();

            Thread receptor = new Thread(() -> {
                try {
                    byte[] buffer = new byte[4096]; // Tamaño mayor por si la matriz es grande
                    while (true) {
                        int leido = entrada.read(buffer);
                        if (leido == -1) {
                            System.out.println("Conexión cerrada por el servidor.");
                            break;
                        }

                        String mensaje = new String(buffer, 0, leido);

                        if (mensaje.startsWith("{")) {
                            Gson gson = new Gson();
                            info = gson.fromJson(mensaje, Data.class);

                            //System.out.println("Puntaje: " + info.getPuntaje());
                            //System.out.println("Vidas: " + info.getVidas1() + ", " + info.getVidas2());

                            int[][] matriz = info.getMatriz();
                            for (int[] fila : matriz) {
                                for (Integer celda : fila) {
                                    //System.out.print(celda + " ");
                                }
                                //System.out.println();
                            }
                        }


                        //System.out.println("\n Actualización del servidor:\n" + mensaje);
                        //System.out.print(">> ");

                        javafx.application.Platform.runLater(() -> {
                            controller.actualizarDatos(info.getMatriz(), info.getPuntaje(), info.getVidas1(), info.getVidas2());
                        });
                    }
                } catch (IOException e) {
                    System.out.println("Error en el hilo receptor: " + e.getMessage());
                }
            });
            receptor.setDaemon(true); // Para que se cierre con el main
            receptor.start();

            // 🧾 Hilo principal: envía comandos
            while (true) {
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

                salida.write(mensaje.getBytes());
                salida.flush();
            }

        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("Cliente finalizado.");
    }

    public static void iniciarClienteSoloRecepcion(GameViewController controller, int juego) {
        new Thread(() -> mainSoloRecepcion(controller, juego)).start();
    }

    public static void mainSoloRecepcion(GameViewController controller, int juego) {
        String servidor = "localhost";
        int puerto = 12345;

        Socket socket = null;
        try {
            socket = new Socket(servidor, puerto);
            OutputStream salida = socket.getOutputStream();
            InputStream entrada = socket.getInputStream();

            // Enviar comando automático al conectar
            String comandoInicial = "Observar Juego " + juego;
            salida.write(comandoInicial.getBytes());
            salida.flush();

            Thread receptor = new Thread(() -> {
                try {
                    byte[] buffer = new byte[4096];
                    Gson gson = new Gson();
                    while (true) {
                        int leido = entrada.read(buffer);
                        if (leido == -1) {
                            System.out.println("Conexión cerrada por el servidor.");
                            break;
                        }

                        String mensaje = new String(buffer, 0, leido);
                        System.out.println("[DEBUG] Recibido: " + mensaje);

                        if (mensaje.startsWith("{")) {
                            Data info = gson.fromJson(mensaje, Data.class);

                            javafx.application.Platform.runLater(() -> {
                                controller.actualizarDatos(info.getMatriz(), info.getPuntaje(), info.getVidas1(), info.getVidas2());
                            });
                        } else {
                            System.out.println("[Servidor] " + mensaje);
                        }
                    }
                } catch (IOException e) {
                    System.out.println("Error en el hilo receptor: " + e.getMessage());
                }
            });
            receptor.setDaemon(true);
            receptor.start();

        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("Cliente en modo solo-recepción iniciado.");
    }
}

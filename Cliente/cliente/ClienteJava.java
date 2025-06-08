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
import javafx.scene.Scene;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.KeyCode;
import javafx.stage.Stage;

public class ClienteJava {

    public static Data info = new Data();

    public static void iniciarClienteDesdeGUI(GameViewController controller, Scene scene) {
        new Thread(() -> main(controller, scene)).start();


    }

    public static void main(GameViewController controller, Scene scene) {
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
                        }// 2. Si es texto: detectar y extraer número de juego
                        else if (mensaje.startsWith("Jugador asignado al juego")) {
                            try {
                                // Extraer número con split
                                String[] partes = mensaje.split(" ");
                                int numJuego = Integer.parseInt(partes[partes.length - 1].trim());
                                info.setJuego(numJuego);

                                System.out.println("Juego asignado: " + numJuego);
                            } catch (Exception e) {
                                System.out.println("Error al extraer número de juego: " + e.getMessage());
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
                scene.setOnKeyPressed(event -> {
                    if (salida == null) return;

                    String direccion = null;
                    String jugador = null;
                    switch (event.getCode()) {
                        case W:direccion = "arriba";jugador="1"; break;
                        case UP: direccion = "arriba";jugador="2"; break;
                        case S:  direccion = "abajo"; jugador="1"; break;
                        case DOWN:  direccion = "abajo";jugador="2"; break;
                        case A:direccion = "izquierda";jugador="1"; break;
                        case LEFT: direccion = "izquierda";jugador="2"; break;
                        case D:  direccion = "derecha"; jugador="1"; break;
                        case RIGHT:  direccion = "derecha";jugador="2"; break;

                    }

                    if (direccion != null) {
                        String comando = "Mover Jugador " + jugador+ " " + direccion + " " + info.getJuego()+1;
                        try {
                            salida.write(comando.getBytes());
                            salida.flush();
                            System.out.println("Comando enviado: " + comando);
                        } catch (IOException e) {
                            System.err.println("Error enviando comando: " + e.getMessage());
                        }
                    }
                });

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

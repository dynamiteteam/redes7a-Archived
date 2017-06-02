#include <SPI.h>
#include <Ethernet.h>
 
// Configuraci�n de direccion MAC e IP.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);
 
// Inicia la libreria Ethernet server con el puerto 80 (por defecto el puerto HTTP).
EthernetServer server(80);
 
void setup() {
// Inicia el puerto serie.
Serial.begin(9600);
 
// Inicia la conexi�n Ethernet y el servidor.
Ethernet.begin(mac, ip);
server.begin();
Serial.print("IP local del servidor ");
Serial.println(Ethernet.localIP());
}
 
void loop() {
EthernetClient client = server.available(); // Escucha a los clientes entrantes.
    if (client) { // Si un cliente se conecta al servidor:
    Serial.println("Nuevo cliente");
    boolean currentLineIsBlank = true; // Marcador para enviar la respuesta desde el servidor.
        while (client.connected()) { // Repite mientas existe clientes conectados:
            if (client.available()) {
            char c = client.read();
            Serial.write(c); // Imprime por el puerto serie la petici�n del cliente (caracter a caracter)
                if (c == '\n' && currentLineIsBlank) { // Se envia la respuesta a una petici�n de un cliente cuando a finalizado la petici�n:
                // Respuesta:
                client.println("HTTP/1.1 200 OK"); // Enviar un encabezado de respuesta HTTP est�ndar
                client.println("Content-Type: text/html");
                client.println("Connection: close"); // Se cerrar� la conexi�s despues de enviar la respuesta.
                client.println("Refresh: 5"); // Refrescar autom�ticamente la p�gina despu�s de 5 segundos.
                client.println();
                client.println("<!DOCTYPE HTML>"); // Tipo de documento.
                client.println("<html>"); // Etiqueta html inicio del documento. 
                client.print("Javier");
                client.print(millis()); // Tiempo de funcionamiento en ms.
                client.print("ms encendido. ");
                client.println("<br />"); // Etiqueta html salto de linea. 
                client.println("</html>"); // Etiqueta html fin del documento.
                break;
                }
                if (c == '\n') { // Si el caracter es un salto de linea:
                currentLineIsBlank = true; // La petici�n a terminado, se respondera a dicha petic�n en el sigueitne ciclo.
                } 
                else if (c != '\r') { // Si el caracter no es un retorno de carro (la petici�n no a terminado).
                currentLineIsBlank = false; // Seguimos escuchando la petici�n.
                }
           }
       }
    delay(1); // Espera para dar tiempo al navegador a recivir los datos.
    client.stop(); // Cierra la conexi�n.
    Serial.println("Cliente desconectado");
    Serial.println();
    }
}
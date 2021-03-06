/*
  Weather Monitoting Web Server
  blog.circuits4you.com
  www.circuits4you.com
 A Weather Monitoring web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Sensors Analog inputs attached to pins A0 through A3 (optional) 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>
//Define Sensor Connections
#define Temperature A0
#define Light      A1
#define Humidity   A2
#define Pressure   A3
// Variables sensor ultrasonico
long distancia2;
long tiempo2;
int estado = 0;
int retardo = 1000;
int pin = 8;
int stop_position = 95;
int velocity = 20;
int count2 = 0;
#define Trigger 4
# define echo 5
Servo servo;
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

String HTTP_req;          // stores the HTTP request
boolean LED_status = 0;   // state of LED, off by default
int count=0;
int T[20],L[20],H[20],P[20];
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
     pinMode(13, OUTPUT);       // LED on pin 2    
     
  }
pinMode(Trigger,OUTPUT); // ultrasonic sensor
      pinMode(echo,INPUT); // ultrasonic sensor
servo.attach(pin);  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
void loop() {
      motor();    
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        HTTP_req += c;  //Save the HTTP request 1 char at a time
//        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
                    client.println("<HTML>");
                    client.println(" <HEAD>");
                    client.println("  <TITLE> Banda Transportadora </TITLE>");
                    client.println("</head>");
                    client.println("<style>");
                    client.println("table {");
                    client.println("    width:70%;");
                    client.println("}");
                    client.println("table, th, td {");
                    client.println("    border: 1px solid black;");
                    client.println("    border-collapse: collapse;");
                    client.println("}");
                    client.println(" th, td {");
                    client.println("    padding: 5px;");
                    client.println("    text-align: left;");
                    client.println("}");
                    client.println("table#t01 tr:nth-child(even) {");
                    client.println("    background-color: #eee;");
                    client.println("}");

                    client.println("table#t01 tr:nth-child(odd) {");
                    client.println("   background-color:#fff;");
                    client.println("}");
                    client.println("table#t01 th {");
                    client.println("    background-color: #00A8A9;");
                    client.println("    color: white;");
                    client.println("}");
                    client.println("</style>");
                    client.println(" </HEAD>");
                    client.println("<body>");
                    client.println("<br>");
                    client.println("<H1><center> <BR> Conveyer Belt </center></h1>");
                    client.println("<center>");
                    client.println("<H2>Current Status</H2>");
                    client.println("<table id=""t01"">");
                    client.println("                    <tr>");
                    client.println("      <th>Data. No</th>");
                    client.println("      <th> Distancia </th>");
                    client.println("      <th>Cantidad</th>");
                    client.println("      <th>Sensor1 </th>");
                    client.println("      <th>Sensor2 </th>");
                    client.println(" </tr>");
                    client.println(" <tr>");

                    if(distancia2 < 7)
                    {
                      count2++;
                    }

                
                    Serial.println(String(distancia2));
                    client.print("                   <td>"); client.print(count); client.print("</td>");
                    client.print("                   <td>"); client.print(distancia2); client.print(" cms</td>");
                    client.print("                   <td>"); client.print(count2); client.print(" </td>");
                    client.print("                   <td>"); client.print(analogRead(Humidity)); client.print(" </td>");
                    client.print("                   <td>"); client.print(analogRead(Pressure)); client.print(" </td>");

                 
                    T[count]= distancia2;
                    L[count]=analogRead(Light);
                    H[count]=analogRead(Humidity);
                    P[count]=analogRead(Pressure);
                    
                    client.println("  </tr>");
                    client.println(" </table>");
                    client.println("<BR>");
                    client.println("<H2>Data Logging</H2>");
                    
                    client.println("<table id=""t01"">");
                    client.println("                    <tr>");
                    client.println("      <th>Sr. No</th>");
                    client.println("      <th>Distancia</th>");
                    client.println("      <th>Sensor 1</th>");
                    client.println("      <th>Sensor 2</th>");
                    client.println("      <th>Sensor 3</th> ");
                    client.println(" </tr>");
             for(int i=0;i<count;i++)
         {
                    client.println(" <tr>");
                    client.print("                   <td>"); client.print(i+1); client.print("</td>");
                    client.print("                   <td>"); client.print(T[i]); client.print(" cms</td>");
                    client.print("                   <td>"); client.print(L[i]); client.print(" </td>");
                    client.print("                   <td>"); client.print(H[i]); client.print(" </td>");
                    client.print("                   <td>"); client.print(P[i]); client.print(" </td>");
                    client.println("  </tr>");
        }
        count++;
        if(count==20){count=0;}//Make count 0 max data logging is 20 values
                    client.println(" </table>");
                    client.println(" </center>");
                    client.println(" </body>");
                    client.println(" </html>");          Serial.print(HTTP_req);
          HTTP_req = "";  //finishehed with request, empty string
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
 
    client.stop();
    Serial.println("client disconnected");
    
  }
  ultrasonic();

}


void ultrasonic()
{
  digitalWrite(4,LOW);
    delayMicroseconds(5);
  digitalWrite(4,HIGH);
  delayMicroseconds(6);
tiempo2 = pulseIn(5,HIGH);
   distancia2 = int(0.017*tiempo2);
  delay(500);
  Serial.println(String(distancia2));
}

void motor()
{
    servo.write(stop_position - velocity);
    delay(15);
     servo.write(stop_position + velocity);
    delay(15);
    
 
}












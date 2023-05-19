const char* ssid = "ESP32-Access-Point";
const char* password = "password";
WiFiServer server(80);


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Setting up access point...");

  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  arm.begin(basePin, shoulderPin, elbowPin);
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        request.trim();

        if (request.startsWith("GET /?message=")) {
          String receivedMessage = request.substring(14);
          receivedMessage.replace("%20", " ");
          String mode = getNextToken(receivedMessage);

          if(mode == "CANVAS") {
            parseCanvas(receivedMessage);
          } else if (mode == "JOYSTICK") {
            parseJoystick(receivedMessage);
          } else if (mode == "TEST") {
            Serial.println();
            Serial.print("Received TEST message.");
          } else {
            Serial.println();
            Serial.print("Received message: " + receivedMessage);
          }

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<body>");
          if (mode == "TEST") {
            client.println("<h1>Connection Test Success</h1>");
          } else {
                  client.println("<h1>Message Received</h1>");
            }
            client.println("</body>");
            client.println("</html>");
      
            break;
          } else {
            client.println("HTTP/1.1 404 Not Found");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<body>");
            client.println("<h1>Page Not Found</h1>");
            client.println("</body>");
            client.println("</html>");
      
            break;
          }
        }
      }
      
      client.stop();
    }
  }

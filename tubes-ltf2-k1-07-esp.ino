#include <WiFi.h>
#include <math.h>

const char* ssid = "ESP32-Access-Point";
const char* password = "password";
WiFiServer server(80);

void printCanvasCommand(const String& mode, const String& command, float pos_x, float pos_y, float delta_z) {
    Serial.println();
    Serial.print("mode: ");
    Serial.print(mode);
    Serial.print(" command: ");
    Serial.print(command);
    Serial.print(" pos_x: ");
    Serial.print(pos_x);
    Serial.print(" pos_y: ");
    Serial.print(pos_y);
    Serial.print(" delta_z: ");
    Serial.print(delta_z);
}

void printJoystickCommand(const String& mode, float delta_x, float delta_y, float delta_z) {
    Serial.println();
    Serial.print("mode: ");
    Serial.print(mode);
    Serial.print(" delta_x: ");
    Serial.print(delta_x);
    Serial.print(" delta_y: ");
    Serial.print(delta_y);
    Serial.print(" delta_z: ");
    Serial.print(delta_z);
}

String getNextToken(String& receivedMessage) {
    String token = receivedMessage.substring(0, receivedMessage.indexOf(" "));
    receivedMessage.remove(0, receivedMessage.indexOf(" ") + 1);
    return token;
}

void parseCanvas(String receivedMessage) {
    String command = getNextToken(receivedMessage);
    float pos_x = getNextToken(receivedMessage).toFloat();
    float pos_y = getNextToken(receivedMessage).toFloat();
    float delta_z = getNextToken(receivedMessage).toFloat();

    printCanvasCommand("CANVAS", command, pos_x, pos_y, delta_z);
    if (command == "MOVE_XY") {
//      goDirectlyTo(pos_x, pos_y, arm.getZ());
    }
    else if (command == "MOVE_Z") {
//      goDirectlyTo(arm.getX(), arm.getY(), arm.getZ() + delta_z);
    }

     else if (command == "PEN_UP") {
//      goDirectlyTo(arm.getX(), arm.getY(), arm.getZ() + delta_z);
    }

     else if (command == "PEN_DOWN") {
//      goDirectlyTo(arm.getX(), arm.getY(), arm.getZ() - delta_z);
    }
}

void parseJoystick(String receivedMessage) {
    float delta_x = getNextToken(receivedMessage).toFloat();
    float delta_y = getNextToken(receivedMessage).toFloat();
    float delta_z = getNextToken(receivedMessage).toFloat();

    printJoystickCommand("JOYSTICK", delta_x, delta_y, delta_z);
    //   goDirectlyTo(arm.getX() + delta_x, arm.getY() + delta_y, arm.getZ() + delta_z);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Setting up access point...");

  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

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

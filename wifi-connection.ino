#include <WiFi.h>
#include <math.h>

const char* ssid = "ESP32-Access-Point";
const char* password = "password";
WiFiServer server(80);

// BUTUH KALIBRASI
// A. CANVAS VARIABLE
const float POS_THRESHOLD = 40.0;
float prev_pos_x = 0.0;
float prev_pos_y = 0.0;

// B. ANALOG CONTROL VARIABLE
const float MULT_X = 10.0;
const float MULT_Y = 10.0;
const float MULT_Z = 10.0;

// C. DIGITAL CONTROL VARIABLE
const float STEP_X = 20.0;
const float STEP_Y = 20.0;
const float STEP_Z = 20.0;


void printCanvasCommand(String mode, String command, float pos_x, float pos_y, float delta_z) {
    Serial.println();
    Serial.print("mode: ");
    Serial.print(mode);
    Serial.print(" command: ");
    Serial.print(command);
    Serial.print(" pos_x: ");
    Serial.print(prev_pos_x);
    Serial.print(" pos_y: ");
    Serial.print(prev_pos_y);
    Serial.print(" delta_z: ");
    Serial.print(delta_z);
}

void printJoystickCommand(String mode, float delta_x, float delta_y, float delta_z) {
    Serial.print("mode: ");
    Serial.print(mode);
    Serial.print(" delta_x: ");
    Serial.print(delta_x);
    Serial.print(" delta_y: ");
    Serial.print(delta_y);
    Serial.print(" delta_z: ");
    Serial.print(delta_z);
}

void parseCanvas(String receivedMessage) {
    receivedMessage.remove(0, receivedMessage.indexOf(" ") + 1);
    String command = receivedMessage.substring(0, receivedMessage.indexOf(" "));
    receivedMessage.remove(0, receivedMessage.indexOf(" ") + 1);
    float pos_x = receivedMessage.substring(0, receivedMessage.indexOf(" ")).toFloat();
    receivedMessage.remove(0, receivedMessage.indexOf(" ") + 1);
    float pos_y = receivedMessage.substring(0, receivedMessage.indexOf(" ")).toFloat();
    receivedMessage.remove(0, receivedMessage.indexOf(" ") + 1);
    float delta_z = receivedMessage.substring(0, receivedMessage.indexOf(" ")).toFloat();
    

    // Check if pos_x has changed significantly
    if (command == "MOVE_XY" && sqrt(pow((pos_x - prev_pos_x), 2) + pow((pos_y - prev_pos_y), 2)) > POS_THRESHOLD) {
      prev_pos_x = pos_x;
      prev_pos_y = pos_y;
      printCanvasCommand("CANVAS", command, prev_pos_x, prev_pos_y, delta_z);
//      goDirectlyTo(prev_pos_x, prev_pos_y, arm.getZ());
    }
    else if (command == "MOVE_Z") {
      printCanvasCommand("CANVAS", command, pos_x, pos_y, delta_z);
//      goDirectlyTo(arm.getX(), arm.getY(), arm.getZ() + delta_z*STEP_Z);
    }

     else if (command == "PEN_UP") {
      printCanvasCommand("CANVAS", command, pos_x, pos_y, delta_z);
//      goDirectlyTo(arm.getX(), arm.getY(), arm.getZ() + STEP_Z);
    }

     else if (command == "PEN_DOWN") {
      printCanvasCommand("CANVAS", command, pos_x, pos_y, delta_z);
//      goDirectlyTo(arm.getX(), arm.getY(), arm.getZ() - STEP_Z);
    }
//    else {
//      Serial.println("No Update");
//    }
}


void parseJoystick(String receivedMessage) {
    receivedMessage.remove(0, receivedMessage.indexOf(" ") + 1);
    float delta_x = receivedMessage.substring(0, receivedMessage.indexOf(" ")).toFloat();
    receivedMessage.remove(0, receivedMessage.indexOf(" ") + 1);
    float delta_y = receivedMessage.substring(0, receivedMessage.indexOf(" ")).toFloat();
    receivedMessage.remove(0, receivedMessage.indexOf(" ") + 1);
    float delta_z = receivedMessage.substring(0, receivedMessage.indexOf(" ")).toFloat();
    
   printJoystickCommand("JOYSTICK", delta_x, delta_y, delta_z);
//   goDirectlyTo(arm.getX() + delta_x*MULT_X, arm.getY() + delta_x*MULT_Y, arm.getZ() + delta_z*MULT_Z);
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
//          Serial.println("Received message: " + receivedMessage);

          // Parse the message
          String mode = receivedMessage.substring(0, receivedMessage.indexOf(" "));
          

          if(mode == "CANVAS") {
            parseCanvas(receivedMessage);
          } else if (mode == "JOYSTICK") {
            parseJoystick(receivedMessage);
          } else {
            Serial.println("Received message: " + receivedMessage);
          }

          // Send the response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<body>");
          client.println("<h1>Message Received</h1>");
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
//    Serial.println("Client disconnected.");
  }
}

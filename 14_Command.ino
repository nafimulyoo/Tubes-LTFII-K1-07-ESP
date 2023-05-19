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
      arm.goDirectlyTo(pos_x, pos_y, arm.getZ());
    }
    else if (command == "MOVE_Z") {
      arm.goDirectlyTo(arm.getX(), arm.getY(), arm.getZ() + delta_z);
    }

     else if (command == "PEN_UP") {
      arm.goDirectlyTo(arm.getX(), arm.getY(), arm.getZ() + delta_z);
    }

     else if (command == "PEN_DOWN") {
      arm.goDirectlyTo(arm.getX(), arm.getY(), arm.getZ() - delta_z);
    }
}

void parseJoystick(String receivedMessage) {
    float delta_x = getNextToken(receivedMessage).toFloat();
    float delta_y = getNextToken(receivedMessage).toFloat();
    float delta_z = getNextToken(receivedMessage).toFloat();

    printJoystickCommand("JOYSTICK", delta_x, delta_y, delta_z);
    arm.goDirectlyTo(arm.getX() + delta_x, arm.getY() + delta_y, arm.getZ() + delta_z);
}

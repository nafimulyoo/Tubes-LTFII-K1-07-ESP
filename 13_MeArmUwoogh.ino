/*"UWOGH UWOGH KODE...... KODE ❤ ❤ ❤ WANGY WANGY WANGY WANGY WANGY WANGY HU HA HU HA HU HA, aaaah baunya ARDUINO wangi aku mau nyiumin aroma wanginya KODE ARDUINO"*/

void meArm::begin(int pinBase, int pinShoulder, int pinElbow) {
  _pinBase = pinBase;
  _pinShoulder = pinShoulder;
  _pinElbow = pinElbow;
  _base = Stepper(STEPS_PER_REVOLUTION, STEP_PIN, DIR_PIN);
  _shoulder.attach(_pinShoulder);
  _elbow.attach(_pinElbow);
}

void meArm::end() {
  digitalWrite(ENABLE_PIN, HIGH); // Disable motor driver
  _shoulder.detach();
  _elbow.detach();
}

// Send the motors to the direction you want to go
void meArm::goDirectlyTo(float x, float y, float z) {
  float radBase, radShoulder, radElbow;
  if (solve(x, y, z, radBase, radShoulder, radElbow)) {
    // Code specific to your stepper motor driver and library goes here
    // For example, if using the AccelStepper library, you can call _base.moveTo(radBase);
    _shoulder.write(radShoulder);
    _elbow.write(radElbow);
    _x = x; _y = y; _z = z;
  }
}


//Travel smoothly from current point to another point
void meArm::gotoPoint(float x, float y, float z) {
  //Starting points - current pos
  float x0 = _x; 
  float y0 = _y; 
  float z0 = _z;
  float dist = sqrt((x0-x)*(x0-x)+(y0-y)*(y0-y)+(z0-z)*(z0-z));
  int step = 10;
  for (int i = 0; i<dist; i+= step) {
    goDirectlyTo(x0 + (x-x0)*i/dist, y0 + (y-y0) * i/dist, z0 + (z-z0) * i/dist);
    delay(50);
  }
  goDirectlyTo(x, y, z);
  delay(50);
}


//Check to see if possible
bool meArm::isReachable(float x, float y, float z) {
  float radBase,radShoulder,radElbow;
  return (solve(x, y, z, radBase, radShoulder, radElbow));
}

//Current x, y and z
float meArm::getX() {
  return _x;
}
float meArm::getY() {
  return _y;
}
float meArm::getZ() {
  return _z;
}

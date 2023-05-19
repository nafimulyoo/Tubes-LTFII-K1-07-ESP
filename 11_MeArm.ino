class meArm {
  public:
    meArm() : _base(200, 2, 3) {} // Assuming 200 steps per revolution, STEP_PIN = 2, DIR_PIN = 3
    void begin(int pinBase, int pinShoulder, int pinElbow);
    void end();
    //Travel smoothly from current point to another point
    void gotoPoint(float x, float y, float z);
    //Set servos to reach a certain point directly without caring how we get there 
    void goDirectlyTo(float x, float y, float z);

    //Check to see if possible
    bool isReachable(float x, float y, float z);
    //Current x, y and z
    float getX();
    float getY();
    float getZ();

  private:
    float _x, _y, _z;
    int _pinBase, _pinShoulder, _pinElbow;
    Servo _shoulder, _elbow;
    Stepper _base;
};


meArm arm;

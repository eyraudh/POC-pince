#include <Servo.h>
#include <Stepper.h>

#define RGT_SERVO_CLOSE_ANGLE 80
#define RGT_SERVO_REST_ANGLE 140
#define RGT_SERVO_OPEN_ANGLE 200
#define LFT_SERVO_CLOSE_ANGLE 90
#define LFT_SERVO_REST_ANGLE 44
#define LFT_SERVO_OPEN_ANGLE 2
#define ARM1_SERVO_HIGH_ANGLE 145
#define ARM1_SERVO_REST_ANGLE 145
#define ARM1_SERVO_LOW_ANGLE 50
#define ARM2_SERVO_LOW_ANGLE 40
#define ARM2_SERVO_REST_ANGLE 190
#define ARM2_SERVO_HIGH_ANGLE 180
#define CLAMP_SERVO_LOW_ANGLE 60
#define CLAMP_SERVO_REST_ANGLE 120
#define CLAMP_SERVO_HIGH_ANGLE 270

const int stepsPerRevolution = 200;

#define pwmA 3
#define pwmB 11
#define brakeA 9
#define brakeB 8
#define dirA 12
#define dirB 13

inline float map(float val, float min, float max, float minOut, float maxOut) {
  return val / (max-min) * (maxOut-minOut) + minOut;
}

Stepper base = Stepper(stepsPerRevolution, dirA, dirB);

Servo handRightServo;  // create servo object to control a servo
Servo handLeftServo;
Servo fstArmServo;
Servo sndArmServo;
Servo clampServo;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  handRightServo.attach(6);  // attaches the servo on pin 9 to the servo object
  handLeftServo.attach(7);
  fstArmServo.attach(5);
  sndArmServo.attach(2);
  clampServo.attach(4);

  // Set the PWM and brake pins so that the direction pins can be used to control the motor:
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(brakeA, OUTPUT);
  pinMode(brakeB, OUTPUT);

  digitalWrite(pwmA, HIGH);
  digitalWrite(pwmB, HIGH);
  digitalWrite(brakeA, LOW);
  digitalWrite(brakeB, LOW);

  // Set the motor speed (RPMs):
  base.setSpeed(20);

  fstArmServo.write(ARM1_SERVO_REST_ANGLE);
  sndArmServo.write(ARM2_SERVO_REST_ANGLE);
  clampServo.write(CLAMP_SERVO_REST_ANGLE);
}

void loop() {
  delay(2000);
  float a = ARM1_SERVO_HIGH_ANGLE;
  for (a = ARM2_SERVO_HIGH_ANGLE; a > ARM2_SERVO_LOW_ANGLE; a -= 0.1)
  {
    sndArmServo.write(floor(a));
    delay(1);
  }
  for (a = ARM1_SERVO_HIGH_ANGLE; a > ARM1_SERVO_LOW_ANGLE; a -= 0.1)
  {
    fstArmServo.write(floor(a));
    delay(1);
  }
  delay(2000);
  for (a = ARM2_SERVO_LOW_ANGLE ; a < ARM2_SERVO_HIGH_ANGLE; a += 0.1)
  {
    sndArmServo.write(floor(a));
    delay(1);
  }
  for (a = ARM1_SERVO_LOW_ANGLE ; a <ARM1_SERVO_HIGH_ANGLE; a += 0.1)
  {
    fstArmServo.write(floor(a));
    delay(1);
  }
  

  // handRightServo.write(RGT_SERVO_OPEN_ANGLE);
  // handLeftServo.write(LFT_SERVO_OPEN_ANGLE);
  // firstArmServo.write(ARM1_SERVO_HIGH_ANGLE);
  // sndArmServo.write(ARM2_SERVO_HIGH_ANGLE);
  // base.step(200);
  // delay(5000);
  // handRightServo.write(RGT_SERVO_CLOSE_ANGLE);
  // handLeftServo.write(RGT_SERVO_OPEN_ANGLE);
  // firstArmServo.write(ARM1_SERVO_LOW_ANGLE);
  // base.step(-200);
  // sndArmServo.write(ARM2_SERVO_LOW_ANGLE);
  // delay(5000);
}

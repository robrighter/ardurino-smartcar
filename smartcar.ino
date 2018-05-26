//This demo is used for testing the Motor module.
#define leftFactor 10
#define rightFactor 5
#define speedSet  150

#include <UCMotor.h>
#include <AFArrayType.h>
UC_DCMotor leftMotor(3, MOTOR34_64KHZ);
UC_DCMotor rightMotor(4, MOTOR34_64KHZ);
char buffer[80];


#define TRIG_PIN A2
#define ECHO_PIN A3
void setup()
{
  Serial.begin(115200);
  pinMode(ECHO_PIN, INPUT); //Set the connection pin output mode Echo pin
  pinMode(TRIG_PIN, OUTPUT);//Set the connection pin output mode trog pin
}

void loop(){
  //read the distance in front of us
  int distanceInFrontOfUs = readPingSampled();
  //if its more than 20cm move forward, otherwise turn left
  sprintf(buffer, "Distance = %d", distanceInFrontOfUs); 
  Serial.println(buffer);
  if(distanceInFrontOfUs > 20){
    Serial.println("Moving Forward");
    moveForward();
  }
  else{
    Serial.println("Turning Left");
    turnLeft();
  }
  delay(1000);
  moveStop();
}


void moveForward() {
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
  leftMotor.setSpeed(speedSet + leftFactor);
  rightMotor.setSpeed(speedSet + rightFactor);
}
void turnLeft() {
  leftMotor.run(BACKWARD);
  rightMotor.run(FORWARD);
  leftMotor.setSpeed(speedSet + leftFactor);
  rightMotor.setSpeed(speedSet + rightFactor);
  delay(400);
  moveStop();
}
void turnRight() {
  leftMotor.run(FORWARD);
  rightMotor.run(BACKWARD);
  leftMotor.setSpeed(speedSet + leftFactor);
  rightMotor.setSpeed(speedSet + rightFactor);
  delay(400);
  moveStop();
}
void moveBackward() {
  leftMotor.run(BACKWARD);
  rightMotor.run(BACKWARD);
  leftMotor.setSpeed(speedSet + leftFactor);
  rightMotor.setSpeed(speedSet + rightFactor);
}
void moveStop() {
  leftMotor.run(RELEASE); rightMotor.run(RELEASE);
}

int readPing()
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, cm;
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  //pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, LOW);

  //pinMode(ECHO_PIN, INPUT);
  duration = pulseIn(ECHO_PIN, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  return cm;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

int readPingSampled(){
  AFAInt samples;
  byte numSamples = 20;
  for(int i=0; i<numSamples; i++){
    samples.add(readPing());
    sprintf(buffer, "Read Sample %d  \n", samples[i]);
    Serial.println(buffer);
    delay(50);
  }
  samples.asc_sort();
  //return the median
  return samples[numSamples/2];
}



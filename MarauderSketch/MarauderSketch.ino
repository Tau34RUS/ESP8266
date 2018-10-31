#include <Adafruit_NeoPixel.h>
#include <Servo.h>

#define servoPin D1
#define sensorPin D2
#define ledPin D4
#define silentPin D5
#define sensorlessPin D0
#define soundIdle D7
//#define soundLaser D6
#define soundRAC D8
#define LED_COUNT 1
#define LED_PIN D3

//Setting Led Pixel
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const long interval_sensor = 1000;
const long interval_laser = 50;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long intervalMillis = 3000;
unsigned long countdownMillis = 2000;
unsigned long countMillis = 0;
unsigned long lastServoUpdate;
unsigned long alertTime = 3000;
unsigned long alertTimeCounter = 0;
unsigned long idleTime = 10000;
unsigned long idleTimeCounter;


//Switches
bool silent = false;
bool noSensor = false;
bool canFire = true;
bool canRotate = true;
int switchState = 0; //0 - default, 1 - no sound, 2 - no sensors, 3 - no sound and sensors

//PIR Sensor
bool pirSensor = false;
bool lastpirSensor = true;
unsigned long previousSensorMillis = 0;

//Servo
int pos;
int randCase;
int randTargetAngle;
bool actionDone = true;
int currAngle;
int rotationSpeed = 10; //less - faster
Servo servo;

void setup() {

  // Initialize Serial port
  Serial.begin(115200);
  Serial.println("");

  //Set color led
  strip.begin();
  strip.setPixelColor(0, 250, 130, 10);
  strip.setBrightness(16);
  strip.show();

  //Startup pins setup
  //inputs - switches and sensors
  pinMode(sensorPin, INPUT);
  pinMode(sensorlessPin, INPUT);
  pinMode(silentPin, INPUT);
  pinMode(servoPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  //outputs - motors, lasers and sound
  //servo.Attach(servoPin);

  Serial.println("Reactor… online. Sensors… online. Weapons… online");
  Serial.println("All systems nominal");
  servo.attach(servoPin);
  delay(200);

}

void loop() {
  
  currentMillis = millis();
    currAngle = servo.read();

    if (currentMillis - previousSensorMillis > interval_sensor && !servo.attached()) {
      
      pirSensor = digitalRead(sensorPin);
      previousSensorMillis = currentMillis;
     

    }
    //if(!pirSensor)alertTime = currentMillis + 5000;


    //Serial.println(currentMillis - alertTime);
    if (pirSensor) {
      
      

      

      if (currentMillis - countMillis > countdownMillis && actionDone) {
        
        randCase = random(1, 4);
        randTargetAngle = random(10, 171);
        countMillis = currentMillis;

      }

      switch (randCase) {
        case 1:
          TorsoRotation(randTargetAngle);
          break;
        case 2:
          TorsoRotation(randTargetAngle);
          break;
        case 3:
          TorsoRotation(randTargetAngle);
          break;

      }

    }
}

void TorsoRotation(int angle)
{

  int increment;
  int targetAngle;

  targetAngle = angle;
  if (currAngle < targetAngle)increment = 1; else if (currAngle > targetAngle)increment = -1; else {
    increment = 0;
    actionDone = true;
  }
  if ((currentMillis - lastServoUpdate) > rotationSpeed)
  {
    pos = currAngle;
    lastServoUpdate = currentMillis;
    pos += increment;
    servo.write(pos);
    actionDone = false;

  }

}

/*
  void pixelTest(int seconds) {

  strip.setBrightness(4);
  strip.show();
  delay(seconds * 1000);
  strip.setBrightness(8);
  strip.show();
  delay(seconds * 1000);
  strip.setBrightness(16);
  strip.show();
  delay(seconds * 1000);
  strip.setBrightness(32);
  strip.show();
  delay(seconds * 1000);
  strip.setBrightness(64);
  strip.show();
  delay(seconds * 1000);
  strip.setBrightness(128);
  strip.show();
  delay(seconds * 1000);
  strip.setBrightness(250);
  strip.show();
  delay(seconds * 1000);

  }

  void ledColor(String color) {

  if (!color.compareTo("red")) {
    strip.setPixelColor(0, 250, 0, 0);
    strip.show();
  } else if (!color.compareTo("blue")) {
    strip.setPixelColor(0, 0, 0, 250);
    strip.show();
  } else if (!color.compareTo("green")) {
    strip.setPixelColor(0, 0, 250, 0);
    strip.show();
  } else if (!color.compareTo("orange")) {
    strip.setPixelColor(0, 250, 130, 10);
    strip.show();
  } else {
    strip.setPixelColor(0, 0, 0, 0);
    strip.show();
  }

  }

*/

#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include "FastLED.h"
#include <NewPing.h>
#define Neopixel_PIN 1
#define TRIG_PIN 19
#define ECHO_PIN 26
#define MAX_DISTANCE 200
NewPing leftSensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
#define TRIG_PIN 26
#define ECHO_PIN 13
#define MAX_DISTANCE 200
NewPing forwardSensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
int leftRotations = 178; //medium
int rightRotations = 178; //medium
int ldistance;
int fdistance;
int compass = 1;
int stepCounter = 1;
boolean flag = false;
float widthOfMaze = 10;
#define Bb 233.1
#define C 261.6
#define D 293.6
#define Eb 311.13
#define E 329.6
#define F 349.2
#define G 392
#define Ab 415.3
#define A 440
#define B 493.8
#define highC 523.25
#define lowG 196

#include "FastLED.h"



#define Neopixel_PIN 1

#define NUM_LEDS 7



CRGB leds[NUM_LEDS];


String commands = "";
const uint16_t port = 80;         // defining port
const char *host = "192.168.4.1"; // ip or dns
String line;

WiFiClient client;
struct Point
{
  int x;
  int y;
};
struct Point movement[50];
struct Point arr[50];
void LeftDirectionChange() {
  if (compass == 1) compass = 4;
  else if (compass == 2) compass = 1;
  else if (compass == 3) compass = 2;
  else if (compass == 4) compass = 3;
}
void RightDirectionChange() {
  if (compass == 1) compass = 2;
  else if (compass == 2) compass = 3;
  else if (compass == 3) compass = 4;
  else if (compass == 4) compass = 1;
}
void setup() {
  M5.begin();
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  FastLED.addLeds<WS2811, Neopixel_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  betterDelay(1000);
  for (int i = 0; i <= 50; i++) {
    movement[i].x = 0;
    movement[i].y = 0;
  }
}
void loop() {
  pthread_t thread1, thread2, thread3;

  // make threads
  //pthread_create(&thread1, NULL, openleds, NULL);
  pthread_create(&thread2, NULL, mainFunction, NULL);
  //pthread_create(&thread3, NULL, music, NULL);

  // wait for them to finish
  //pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  //pthread_join(thread3, NULL);


}
void* mainFunction(void* mom) {
  M5.update();
  while (1){


    ldistance = leftSensor.ping_cm();
  fdistance = forwardSensor.ping_cm();
  //reaches a junction where robot must turn right
  if (ldistance < (widthOfMaze * 2) && fdistance < (widthOfMaze * 2)) {
    setDrive(-12, -12);
    betterDelay(500);
    turnRight();
    setDrive(12, 12);
    betterDelay(500);
  }
  //robot recognizes a void on the left
  else if (ldistance > 2 * widthOfMaze) {
    setDrive(-12, -12);
    betterDelay(500);
    turnLeft();
    setDrive(12, 12);
    betterDelay(500);
    driveForward();
  }
  else {
    driveForward();
  }

  M5.Lcd.setCursor(0, 50);
  M5.Lcd.printf("Left Distance = %3dcm", ldistance);
  M5.Lcd.setCursor(0, 25);
  M5.Lcd.printf("Forward Distance = %3dcm", fdistance);

  if (flag) {
    M5.Lcd.clear();
    transformArray();
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.print(commands);
    delay(10000);
    stringtoMovement(commands);

}
}
}


void* music(void* mom) {


  for (;;) {
    playNote(C, 100);
    notePause(100);
    playNote(Eb, 100);
    notePause(50);
    playNote(C, 50);
    notePause(50);
    playNote(C, 50);

    playNote(F, 50);
    notePause(50);
    playNote(C, 50);
    notePause(50);
    playNote(Bb, 50);
    notePause(50);
    playNote(C, 100);
    notePause(100);
    playNote(G, 100);
    notePause(50);
    playNote(C, 50);
    notePause(50);
    playNote(C, 50);

    playNote(Ab, 50);
    notePause(50);
    playNote(G, 50);
    notePause(50);
    playNote(Eb, 50);
    notePause(50);
    playNote(C, 50);
    notePause(50);
    playNote(G, 50);
    notePause(50);
    playNote(highC, 50);
    notePause(50);
    playNote(C, 50);
    playNote(Bb, 50);
    notePause(50);
    playNote(Bb, 50);
    playNote(lowG, 50);
    notePause(50);
    playNote(Eb, 50);
    notePause(50);
    playNote(C, 200);
    notePause(800);
  }       




}
void notePause(int ms) {
  M5.Speaker.end();
  M5.Speaker.begin();
  delay(ms * 2);
}
void playNote(float frq, int ms) {
  M5.Speaker.tone(frq, 50);
  delay(ms * 2);
}





void* openleds(void* mom) {
  
  while (1) {
    leds[3] = CRGB(100, 200, 50);

    for (int i = 0; i < 3; i++)
    {
      leds[i] = CRGB(255, 0, 0);
    }

    delay(400);
    for (int i = 4; i < 7; i++)
    {
      leds[i] = CRGB(0, 0, 255);
    }
    FastLED.show();


    for (int i = 0; i < 3; i++)
    {
      leds[i] = CRGB(255, 255, 0);
    }

    delay(400);
    for (int i = 4; i < 7; i++)
    {
      leds[i] = CRGB(0, 255, 0);
    }
    FastLED.show();

  }
  
}

void betterDelay(int t) {
  uint32_t startTime = millis();
  while (millis() < startTime + t) {
    M5.update();
    if (M5.BtnB.wasPressed()) {
      flag = true;

    }
  }
}
void stringtoMovement(String request) {
  for (int i = 0; i < request.length(); i++) {
    if (request.substring(i, i + 1).equals("l")) {
      turnLeft();
    } else if (request.substring(i, i + 1).equals("r")) {
      turnRight();
    } else if (request.substring(i, i + 1).equals("f")) {
      driveForward();
    }
  }
}
void turnRight() {
  RightDirectionChange();
  setDrive(66, -66);
  betterDelay(2800);
}
void turnLeft() {
  LeftDirectionChange();
  setDrive(-67, 67);
  betterDelay(2800);
}
void setDrive(int rotation1, int rotation2) {
  Serial2.printf("$J = G21 G91 X%d Y%d F%d", rotation1, rotation2, 4000);
  Serial2.print("\r\n\r\n");
}
void driveForward() {
  setDrive(leftRotations, rightRotations);
  betterDelay(4300);
  stepCounter++;
  if (compass == 1)
  {
    movement[stepCounter].y = movement[stepCounter - 1].y + 1;
    movement[stepCounter].x = movement[stepCounter - 1].x;
  }
  if (compass == 2)
  {
    movement[stepCounter].x = movement[stepCounter - 1].x + 1;
    movement[stepCounter].y = movement[stepCounter - 1].y;
  }
  if (compass == 3)
  {
    movement[stepCounter].y = movement[stepCounter - 1].y - 1;
    movement[stepCounter].x = movement[stepCounter - 1].x;
  }
  if (compass == 4)
  {
    movement[stepCounter].x = movement[stepCounter - 1].x - 1;
    movement[stepCounter].y = movement[stepCounter - 1].y;
  }
}
void transformArray() {
  for (int i = 0; i < 50; i++)
  {
    for (int j = 50; j > i; j--)
    {
      if (!(movement[i].x == 0 && movement[i].y == 0) && !(movement[j].x == 0 && movement[j].y == 0) && ((movement[i].x == movement[j].x) && (movement[i].y == movement[j].y)))
      {
        int tempi = i;
        int tempj = j;
        for (int m = 0; m <= 50; m++) {
          arr[m].x = 0;
          arr[m].y = 0;
        }
        for (int k = 0; k <= tempi; k++)
        {
          arr[k] = movement[k];
        }
        for (int k = tempi + 1; k < 50; k++, tempj++)
        {
          arr[k] = movement[tempj + 1];
        }
        for (int r = 0; r < 50; r++)
        {
          movement[r] = arr[r];
        }
      }
    }
  }
  convertPointsToCode(movement);
}
void convertPointsToCode(Point points[]) {
  //for loop comparing all the different two points
  compass = 1;

  for (int i = 1; i < 50; i++) {
    if (points[i].x != 0 || points[i].y != 0) {
      if (points[i].x == points[i - 1].x && points[i].y > points[i - 1].y) {

        if (compass == 4) {
          compass = 1;
          //turn Right

          commands += "r";
        }
        if (compass == 2) {
          compass = 1;
          //turn Left

          commands += "l";
        }
      } else if (points[i].x == points[i - 1].x && points[i].y < points[i - 1].y) {
        if (compass == 2) {
          compass = 3;
          //turn Right

          commands += "r";
        }
        if (compass == 4) {
          compass = 3;
          //turn Left

          commands += "l";
        }

      } else if (points[i].y == points[i - 1].y && points[i].x > points[i - 1].x) {
        if (compass == 1) {
          compass = 2;
          //turn Right
          commands += "r";
        }
        if (compass == 3) {
          compass = 2;
          //turn Left
          commands += "l";
        }
      } else if (points[i].y == points[i - 1].y && points[i].x < points[i - 1].x) {
        if (compass == 1) {
          compass = 4;
          //turn Left

          commands += "l";
        }
        if (compass == 3) {
          compass = 4;
          //turn Right

          commands += "r";
        }
      }
      commands += "f";
    }

  }

}

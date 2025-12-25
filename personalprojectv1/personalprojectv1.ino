#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo helmetServo;
//notes
int NOTE_C4 = 262;
int NOTE_E4 = 330;
int NOTE_G4 = 392;
int NOTE_C5 = 523;
int NOTE_G5 = 784;

//pins
int servoPin = 3;
int buttonPin = 2;
int ldrbuttonPin = 9;
int ldrPin = A0;
int buzzerPin = 8;

int redPin = 4;
int greenPin = 5;
int bluePin = 6;

//settings
int motorSpeed = 5;
int lightThreshold = 180; //shadow trigger level (if adjust needed)
//memory 
int buttonStatus = 0;
int ldrbuttonStatus = 0;
int lightValue = 0;
int armorState = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(ldrbuttonPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  helmetServo.attach(servoPin);
//lcd
  lcd.init();
  lcd.backlight();
//startup settings; close the helmet and turn off the leds
  helmetServo.write(0);
  setEyeColor(0, 0, 0);
  bootSequence();
  Serial.begin(9600);
}

void loop() {
  //1. listen the button
  ldrbuttonStatus = digitalRead(ldrbuttonPin);
  buttonStatus = digitalRead(buttonPin);
  lightValue = analogRead(ldrPin);
  Serial.println(lightValue);
  if (ldrbuttonStatus == HIGH) {
    lcd.setCursor(0, 0); lcd.print("SENSOR CHECK:   ");
    lcd.setCursor(0, 1); lcd.print("LDR LEVEL: ");
    lcd.print(lightValue);
    lcd.print("    ");
  }
  if (buttonStatus == HIGH || lightValue < lightThreshold) {
    if (armorState == 0) {
      //lcd information
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("SYSTEM: ACTIVE");
      lcd.setCursor(0, 1); lcd.print("MASK: OPENING...");
       

      tone(buzzerPin, NOTE_C4); delay(100);
      tone(buzzerPin, NOTE_E4); delay(100);
      tone(buzzerPin, NOTE_G4); delay(100);
      noTone(buzzerPin);

      setEyeColor(0, 255, 255);
      for (int pos = 0; pos <= 180; pos += 1) {
        helmetServo.write(pos);
        delay(motorSpeed);
      }
       armorState = 1;

       lcd.setCursor(0, 1); lcd.print("MASK: OPEN");

    } else {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("SYSTEM: STANDBY");
      lcd.setCursor(0, 1); lcd.print("MASK: CLOSING...");

      tone(buzzerPin, NOTE_G4); delay(100);
      tone(buzzerPin, NOTE_E4); delay(100);
      tone(buzzerPin, NOTE_C4); delay(100);
      noTone(buzzerPin);

      for (int pos = 180; pos >= 0; pos -= 1) {
        helmetServo.write(pos);
        delay(motorSpeed);
      }
      setEyeColor(0, 0, 0);
      armorState = 0;
      
      lcd.setCursor(0, 1); lcd.print("MASK: CLOSED...");
    }
    delay(500);
  }
}

void setEyeColor(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}

void bootSequence() {
  // LCD LAUNCH MESSAGE
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("METAL SPARK OS");
  lcd.setCursor(0, 1); lcd.print("INITIALIZING...");
  // LED CHECK
  setEyeColor(255, 0, 0); delay(150);
  setEyeColor(0, 255, 0); delay(150);
  setEyeColor(0, 0, 255); delay(150);
  setEyeColor(0, 0, 0);

  tone(buzzerPin, NOTE_G5); delay(100); noTone(buzzerPin); delay(50);
  tone(buzzerPin, NOTE_G5); delay(100); noTone(buzzerPin);

  // MOTOR CHECK
  helmetServo.write(30);
  delay(300);
  helmetServo.write(0);
  delay(300);

  // Sys ready signal (2 blinks)
  for(int i=0; i<2; i++) {
    setEyeColor(0, 255, 0);
    delay(150);
    setEyeColor(0, 255, 0);
    delay(150);
  }  
}
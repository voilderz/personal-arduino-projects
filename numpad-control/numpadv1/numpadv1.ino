#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;


char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};


byte rowPins[ROWS] = { 22, 23, 24, 25 };
byte colPins[COLS] = { 26, 27, 28, 29 };


Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("INPUT TEST:");
}

void loop() {

  char key = myKeypad.getKey();

  if (key) {
    Serial.println(key);


    lcd.setCursor(0, 1);
    lcd.print("KEY: ");
    lcd.print(key);
  }
}
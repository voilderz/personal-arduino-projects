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
  { '*', '0', '#', 'D' }, 
};

byte rowPins[ROWS] = { 22, 23, 24, 25};
byte colPins[COLS] = { 26, 27, 28, 29};

/*
code down below introduces the "keys" matrix to arduino. 
with help of <keypad.h> library, it creates a digital map.
*/
Keypad passKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int passLength = 5;
char masterPass[passLength] = "1492";
char inputPass[passLength];
byte data_count = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("SYSTEM LOCKED");
}

void loop() {
  char key = passKeypad.getKey();

  if (key) {
    switch (key) {
      case '#':
      checkPass();
      break;

      case '*':
      clearData();
      break;

      default:
      collectData(key);
      break;

    }
  }
}

void checkPass() {
  inputPass[data_count] = '\0';

  if (strcmp(inputPass, masterPass) == 0) {
    lcd.clear();
    lcd.print("ACCESS GRANTED");
    delay(2000);
    clearData();
  } else {
    lcd.clear();
    lcd.print("ACCESS DENIED");
    delay(2000);
    clearData();
  }
}

void collectData(char key) {
  if (data_count < passLength - 1) {
    inputPass[data_count] = key;
    lcd.setCursor(data_count, 1);
    lcd.print("*");
    data_count++;
  }
}

void clearData() {
  while (data_count != 0) {
    inputPass[data_count--] = 0;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SYSTEM LOCKED");
  data_count = 0;
}
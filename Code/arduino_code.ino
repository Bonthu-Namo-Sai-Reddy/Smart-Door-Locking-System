#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int greenLedPin = 13;
const int redLedPin = 12;
const int buzzerPin = 11;
const int servoPin = 10;

int password[] = {1, 2, 3, 4}; // initial password
int enteredPassword[4];
int position = 0;
bool changePassword = false;
Servo myServo;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  myServo.attach(servoPin);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Electronic Lock");
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    if (key == 'A') { // backspace mode (delete previous character)
      if (position > 0) {
        position--;
        lcd.setCursor(position, 1);
        lcd.print(" ");
        lcd.setCursor(position, 1);
      }
    } else if (key == 'B') { // change password mode
      changePassword = true;
      position = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter new password");
    } else if (key == 'C') { // backspace mode (delete previous four characters)
      if (position > 3) {
        position -= 4;
        for (int i = position; i < position + 4; i++) {
          lcd.setCursor(i, 1);
          lcd.print(" ");
        }
        lcd.setCursor(position, 1);
      } else {
        position = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter password");
      }
    } else if (changePassword) { // set new password
      int num = key - '0';
      enteredPassword[position] = num;
      position++;
      if (position == 4) {
        for (int i = 0; i < 4; i++) {
          password[i] = enteredPassword[i];
        }
        changePassword = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Password changed");
        Serial.println("Password changed");
      }
    } else { // enter password mode
      int num = key - '0';
      enteredPassword[position] = num;
      position++;
      lcd.print("*");
      if (position == 4) {
        bool correctPassword = true;
        for (int i = 0; i < 4; i++) {
          if (enteredPassword[i] != password[i]) {
correctPassword = false;
}
}
    if (correctPassword) {
      digitalWrite(greenLedPin, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access granted");
      Serial.println("Access granted");
      myServo.write(0); // open the lock
      delay(8000);
      myServo.write(90); // close the lock
      digitalWrite(greenLedPin, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter password");
    } else {
      digitalWrite(redLedPin, HIGH);
      tone(buzzerPin, 2000, 3000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access denied");
      Serial.println("Access denied");
      delay(2000);
      digitalWrite(redLedPin, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter password");
    }

    position = 0;
    for (int i = 0; i < 4; i++) {
      enteredPassword[i] = 0;
    }
  }
}
  }
  }

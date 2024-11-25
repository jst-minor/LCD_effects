#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

//-------------------Settings--------------------//
#define tmr 300   //Display cleaning speed, ms

unsigned long ClearPassedTime = 0;
char counter = 0;
bool WClear = 0;

//--------------custom Characters for better visual effect--------------//
byte customChar[8] = {0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15};
byte customChar1[8] = {0x00, 0x00, 0x00, 0x15, 0x0A, 0x15, 0x0A, 0x15};


void setup(){
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, customChar);
  lcd.createChar(1, customChar1);

  lcd.setCursor(5, 2);
  lcd.print("some text");

  WClear = true;
}

void loop(){
  //------------------clearing the display------------------//
  if (WClear && millis()-ClearPassedTime >= tmr){
    ClearPassedTime = millis();
    if (counter < 4){
      lcd.setCursor(0, counter);
      for (char i = 0; i <= 19; i++) lcd.write(255);
    }
    if (counter > 0 && counter < 5){
      lcd.setCursor(0, counter-1);
      for (char i = 0; i <= 19; i++) lcd.write(0);
    }
    if (counter > 1 && counter < 6){
      lcd.setCursor(0, counter-2);
      for (char i = 0; i <= 19; i++) lcd.write(1);
    }
    if (counter > 2 && counter < 7){
      lcd.setCursor(0, counter-3);
      lcd.print("                    ");
    }
    if (counter > 7){
      lcd.clear();
      counter = 0;
      WClear = false; 
    } else counter++;
  }
}
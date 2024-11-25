#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);


//---------------SETTINGS---------------//
#define tmr0 500    //Cursor flicker rate, ms
#define tmr1 150    //Character output rate, ms

//---Separate elapsed time flags for cursor, text input and LED---//
unsigned long CursorPassedTime = 0;
unsigned long CharPassedTime = 0;

unsigned char for_i = 0;            //Variable for serial output
unsigned char CursorX, CursorY;     //Cursor coordinates

String ToWrite;                     //Queue of output

bool CursorF;                       //Flag for the cursor


void setup(){
  lcd.init();
  lcd.backlight();

  /*Setting coordinates and the text - you can do this at any 
  place in the program according to your needs.*/
  CursorX = 0; CursorY = 0;
  ToWrite = "ABCDEFGHIJKLMNOPGRSTUVWXYZ/nit works!!!        Even like that";
}

void loop(){
  //----------Printing the text----------//
  if (ToWrite != ""){
    if (millis() - CharPassedTime >= tmr1){
      if (for_i < ToWrite.length()){
        lcd.setCursor(CursorX, CursorY);
        CharPassedTime = millis();
        CursorX++;
        if (ToWrite[for_i] == '/' && ToWrite[for_i+1] == 'n'){           //Processing the "/n" command
          lcd.print(' ');
          CursorX = 0;
          CursorY++;
          for_i += 2;
        } else if (ToWrite[for_i] == ' ' && ToWrite[for_i+1] == ' '){    //Skipping spaces
          char i;
          for (i = 2; i <= 80; i++){
            if (ToWrite[for_i+i] != ' '){
              break;
            }
          }
          CursorX += i;
          for_i += i;
        } else {
          lcd.print(ToWrite[for_i]);
          for_i++;
        }
      } else {
        ToWrite = "";
        for_i = 0;
      }
    }
  }

  //---------blinking the cursor---------//
  if (millis() - CursorPassedTime >= tmr0){
    lcd.setCursor(CursorX, CursorY);
    CursorPassedTime = millis();
    if(CursorF){
      lcd.write(255);
    } else {
      lcd.print(" ");
    }
    CursorF = !CursorF;
  }

  //---moving to the next line---//
  if (CursorX>19){
    CursorX = 0;
    CursorY++;
  }
  if (CursorY>3) CursorY = 0;
}
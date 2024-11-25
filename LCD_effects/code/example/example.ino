#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

//---------------SETTINGS---------------//
#define LED_PIN 13
#define BUTTON_PIN 2
#define BDelay 500   //Delay to avoid button rattling, ms
#define tmr1 500     //Cursor flicker rate, ms
#define tmr2 150     //Character output rate, ms
#define tmr3 300     //LED blinking rate, ms
#define tmr4 300     //Display cleaning speed, ms

/*Separate elapsed time flags for cursor, 
text output, clearing the text, button and LED*/
unsigned long CursorPassedTime = 0;
unsigned long CharPassedTime = 0;
unsigned long LEDPassedTime = 0;
unsigned long ButtonPassedTime = 0;
unsigned long ClearPassedTime = 0;

String ToWrite;            //Queue of output

char counter = 0;          //Wave position
char ButtonCNTR = 1;       //A variable for counting button clicks
char for_i = 0;            //A variable for serial output
char CursorX, CursorY;     //Cursor coordinates

//--------------custom Characters for better visual effect--------------//
byte customChar[8] = {0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15};
byte customChar1[8] = {0x00, 0x00, 0x00, 0x15, 0x0A, 0x15, 0x0A, 0x15};

bool WClear = 0;                    //Flag for the wave
bool CursorF;                       //Flag for the cursor
bool LEDF;                          //Flag for the LED
bool ButtonF = false;               //Flag for the button

void setup(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, customChar);
  lcd.createChar(1, customChar1);

  //--------------------Setting coordinates and the text--------------------//
  CursorX = 0; CursorY = 0;
  ToWrite = "some text";
}

void loop(){
  //----------Interaction with the button----------//
  ButtonF = !digitalRead(BUTTON_PIN);
  if (ButtonF && millis()-ButtonPassedTime >= BDelay){
    ButtonPassedTime = millis();
    if (ButtonCNTR == 5) ButtonCNTR = 0;
    else ButtonCNTR++;

    if (ButtonCNTR == 1) ToWrite += "some text";
    else if (ButtonCNTR > 1 && ButtonCNTR <= 4) ToWrite += "/nsome text";
    else if (ButtonCNTR == 5) WClear = true;

    ButtonF = false;
  }
  //---flashing the LED to demonstrate multitasking---//
  if (millis() - LEDPassedTime >= tmr3){
    if (LEDF){
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
    LEDF = !LEDF;
    LEDPassedTime = millis();
  }

  //----------Printing the text----------//
  if (ToWrite != ""){
    if (millis() - CharPassedTime >= tmr2){
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
  if (millis() - CursorPassedTime >= tmr1){
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

  //------------------clearing the display------------------//
  if (WClear && millis()-ClearPassedTime >= tmr4){
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
      CursorX = 0; CursorY = 0;
      counter = 0;
      WClear = false;
    } else counter++;
  }
}
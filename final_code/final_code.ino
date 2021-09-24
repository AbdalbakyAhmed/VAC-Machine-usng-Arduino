#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
///////////////////////////////




//Keypad Declaretion
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {44, 45, 46, 47}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {50, 51, 52, 53}; //connect to the column pinouts of the keyp
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);




//Stepper Motor declare
#include <Stepper.h>
#define stepsPerRevolution 200  // change this to fit the number of steps per revolution
#define stepper_speed 100       // change this for the need RPM
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);  //Motor Driver PINs

//DC Motor declare
#define M1 12
#define M2 13
//#define enM

//Limit Switches declare
#define LS1 22
#define LS2 23
#define LS3 24
#define LS4 25

//Relays
#define heater_relay 28
#define vacuum_relay 29

#define start_sw 30

char LS1_R, LS2_R, LS3_R, LS4_R; //variables to get Readings from Limit Switches
char flag = 0;
long delay_1 = 5000, delay_2 = 5000;


void setup() {
  Serial.begin(9600);
  lcd.clear();
  delay(1000);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.println("  Vac Forming   ");
  lcd.setCursor(0, 1);
  lcd.print("    Machine");

  myStepper.setSpeed(stepper_speed);
  pinMode(LS1, INPUT);
  pinMode(LS2, INPUT);
  pinMode(LS3, INPUT);
  pinMode(LS4, INPUT);
  pinMode(start_sw, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(heater_relay, OUTPUT);
  pinMode(vacuum_relay, OUTPUT);
}


void loop() {
  char start_R;
  start_R = digitalRead(start_sw);

  //START the Machine
  if (start_R == HIGH) {
    Serial.println("Machine Start");
    //Moving Right Scope
    LS2_R = digitalRead(LS2);
    delay(100);
    if (flag == 0)
    {
      while (LS2_R != HIGH)
      {
        Serial.println("Moving Right");
        digitalWrite(M1, HIGH);
        digitalWrite(M2, LOW);

        LS2_R = digitalRead(LS2);
      }
      if (flag == 0 && LS2_R == HIGH)
      {
        digitalWrite(M1, LOW);
        digitalWrite(M2, LOW);
        flag = 1;
        Serial.println("Flag=1 now");
      }
    }
    /////////////////////////////
    // we need to stop the stepper Motor

    //Heater Scope
    if (LS2_R == HIGH) {
      Serial.println("Heater ON");
      digitalWrite(heater_relay, HIGH);
      //We will get the Delay value from user.!!!
      delay(delay_1);
      digitalWrite(heater_relay, LOW);
      Serial.println("Heater OFF");
    }
    ////////////////////////
    //Moving Left Scope
    LS1_R = digitalRead(LS1);
    delay(100);
    if (flag == 1)
    {
      while (LS1_R != HIGH)
      {
        Serial.println("Moving Left");
        digitalWrite(M2, HIGH);
        digitalWrite(M1, LOW);
        LS1_R = digitalRead(LS1);
      }
      if (LS1_R == HIGH && flag == 1)
      {
        digitalWrite(M2, LOW);
        digitalWrite(M1, LOW);
        flag = 2;
        Serial.println("flag=2");
      }
    }

    //Moving Up Scope
    LS4_R = digitalRead(LS4);
    delay(100);
    if (flag == 2)
    {
      while (LS4_R != HIGH)
      {
        Serial.println("Moving UP");
        myStepper.step(stepsPerRevolution);
        
        LS4_R = digitalRead(LS4);
      }
      if (flag == 2 && LS4_R == HIGH)
      {
        
        flag = 3;
        Serial.println("flag=3 and Stop UP");
      }
    }

    //Vacuum Scope
    if (LS4_R == HIGH) {
      Serial.println("vacuum ON");
      digitalWrite(vacuum_relay, HIGH);
      //We will get the Delay value from user.!!!
      delay(delay_2);
      digitalWrite(vacuum_relay, LOW);
      Serial.println("vacuum OFF");
    }

    //Moving Down Scope
    LS3_R = digitalRead(LS3);
    delay(100);
    if (flag == 3) {
      while (LS3_R != HIGH) {
        Serial.println("Moving Down");
        myStepper.step(-stepsPerRevolution);
        
        LS3_R = digitalRead(LS3);
      }
      if (flag == 3 && LS3_R == HIGH) {
        
        flag = 5; //any value to break the loop, and don't reply the process
        Serial.println("flag=5 And stop Down");
      }
    }
  }


  else {
    flag = 0;
    //Do Nothing
    Serial.println("Machine Off");
  }
}

int get_num(char key) {
  int num;
  switch (key) {
    case '0':
      num = 0;
      break;
    case '1':
      num = 1;
      break;
    case '2':
      num = 2;
      break;
    case '3':
      num = 3;
      break;
    case '4':
      num = 4;
      break;
    case '5':
      num = 5;
      break;
    case '6':
      num = 6;
      break;
    case '7':
      num = 7;
      break;
    case '8':
      num = 8;
      break;
    case '9':
      num = 9;
      break;
  }
  return num;
}









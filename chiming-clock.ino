// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"
#include <AnalogButtons.h>

#define switchHourPin A0
#define switchMinutePin A1
#define switchMinuteHoldPin A2
#define switchChimesPin A3
#define switchDisplayPin A4

// Setup display
const byte display_CLK = 0;   // define CLK pin (any digital pin)
const byte display_DIO = 1;   // define DIO pin (any digital pin)
SevenSegmentExtended display(display_CLK, display_DIO);

// Setup RTC
RTC_DS1307 rtc;

// Setup chime outputs
int chime1 = 5;
int chime2 = 6;
int chime3 = 7;
int chime4 = 8;
int chime5 = 9;
float chimeBPM = 480;
float chimeBeat = 1e3*60.0/chimeBPM; // milliseconds
float chimeOnTime = 100; // milliseconds
float chimeOffTime = chimeBeat-chimeOnTime; // milliseconds
bool chimed = false;

int potentiometer = 5;
int ledBlue = 11;
int ledYellow = 13;
int ledGreen = 12;
int sw1 = 7;
int sw2 = 8;

int val = 0;


int chimes = 0; // 0 --> Chimes on, 1 --> Day only, 2 --> Chimes off


// Setup analog switches
AnalogButtons switchHour(switchHourPin, INPUT);
void fnHourAdvance() {
  DateTime now = rtc.now();
  rtc.adjust(now+TimeSpan(0,1,0,0));
  }
void fnHourRetard() {
  DateTime now = rtc.now();
  rtc.adjust(now+TimeSpan(0,-1,0,0));
}
Button buttonHourAdvance = Button(1007, &fnHourAdvance);
Button buttonHourRetard = Button(11, &fnHourRetard);

AnalogButtons switchMinute(switchMinutePin, INPUT);
void fnMinuteAdvance() {
  DateTime now = rtc.now();
  rtc.adjust(now+TimeSpan(0,0,1,0));
  }
void fnMinuteRetard() {
  DateTime now = rtc.now();
  rtc.adjust(now+TimeSpan(0,0,-1,0));
}
Button buttonMinuteAdvance = Button(1007, &fnMinuteAdvance);
Button buttonMinuteRetard = Button(11, &fnMinuteRetard);

AnalogButtons switchMinuteHold(switchMinuteHoldPin, INPUT);
void fnMinuteHold() {
  DateTime now = rtc.now();
  rtc.adjust(now+TimeSpan(0,0,0,-now.second()));
  }
Button buttonMinuteHold = Button(1007, &fnMinuteHold);

AnalogButtons switchChimes(switchChimesPin, INPUT);
void fnChimesOn() {
  chimes = 0;
  }
void fnChimesDay() {
  chimes = 1;
}
void fnChimesOff() {
  chimes = 2;
}
Button buttonChimesOn = Button(1007, &fnChimesOn);
Button buttonChimesDay = Button(512, &fnChimesDay);
Button buttonChimesOff = Button(11, &fnChimesOff);

AnalogButtons switchDisplay(switchDisplayPin, INPUT);
void fnDisplayOn() {
  display.setBacklight(10);
  }
void fnDisplayOff() {
  display.setBacklight(0);
}

Button buttonDisplayOn = Button(1007, &fnDisplayOn);
Button buttonDisplayOff = Button(11, &fnDisplayOff);



void setup () {
  display.begin();            // initializes the display
  display.setBacklight(10);  // set the brightness to 100 %

//#ifndef ESP8266
//  while (!Serial); // for Leonardo/Micro/Zero
//#endif

  //Serial.begin(9600);
  if (! rtc.begin()) {
    // Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    // Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  }

  switchHour.add(buttonHourAdvance);
  switchHour.add(buttonHourRetard);

  switchMinute.add(buttonMinuteAdvance);
  switchMinute.add(buttonMinuteRetard);

  switchMinuteHold.add(buttonMinuteHold);

  switchChimes.add(buttonChimesOn);
  switchChimes.add(buttonChimesDay);
  switchChimes.add(buttonChimesOff);

  switchDisplay.add(buttonDisplayOn);
  switchDisplay.add(buttonDisplayOff);
}

void loop () {
//    display.print("POO ");
//    delay(250);
//    display.print("OO P");
//    delay(250);
//    display.print("O PO");
//    delay(250);
//    display.print(" POO");
//    delay(250);  

    switchHour.check();
    //switchMinute.check();
    //switchMinuteHold.check();
    //switchChimes.check();
    //switchDisplay.check();
    //configure();
  
    DateTime now = rtc.now();

    display.printTime(now.hour(),now.minute(), false);

    //digitalWrite(ledGreen,digitalRead(sw1));
    //digitalWrite(ledYellow,digitalRead(sw2));

    val = analogRead(potentiometer);
    //Serial.println(val);
    if (val > 512) {
      digitalWrite(ledBlue,1);
      //fnDisplayOff();
      if (! chimed){
        chime();
        chimed = true;
      }
    }
    else {
      digitalWrite(ledBlue,0);
      //fnDisplayOn();
      chimed = false;
    }

    //delay(1);


    
    




    
}




void configure() {
  unsigned int value = analogRead(switchHourPin);
  Serial.println(value);
  delay(250);
}

void chime() {
  chime60();
}

void chimeSeq1() {
  digitalWrite(chime1,1);
  delay(chimeOnTime);
  digitalWrite(chime1,0);
  delay(chimeOffTime);
  digitalWrite(chime2,1);
  delay(chimeOnTime);
  digitalWrite(chime2,0);
  delay(chimeOffTime);
  digitalWrite(chime3,1);
  delay(chimeOnTime);
  digitalWrite(chime3,0);
  delay(chimeOffTime);
  digitalWrite(chime4,1);
  delay(chimeOnTime);
  digitalWrite(chime4,0);
  delay(chimeOffTime);
  delay(2*chimeBeat);
}

void chimeSeq2() {
  digitalWrite(chime3,1);
  delay(chimeOnTime);
  digitalWrite(chime3,0);
  delay(chimeOffTime);
  digitalWrite(chime1,1);
  delay(chimeOnTime);
  digitalWrite(chime1,0);
  delay(chimeOffTime);
  digitalWrite(chime2,1);
  delay(chimeOnTime);
  digitalWrite(chime2,0);
  delay(chimeOffTime);
  digitalWrite(chime4,1);
  delay(chimeOnTime);
  digitalWrite(chime4,0);
  delay(chimeOffTime);
  delay(2*chimeBeat);
}

void chimeSeq3() {
  digitalWrite(chime3,1);
  delay(chimeOnTime);
  digitalWrite(chime3,0);
  delay(chimeOffTime);
  digitalWrite(chime2,1);
  delay(chimeOnTime);
  digitalWrite(chime2,0);
  delay(chimeOffTime);
  digitalWrite(chime1,1);
  delay(chimeOnTime);
  digitalWrite(chime1,0);
  delay(chimeOffTime);
  digitalWrite(chime3,1);
  delay(chimeOnTime);
  digitalWrite(chime3,0);
  delay(chimeOffTime);
  delay(2*chimeBeat);
}

void chimeSeq4() {
  digitalWrite(chime1,1);
  delay(chimeOnTime);
  digitalWrite(chime1,0);
  delay(chimeOffTime);
  digitalWrite(chime3,1);
  delay(chimeOnTime);
  digitalWrite(chime3,0);
  delay(chimeOffTime);
  digitalWrite(chime2,1);
  delay(chimeOnTime);
  digitalWrite(chime2,0);
  delay(chimeOffTime);
  digitalWrite(chime4,1);
  delay(chimeOnTime);
  digitalWrite(chime4,0);
  delay(chimeOffTime);
  delay(2*chimeBeat);
}

void chimeSeq5() {
  digitalWrite(chime4,1);
  delay(chimeOnTime);
  digitalWrite(chime4,0);
  delay(chimeOffTime);
  digitalWrite(chime2,1);
  delay(chimeOnTime);
  digitalWrite(chime2,0);
  delay(chimeOffTime);
  digitalWrite(chime1,1);
  delay(chimeOnTime);
  digitalWrite(chime1,0);
  delay(chimeOffTime);
  digitalWrite(chime3,1);
  delay(chimeOnTime);
  digitalWrite(chime3,0);
  delay(chimeOffTime);
  delay(2*chimeBeat);
}
void chimeHours() {
  DateTime now = rtc.now();
  int n = now.hour()%13;
  for (int i=0; i <= n; i++){
      digitalWrite(chime5, 1);
      delay(chimeOnTime);
      digitalWrite(chime5, 0);
      delay(chimeOffTime);
      delay(4*chimeBeat);
   }
}

void chime15() {
  chimeSeq1();
}
void chime30() {
  chimeSeq2();
  chimeSeq3();
}
void chime45() {
  chimeSeq4();
  chimeSeq5();
  chimeSeq1();
}
void chime60() {
  chimeSeq2();
  chimeSeq3();
  chimeSeq4();
  chimeSeq5();
  chimeHours();
}

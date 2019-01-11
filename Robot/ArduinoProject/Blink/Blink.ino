/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
const int LED_LEFT = 3;
const int LED_RIGHT = 4;

const int THRESHOLD = 10;
int lastBrightnessLeft = 0;
int lastBrightnessRight = 0;
int state = 0;
// 0 ... drive forward
// 1 ... turn left
// 2 ... turn right

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(A0, HIGH);
  pinMode(A5, INPUT);
  digitalWrite(A5, HIGH);

 lastBrightnessLeft = analogRead(A5);
 lastBrightnessRight = analogRead(A0);
}

// the loop function runs over and over again forever
void loop() {
  /*digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(3, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);*/                       // wait for a second

 int brightnessLeft = analogRead(A5);
 int brightnessRight = analogRead(A0);

  int transitionLeft = checkTransition(brightnessLeft, lastBrightnessLeft);
  int transitionRight = checkTransition(brightnessRight, lastBrightnessRight);


  if(state == 0){
    if(transitionLeft){
      state = 1;
    }else if(transitionRight){
      state = 2;
    }
  }else if(state == 1){
    if(transitionRight){
      state = 0;
    }
  }else if(state == 2){
    if(transitionLeft){
      state = 0;
    }
  }

  

  /*
  if (lastBrightnessLeft - brightnessLeft > THRESHOLD){
    Serial.println("Uebergang1");
  }
  if (lastBrightnessRight - brightnessRight > THRESHOLD){
    Serial.println("Uebergang2");
  }*/
  
  digitalWrite(LED_LEFT, LOW);
  digitalWrite(LED_RIGHT, LOW);
  if(state == 0){
    digitalWrite(LED_LEFT, HIGH);
    digitalWrite(LED_RIGHT, HIGH);
  }
  if(state == 1){
    digitalWrite(LED_RIGHT, HIGH);
  }
  if(state == 2){
    digitalWrite(LED_LEFT, HIGH);    
  }
  lastBrightnessLeft = brightnessLeft;
  lastBrightnessRight = brightnessRight;
  delay(100);
  //Serial.println(brightness);
}



// return 1 ... good transition
// return 0 ... no transition
int checkTransition(int brightness, int lastBrightness){
   if (brightness - lastBrightness > THRESHOLD){
    return 1;
  }else{
    return 0;
  }
}

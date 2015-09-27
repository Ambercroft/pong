/*
 FRAMEWORK 
 starting point for a project 
 Created by Bob Wickson
 2014/06/24 - Version 1.0
 2015/09/06 - Version 1.1 converted from original program
 
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:

#include <Servo.h>

void lift();
int led = 13;
int off_time = 4000;
int reset_time = 250;
int val = 0;
int promini = 1;

int step1 = 9;
int step2 = 8;
int step3 = 10;
int step4 = 11;

int stp = 0;
int curstp = 0;

int liftdown = 0; // run the lift down
int liftup   = 0; // run the lift up
int lifttop = 18; // this should be a0
int liftbot = 19; // this should be a1
int liftspeed = 10; // valid between 0 and 255
int button3 = 20; // this should be a2
int button4 = 21; // this should be a3

int liftdir = 12; // this should be MISO or 16 on mini
int liftpwr = 13; // this should be SCK

int gate1 = 2; // this runs the solinoid gate
int servo1 = 3; // this runs the servo gate
int swv1 = 90; // reset to centre position

Servo track_switch1;


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(gate1, OUTPUT);  
  digitalWrite(gate1, LOW);
  
  track_switch1.attach(servo1);
  track_switch1.write(swv1);
  
  
  pinMode(led, OUTPUT);
  pinMode(step1,OUTPUT);
  pinMode(step2,OUTPUT);
  pinMode(step3,OUTPUT);
  pinMode(step4,OUTPUT);
     
  digitalWrite(led, LOW);
  digitalWrite(step1, LOW);
  digitalWrite(step2, LOW);
  digitalWrite(step3, LOW);
  digitalWrite(step4, LOW);

  pinMode(lifttop,INPUT);
  pinMode(liftbot,INPUT);
  pinMode(liftdir,OUTPUT);
  digitalWrite(liftdir,LOW);
  pinMode(liftpwr,OUTPUT);
  digitalWrite(liftpwr,LOW);
  
  // initialize communication
  int n = 0;
  Serial.begin(9600);
  while(n != 58){ // this is the code for a ':'
    Serial.print(n);
    Serial.println("Pong control");
    Serial.println("Board is waiting for reply of ':' to start.");
    if(Serial.available()){
      n = Serial.read();
          }
  }
}

// the loop routine runs over and over again forever:
void loop() {
  int n = 0;
  // read from the usb port
  if(Serial.available()){
    n = Serial.read();
    if ( n > 57 ) {
       Serial.println(n); // uncomment for echo back to contol server
    }
  }
  
  // Elevator motor
  // If top switch on, only allow down
  if ( digitalRead(lifttop)){
    digitalWrite(liftpwr,LOW);
    liftup = 0;
  }
  if ( digitalRead(liftbot)){
    digitalWrite(liftpwr,LOW);
    liftdown = 0;
  }
  if ( (liftdown == 1) && (digitalRead(liftbot) == 0)){
    digitalWrite(liftdir,LOW);
    analogWrite(liftpwr,liftspeed);
  }
  if ( (liftup == 1) && (digitalRead(lifttop) == 0)){
    digitalWrite(liftdir,HIGH);
    analogWrite(liftpwr,liftspeed);
  }
  // If top switch turned on, stop
  // If bottom switch turned on, stop
  // If bottom switch on, only allow up
  

  switch (n){
  case 65: // relay1 -shft a-
    
  case 97: // a
    Serial.println("Stepper on");
    stp = 1;
    
    break;
  case 98: // b
    Serial.println("Stepper off");
    stp = 0;
    
    break;
  case 105: // i
    Serial.println("i - step 2 LHLL");
       digitalWrite(step1, LOW);
       digitalWrite(step2, HIGH);
       digitalWrite(step3, LOW);
       digitalWrite(step4, LOW);
    stp = 0;
    
    break;
  case 111: // o
    Serial.println("o - step 3 LLHL");
       digitalWrite(step1, LOW);
       digitalWrite(step2, LOW);
       digitalWrite(step3, HIGH);
       digitalWrite(step4, LOW);
    stp = 0;
    
    break;
  case 112: // p
    Serial.println("p - step 4 LLLH");
       digitalWrite(step1, LOW);
       digitalWrite(step2, LOW);
       digitalWrite(step3, LOW);
       digitalWrite(step4, HIGH);
    stp = 0;
    
    break;
  case 117: // u
    Serial.println("u - step 1 HLLL");
       digitalWrite(step1, HIGH);
       digitalWrite(step2, LOW);
       digitalWrite(step3, LOW);
       digitalWrite(step4, LOW);
    stp = 0;
    
    break;
  case 104: // h
    Serial.println("h - HLHL");
       digitalWrite(step1, HIGH);
       digitalWrite(step2, LOW);
       digitalWrite(step3, HIGH);
       digitalWrite(step4, LOW);
    stp = 0;
    
    break;
  case 106: // j
    Serial.println("j - HHLL");
       digitalWrite(step1, HIGH);
       digitalWrite(step2, HIGH);
       digitalWrite(step3, LOW);
       digitalWrite(step4, LOW);
    stp = 0;
    
    break;
  case 107: // k
    Serial.println("k - LHLH");
       digitalWrite(step1, LOW);
       digitalWrite(step2, HIGH);
       digitalWrite(step3, LOW);
       digitalWrite(step4, HIGH);
    stp = 0;
    
    break;
  case 108: // l
    Serial.println("l - LLHH");
       digitalWrite(step1, LOW);
       digitalWrite(step2, LOW);
       digitalWrite(step3, HIGH);
       digitalWrite(step4, HIGH);
    stp = 0;
    
    break;
  case 110: // n
    Serial.println("n - LLLL");
       digitalWrite(step1, LOW);
       digitalWrite(step2, LOW);
       digitalWrite(step3, LOW);
       digitalWrite(step4, LOW);
    stp = 0;
    
    break;
   case 109: // m
    Serial.println("m - HLLL");
       digitalWrite(step1, HIGH);
       digitalWrite(step2, LOW);
       digitalWrite(step3, LOW);
       digitalWrite(step4, LOW);
    stp = 0;
    
    break;
    
  case 116: // t - elevator up 
    liftup = 1;
    liftdown = 0;
    Serial.println("Elevator up");
    break;
  case 121: // y - elevator down
    liftdown = 1;
    liftup = 0;
    Serial.println("Elevator down");
    break;
  case 101: // e - reduce speed
    liftspeed = liftspeed - 10;
    if ( liftspeed < 0 ) {
      liftspeed = 0;
    }
    Serial.print("Lift speed = ");
    Serial.println(liftspeed);
    break;
  case 114: // r - increase speed
    liftspeed = liftspeed + 10;
    if ( liftspeed > 255 ) {
      liftspeed = 255;
    }
    Serial.print("Lift speed = ");
    Serial.println(liftspeed);
    break;
  case 113: // q - elevator status
    Serial.println("Elevator Status");
    if ( liftdown ){
      Serial.println(" Going down.");
    }
    if ( liftup ){
      Serial.println(" Going up.");
    }
    if ( !liftdown && !liftup ){
      Serial.println(" Stopped.");
    }
    if (digitalRead(lifttop)){
      Serial.println(" Elevator is at top.");
    }
    if (digitalRead(liftbot)){
      Serial.println(" Elevator is at bottom.");
    }
    if (digitalRead(lifttop) && digitalRead(liftbot)){
      Serial.println("Elevator ERROR!!");
      Serial.println(" --- *** Stopping *** ---");
      digitalWrite(liftpwr,LOW);
    }
    Serial.print(" Elevator speed is set to = ");
    Serial.println(liftspeed);
    Serial.println(" ");
    Serial.println(" Servo switch is set to = ");
    Serial.println(swv1);
    Serial.println(" ");
    break;
  
  case 115: // s - activate solinoid
    Serial.println("Release the Kraken!!");
    digitalWrite(gate1, HIGH);
    // delay(100); // maybe a delay is required
    digitalWrite(gate1, LOW);
    break;
    
  case 100: // d - servo left
    swv1 = swv1 - 10;
    if ( swv1 < 0 ){ // to far
      Serial.println("Servo switch 1 is Maximum left.");
      swv1 = 0;
    }
    track_switch1.write(swv1);
    break;
  case 102: // f - servo right
    swv1 = swv1 + 10;
    if ( swv1 > 180 ){ // to far
      Serial.println("Servo switch 1 is Maximum right.");
      swv1 = 180;
    }
    track_switch1.write(swv1);
    break;
    
  case 58: // reply that we are  in the control loop
    Serial.println("PING PONG CONTROL");
    break;
    
  case 63:
    Serial.println("-- Commands for PING PONG control --");
    Serial.println("a - start stepper ");
    Serial.println("b - stop stepper ");
    Serial.println("u - set postion  HLLL");
    Serial.println("i - set postion  LHLL");
    Serial.println("o - set postion  LLHL");
    Serial.println("p - set postion  LLLH");
    Serial.println("h - set postion  HHLL");
    Serial.println("j - set postion  HLHL");
    Serial.println("k - set postion  LHLH");
    Serial.println("l - set postion  LLHH");
    Serial.println("n - set position LLLL");
    Serial.println("m - set position HLLL");
    Serial.println(" ");
    Serial.println("t - elevator up ");
    Serial.println("y - elevator down ");
    Serial.println("e - reduce elevator speed ");
    Serial.println("r - increase elevator speed ");
    Serial.println("q - elevator status ");
    Serial.println(" ");
    Serial.println("s - activate solinoid gate ");
    
  
  default:
    //Serial.println(n);
    digitalWrite(led, HIGH); 
    delay(250);              
    digitalWrite(led, LOW);
    delay(250);
  }
  if ( stp == 1 ){
    lift();
  }
}

// beging of stepper lift
void lift (){
    if ( curstp == 0 ){
       digitalWrite(step1, HIGH);
       digitalWrite(step2, LOW);
       digitalWrite(step3, LOW);
       digitalWrite(step4, LOW);
       curstp = 1;
       Serial.println("step 1");
    delay(250);              
    }
    if ( curstp == 1 ){
      digitalWrite(step1, LOW);  
      digitalWrite(step2, LOW);
      digitalWrite(step3, HIGH);
      digitalWrite(step4, LOW);
      curstp = 2;
       Serial.println("step 2");
    delay(250);              
    }
    if ( curstp == 2 ){
      digitalWrite(step1, LOW);
      digitalWrite(step2, HIGH);
      digitalWrite(step3, LOW);
      digitalWrite(step4, LOW);
      curstp = 3;
       Serial.println("step 3");
    delay(250);              
    }
    if ( curstp == 3 ){
      digitalWrite(step1, LOW);
      digitalWrite(step2, LOW);
      digitalWrite(step3, LOW);
      digitalWrite(step4, HIGH);
      curstp = 0;
       Serial.println("step 4");
    delay(250);              
    }
} // end of stepper lift



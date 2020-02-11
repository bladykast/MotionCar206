#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

//Mert Arduino and Tech YouTube Channel -- https://goo.gl/ivcZhW

//Add the necessary libraries
//You can find all the necessary library links in the video description
#include <SPI.h>      //SPI library for communicate with the nRF24L01+
#include "RF24.h"     //The main library of the nRF24L01+

//Define enable pins of the Motors
const int enbA = 8;
const int enbB = 4;

//Define control pins of the Motors
//If the motors rotate in the opposite direction, you can change the positions of the following pin numbers
const int IN1 = 7;    //Right Motor (-)
const int IN2 = 6;    //Right Motor (+)
const int IN3 = 3;    //Left Motor (+)
const int IN4 = 2;    //Right Motor (-)

//Define variable for the motors speeds
//I have defined a variable for each of the two motors 
//This way you can synchronize the rotation speed difference between the two motors
int RightSpd = 150;
int LeftSpd = 150;

//Define packet for the direction (X axis and Y axis)
int data[2];

//Define object from RF24 library - 9 and 10 are a digital pin numbers to which signals CE and CSN are connected
RF24 radio(9,10);

//Create a pipe addresses for the communicate
const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

void setup(){
  //Define the motor pins as OUTPUT
  pinMode(enbA, OUTPUT);
  pinMode(enbB, OUTPUT);
 digitalWrite(enbA, HIGH);
 digitalWrite(enbB,HIGH);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600);
  Serial.print("I work!");
  radio.begin();                    //Start the nRF24 communicate     
  radio.setDataRate( RF24_250KBPS );       
  radio.openReadingPipe(1, thisSlaveAddress);   //Sets the address of the transmitter to which the program will receive data.
  radio.startListening();   
  Serial.print("I got this far!"); 
  }

void loop(){
  if (radio.available()){
    radio.read(data, sizeof(data));
    Serial.print(data[0]);
    Serial.print("      ");
    Serial.print(data[1]);
    Serial.println("");

    if(data[0] > 380){
      //forward            
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    
    if(data[0] < 310){
      //backward             
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
     
    if(data[1] > 180){
      //left
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }

    if(data[1] < 110){
      //right
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }

    if(data[0] > 330 && data[0] < 360 && data[1] > 130 && data[1] < 160){
      //stop car
      //analogWrite(enbA, 0);
      //analogWrite(enbB, 0);
    }
  }
}

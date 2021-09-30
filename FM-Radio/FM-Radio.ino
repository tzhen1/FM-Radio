/**
 ******************************************************************************
   @file           : main.c
   @brief          : Main program body
 ******************************************************************************
   @attention
   source https://github.com/adamjansch/AR1010lib
 ******************************************************************************
*/
#include "ar1010lib.h"
#include <Wire.h> //i2c protocol communicate wit radio module
#include <LiquidCrystal_I2C.h> // lcd

const int buttonFreqUP = 3; //pin
const int buttonFreqDown = 2; //pin
const int buttonVolUp = 6; //pin
const int buttonVolDown = 5; //pin
//buttons orientation check with multimeter continuity
AR1010 radio = AR1010(); // instance of ar1010
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display


double frequencyFM = 1009;//910 100.9(1009) = classical, eagle radio 96.4 , 1058
int VolumeFM = 4;

void setup()
{
  lcd.begin(); // initialize the LCD
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("FM:");
  lcd.print(frequencyFM/10); // /10 to show correct Freq
  lcd.print("MHz"); //

  lcd.setCursor(0, 1);
  lcd.print("Volume:");
  lcd.print(VolumeFM);


  Wire.begin();
  Serial.begin(9600);
  Serial.println("FM Radio");
  radio.initialise();
  radio.setVolume(VolumeFM); // set intial vol which is2
  radio.setFrequency(frequencyFM);

  delay(1000);
  pinMode(buttonFreqUP, INPUT); //sets digital pin 3 as input
  pinMode(buttonFreqDown, INPUT);
  pinMode(buttonVolUp, INPUT);
  pinMode(buttonVolDown, INPUT);
  Serial.println(radio.frequency());
}

void loop() {
  if (digitalRead(buttonFreqUP) == LOW) //if digital pin on, current run through
  {
    frequencyUP();
    lcdUpdate();
  }


  if (digitalRead(buttonFreqDown) == LOW) //if digital pin on, current run through
  {
    frequencyDown();
    lcdUpdate();
  }

  if (digitalRead(buttonVolUp) == LOW) //if digital pin on, current run through
  {
    volumeUP();
    lcdUpdate();
  }

  if (digitalRead(buttonVolDown) == LOW) //if digital pin on, current run through
  {
    volumeDown();
    lcdUpdate();
  }

  delay(300); // when pressed, hold button for atleast 300ms
}

void lcdUpdate()//update LCD vol and freq
{
  lcd.clear();

  lcd.setCursor(0, 0); // first row
  lcd.print("FM:");
  lcd.print(frequencyFM/10); // set to new freq if changed below, prints /10 cuz 1058 -> 105.8
  lcd.print("MHz"); 

  lcd.setCursor(0, 1); // 2nd row
  lcd.print("Volume:");
  lcd.print(VolumeFM);
}


void volumeUP() // get more pull up
{
  if (VolumeFM < 15) //if less than greatest
  {
    delay(500); // delay for button press
    VolumeFM ++; // increment volume by 1
    radio.setVolume(VolumeFM); // set vol by the increment
    Serial.println("button pressed vol up"); // check on serial monitor if pressed
  }
}
void volumeDown()
{
  if (VolumeFM > 0) // if >0 , so dont go negative
  {
    delay(500);
    VolumeFM --; // vol decreased by 1
    radio.setVolume(VolumeFM);
    Serial.println("button pressed down up");
  }
}
void frequencyUP()
{
  radio.seek('u'); // seek next station , 'u' shown in library
  delay(500); // delay for button press
  Serial.println("button pressed freq up"); // check on serial monitor if pressed
  Serial.println(radio.frequency());
  frequencyFM = radio.frequency(); // sets global variable to this, so it can be used to change on LCD

}
void frequencyDown()
{
  radio.seek('d'); //seek down
  delay(500);
  Serial.println("button pressed freq down");
  Serial.println(radio.frequency());
  frequencyFM = radio.frequency();

}

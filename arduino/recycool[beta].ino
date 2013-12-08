
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 4, 5, 6, 0, 1, 2, 3, 7, NEGATIVE);  // Set the LCD I2C address

//top
const int top = 2;
int top_switch = 0; // closed

//volume

const int trigger = 7;
const int echo = 8;



void setup() 
{
  Serial.begin(9600);  

  lcd.begin(16,2);

  lcd.print("Recycool");
  lcd.setCursor(0,1);
  
  pinMode(top, INPUT);
  digitalWrite(top, HIGH);    // Activate internal pullup resistor
  
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  
}


void loop() 
{
  
  top_switch = digitalRead(top);
  lcd.setCursor(0,1);
  if(top_switch == LOW) { // top closed
    Serial.println("closed");
    lcd.print("closed");
  } else {
    Serial.println("open"); // top open
    lcd.print("opened");
  }
  
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds( 10 );
  digitalWrite( trigger, LOW);
  
  long duration = pulseIn( echo, HIGH );
  long distance = 0.034 * duration / 2;
  
  if( distance < 0 || distance > 75 ){
     distance = 75;
  }
  
  lcd.setCursor(10,1);
  lcd.print("    ");
  lcd.setCursor(10,1);
  lcd.print(distance);
  Serial.println(distance); 
  
  
  
  
  delay(500);
}





#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x9E, 0x3E };
char server[] = "192.168.178.34";    // name address for Google (using DNS)
IPAddress ip(192,168,178,200);
int i = 0;
static uint32_t timer;

EthernetClient client;

LiquidCrystal_I2C lcd(0x20, 4, 5, 6, 0, 1, 2, 3, 7, NEGATIVE);  // Set the LCD I2C address

//top
const int top = 2;
int top_status = 0; // closed

//volume

const int trigger = 7;
const int echo = 8;

//weight

const int weight = A0;
float weight_input = 0;
float weight_val = 0;


void setup() 
{
  Serial.begin(9600);  

  lcd.begin(16,2);

  lcd.print("Recycool");
  lcd.setCursor(0,1);
  
  pinMode(top, INPUT);
  pinMode(weight, INPUT);
  digitalWrite(top, HIGH);    // Activate internal pullup resistor
  
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  if (client.connect(server, 80)) {
    Serial.println('Ok, creazione client effettuata');
  }  
}


void loop() 
{
  if (millis() > timer) { 
    
    //Status
    top_status = digitalRead(top);
    lcd.setCursor(0,1);
    if(top_status == LOW) { // top closed
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
    
    //Distance
    long distance = 0.034 * duration / 2;
    
    if( distance < 0 || distance > 75 ){
     distance = 75;
    }
    
    weight_input = analogRead(weight);
    
    //Weight
    weight_val = mapfloat(weight_input, 17, 27, 0.1, 0.2);
    
    httpRequest(distance); 
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }    
     
    timer = millis() + 5000;
  }
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void httpRequest(long distance)
{
  
      String distance_str = String(distance, DEC);
      String test = String(i);
     
      String baseurl = "GET /arduino?t=";
      String endurl = " HTTP/1.1";
  
      delay(1000);
      Serial.println(baseurl + test + endurl);
      // Make a HTTP request:
      //client.println(baseurl + test + endurl);
      client.println("GET /arduino?status=power-on&load=50&volume=30&weight=90 HTTP/1.1");
      client.println("Host: 192.168.178.34");
      client.println("Connection: close");
      client.println();
      
      i++;
  
}




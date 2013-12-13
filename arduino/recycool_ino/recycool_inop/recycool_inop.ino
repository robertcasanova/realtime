
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

byte mac[]    = {  0x90, 0xA2, 0xDA, 0x00, 0x9E, 0x3E };
IPAddress server(192,168,1,10); // Google
IPAddress ip(192,168,1,200);
IPAddress myDns(192,168,1,10);
IPAddress gateway(192,168,1,10);

EthernetClient client;

unsigned long prevTime = 0;          // last time you connected to the server, in milliseconds
unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds

boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 10*1000;  // delay between updates, in milliseconds




LiquidCrystal_I2C lcd(0x20, 4, 5, 6, 0, 1, 2, 3, 7, NEGATIVE);  // Set the LCD I2C address

//top
const int top = 2;
int top_switch = 0; // closed

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
  
  Ethernet.begin(mac, ip, myDns, gateway);
  
  delay(1000);
    
  

  
}


void loop() 
{
  if( millis() - prevTime > 1000 ) {
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
    
    weight_input = analogRead(weight);
    weight_val = mapfloat(weight_input, 17, 27, 0.1, 0.2);
    
    
    lcd.setCursor(10,1);
    lcd.print("    ");
    lcd.setCursor(10,1);
    lcd.print(weight_val);
    Serial.println(weight_val);
    prevTime = millis();

  }
  
  
  
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  
  if(!client.connected() && millis() - lastConnectionTime > postingInterval ) {
    httpRequest();
  }
  
  lastConnected = client.connected();

}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /arduino?volume=10 HTTP/1.1");
    client.println("User-Agent: arduino");
    client.println("Connection: close");
    client.println();

    lastConnectionTime = millis();
    // note the time that the connection was made:
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}



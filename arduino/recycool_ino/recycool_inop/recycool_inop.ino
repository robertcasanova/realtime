
#include <SPI.h>
#include <Dhcp.h>
#include <Ethernet.h>
#include <util.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x9E, 0x3E };

IPAddress ip(10,10,9,53);
char server[] = "10.10.9.73";    
EthernetClient client;

boolean lastConnected = false;                 // state of the connection last time through the main loop
unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 1 * 500;  // delay between updates, in milliseconds

unsigned long lastSensorValues = 0;                 // state of the connection last time through the main loop
const unsigned long sensorInterval = 1 * 500;  // delay between sensor values, in milliseconds

LiquidCrystal_I2C lcd(0x20, 4, 5, 6, 0, 1, 2, 3, 7, NEGATIVE);  // Set the LCD I2C address
  
String tmp = "";
String hum = "";

//Sensors

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

//distance
long distance = 0;

//recycool status
String rStatus = "power_off";

void setup(void) {

   // start serial port
  Serial.begin(9600);  
  delay(3000);
  
  lcd.begin(16,2);

  lcd.print("Recycool");
  lcd.setCursor(0,1);
  
  pinMode(top, INPUT);
  pinMode(weight, INPUT);
  digitalWrite(top, HIGH);    // Activate internal pullup resistor
  
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);  

  //status acceso
  rStatus = "power_on";
  
  httpRequest();
  
  delay(3000);
}

void httpRequest() {

  //load, volume, weight


  Ethernet.begin(mac, ip);
  delay(1000);

  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    int normalized_distance = map((70-distance), 0,70,0,100);
    client.println("GET /arduino?status="+ rStatus + "&load=" + normalized_distance + "&volume=" + ( (70-distance) * 625 / 1000 ) + "&weight="+ (int) (weight_val * 1000) +" HTTP/1.1");
    client.println("Host: 192.168.0.1");
    client.println("Connection: close");
    client.println("User-Agent: arduino");
    client.println();

  }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }

  // note the time that the connection was made:
  lastConnectionTime = millis();

}

void getSensorValues(void) {
  
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
    
    long temp_dist = 0.034 * duration / 2;
    
    distance = temp_dist;
    
    if( distance < 0 || distance > 70 ){
     distance = 70;
    }
    
    weight_input = analogRead(weight);
    weight_val = mapfloat(weight_input, 17, 27, 0.1, 0.2);
    
    
    lcd.setCursor(10,1);
    lcd.print("    ");
    lcd.setCursor(10,1);
    lcd.print(weight_val);
    Serial.println(weight_val);
  
    lastSensorValues = millis(); 
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop(void) { 
  
    if(top_switch == LOW) {
      rStatus = "update";
    } else {
      rStatus = "top_open";
    }

   if (client.available()) {
       char c = client.read();
       Serial.print(c);
   }

   // if there's no net connection, but there was one last time
   // through the loop, then stop the client:
   if (!client.connected() && lastConnected) {
       Serial.println();
       Serial.println("disconnecting.");
       client.stop();
   }

  
   if(millis() - lastSensorValues > sensorInterval) {      
        getSensorValues();
   }
   

   // if you're not connected, and ten seconds have passed since
   // your last connection, then connect again and send data:
   if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
       httpRequest();
   }
   
   // store the state of the connection for next time through
   // the loop:
   lastConnected = client.connected();
}




 

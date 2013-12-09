/*
  Recycool.h - Library for create IOT bins.
  Created by Robert Casanova, Fabrizio Calderan and Enrico Antonello, December, 2013.
  
  This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. 
  To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/deed.en_US.

*/

#ifndef Recycool_h
#define Recycool_h

#include "Arduino.h"
#include "SPI.h"
#include "Ethernet.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"

class Recycool 
{
	public:
		Recycool();
		int setupConnection(uint8_t *mac_address, IPAddress server, char* route);
		int setupTop(uint8_t pin);
		int setupWeight(uint8_t pin);
		int setupLCD(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs, 
                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                     uint8_t backlighPin, t_backlighPol pol); //simplify
		int setupDistance(uint8_t trigger, uint8_t echo);
		int getWeight();
		int getDistance();
		int getStatus();
		int sendData();

	private:
		void _httpRequest(IPAddress server, char* route);

};

#endif
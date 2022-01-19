#include <Arduino.h>

// Scan the I2C bus + repeatedly send the CD eject command over PC-Link to a HiFi deck

// ESP32 I2C Scanner
// Based on code of Nick Gammon

#include <Wire.h>

void receiveEvent(int howMany);

void Scanner ()
{
  Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;

  //Wire.begin();
  for (byte i = 0; i < 128; i++)
  {
    Wire.beginTransmission (i);          // Begin I2C transmission Address (i)
    if (Wire.endTransmission () == 0)  // Receive 0 = success (ACK response) 
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);     // PCF8574 7 bit address
      Serial.println (")");
      count++;
    }
  }
  Serial.print ("Found ");      
  Serial.print (count, DEC);        // numbers of devices
  Serial.println (" device(s).");
}

void setup()
{
  Serial.begin (115200);  
  Wire.begin (21, 22);   // sda= GPIO_21 /scl= GPIO_22
  //Wire.setClock(10000);
  //Wire.onReceive(receiveEvent); // register event
  delay(1000);
  Scanner();
  delay(1000);
}

void loop()
{
  //Scanner ();

  delay(5000);

  Serial.println("Sending command");

  Wire.beginTransmission(0x0);
  Wire.write("\x20\x90\x54\x00\x01");
  Wire.endTransmission();

/*
  int b = Wire.requestFrom(0x0, 1, 1); 
  delay(100);

  if (Wire.available() > 0) {
      receiveEvent(0);
      delay(100);
  }
  */

/*
  for (int i=0; i<128; i++) {
    /*
    Serial.println(i);
    delay(10);
    Wire.beginTransmission(0x0);
    Wire.write(i);
    Wire.write(i);
    Wire.endTransmission();* /

    
    int b = Wire.requestFrom(i, 256, 1); 

    if (b > 0) {
      Serial.print("got bytes: ");
      Serial.println(b);
    }
    delay(100);


    if (Wire.available() > 0) {
      
      receiveEvent(0);
      delay(100);
    }
  }*/
}


void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c, HEX); // print the character
  }
  int x = Wire.read(); // receive byte as an integer
  Serial.println(x, HEX); // print the integer
}

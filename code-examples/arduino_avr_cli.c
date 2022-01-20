#include <Arduino.h>
#include <Wire.h>

// use this from a AVR board to talk and explore the pc-link bus
// send commands written in hexadecimal over the serial board (hex chars in capitals), spaces are ignored
// all data from the I2C bus will be outputed


void receiveEvent(int howMany);

uint8_t charToHexDigit(uint8_t c)
{
  if (c >= 'A')
    return c - 'A' + 10;
  else
    return c - '0';
}

uint8_t hexToByte(uint8_t a, uint8_t b)
{
  return charToHexDigit(a) * 16 + charToHexDigit(b);
}

void setup()
{
  Serial.begin (0x2);  
  TWAR = (0x2 << 1) | 1;  // enable broadcasts to be received
  
  Wire.begin();//2,3);// (21, 22);   // address=0x01, sda= GPIO_21,  scl= GPIO_22
  Wire.setClock(400000);
  Wire.onReceive(receiveEvent); // register event

  delay(1000);
 
 Serial.println("Hello!");
}

//"dispinfo":  "20 C2 D2",
//20 C0 50 04 FF 00 switch to md
//20 90 54 00 01
//20 B8 C1 00 63 00 05 48 65 6C 6C 6F 20 57 6F 72 6C 64 00 00 00
//uint8_t CMD_HELLO[] = {0x1,0xb8,0xc1,0x00,0x63,0x00,0x05,0x48,0x65,0x6c,0x6c,0x6f,0x20,0x57,0x6f,0x72,0x6c,0x64,0,0,0};
//uint8_t CMD_EJECT[] = {0x20,0x90,0x54,0x00,0x01};



void loop()
{
  if (Serial.available() > 0) {
    Serial.print("Receving input: ");
    Wire.beginTransmission(0x00);
    
    while (Serial.available() > 0) {
       int c1 = Serial.read();

       if (c1 == ' ')
         continue;
       
       int c2 = Serial.read();

       if (c2 == -1) 
         c2 = 0;

       uint8_t b = hexToByte(c1, c2);
       Serial.print(b, HEX);
       Serial.print(" ");
       
       Wire.write(b);
    }

    Serial.println("\nSending command");
    Wire.endTransmission(true);
  }

/*
  Wire.requestFrom(0x10, 1, true);
  delay(20); 

  if (Wire.available() > 0) {
    //Serial.println("RReceiving data from I2C: ");
    while (Wire.available() > 0) { // loop through all but the last
      uint8_t c = Wire.read(); // receive byte as a character
      Serial.print(c, HEX); // print the character
      Serial.print(" ");
    }
    Serial.println("");
  }*/

  delay(100);
}


void receiveEvent(int howMany) {
  Serial.print("IN: ");
  int count = 0;
  
  while (Wire.available() > 0) { // loop through all but the last
    count++;
    uint8_t c = Wire.read(); // receive byte as a character
    Serial.print(c, HEX); // print the character
    Serial.print(" ");
  }

  if (count > 0) {
    Serial.println("");
    if (count < howMany)
      Serial.println("WARN: didn't receive all bytes");
  }
}


// Found address: 9 (0x9)
// Found address: 16 (0x10)

//*/


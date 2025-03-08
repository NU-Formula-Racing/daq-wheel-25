#include <Arduino.h>
#include <iostream>
#include <bitset>
/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete instructions at https://RandomNerdTutorials.com/esp32-uart-communication-serial-arduino/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/
// sender code
// Define TX and RX pins for UART (change if needed)
#define TXD1 19
#define RXD1 21

// Use Serial1 for UART communication
HardwareSerial mySerial(1);

int counter = 0;

void setup() {
  //Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);  // UART setup
  
  Serial.println("ESP32 UART Transmitter");
  
}

String convertToBinary(int n) {
    String binaryString = "";

    if (n == 0) {
        return "00000000";  // Special case for 0
    }

    while (n > 0) {
        int remainder = n % 2;
        binaryString = String(remainder) + binaryString;  // Prepend to the string
        n /= 2;
    }

    // Pad with leading zeros to make it 8 digits
    while (binaryString.length() < 8) {
        binaryString = "0" + binaryString;
    }

    return binaryString;
}




void loop() {
  
String tempArray[8][4];
for (int j = 0; j < 8; j++) {
  for (int i = 0; i < 4; i++) {
        String binary = convertToBinary(random(0,256));
        tempArray[j][i] = binary;  // Generate a random number between 0 and 255
    }
}
   
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 4; i++) {
      mySerial.println(tempArray[j][i]);
      mySerial.println(",");
    }
    mySerial.println("----");
  }

  mySerial.println("---END OF TEMP BLOCK---");
  
  delay(1000); 
}
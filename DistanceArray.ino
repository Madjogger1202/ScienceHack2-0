/*
  Read an 8x8 array of distances from the VL53L5CX
  By: Nathan Seidle
  SparkFun Electronics
  Date: October 26, 2021
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  This example shows how to read all 64 distance readings at once.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/18642

*/

#include <Wire.h>

#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX

SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData; // Result data class structure, 1356 byes of RAM
int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output

void setup()
{
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT); 
  digitalWrite(16, 0);
  digitalWrite(17, 1);
  digitalWrite(18, 1);
  digitalWrite(19, 0);
  delay(100);
  Serial.begin(115200);
  delay(1000);
  Serial.println("SparkFun VL53L5CX Imager Example");
Wire.setSDA(20);
  Wire.setSCL(21);

  Wire.begin(); //This resets to 100kHz I2C
  Wire.setClock(400000); //Sensor has max I2C freq of 400kHz 
  
  Serial.println("Initializing sensor board. This can take up to 10s. Please wait.");
 if (myImager.begin() == false)
  {
    Serial.println(F("Sensor not found - running scanner"));
  }
  else
  {
    while (Serial.available()) //Clear incoming key presses
      Serial.read();

    Serial.println(F("Press any key to set device address to 0x44"));
    int deviceAddress = 0x44; //Valid: 0x08 <= address <= 0x77
    if (myImager.setAddress(deviceAddress) == false)
    {
      Serial.println(F("Device failed to set new address. Please try again. Freezing..."));
      while (1);
    }
    delay(500);
    int newAddress = myImager.getAddress();
    myImager.setResolution(8 * 8); //Enable all 64 pads
    imageResolution = myImager.getResolution(); //Query sensor for current resolution - either 4x4 or 8x8
    imageWidth = sqrt(imageResolution); //Calculate printing width
    myImager.startRanging();
  }
  myImager.setResolution(8 * 8); //Enable all 64 pads
  imageResolution = myImager.getResolution(); //Query sensor for current resolution - either 4x4 or 8x8
  imageWidth = sqrt(imageResolution); //Calculate printing width
  myImager.setRangingFrequency(15);
    myImager.startRanging();
  
}

void loop()
{
  //Poll sensor for new data
  digitalWrite(16, 0);
  digitalWrite(17, 1);
  digitalWrite(18, 1);
  digitalWrite(19, 0);
  delay(100);
  digitalWrite(16, 0);
  analogWrite(17, 0);
  digitalWrite(18, 0);
  digitalWrite(19, 0);
  if (myImager.isDataReady() == true)
  {
    if (myImager.getRangingData(&measurementData)) //Read distance data into array
    {
      //The ST library returns the data transposed from zone mapping shown in datasheet
      //Pretty-print data with increasing y, decreasing x to reflect reality
      for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
      {
        for (int x = imageWidth - 1 ; x >= 0 ; x--)
        {
          Serial.print("\t");
          Serial.print(measurementData.distance_mm[x + y]);
        }
        Serial.println();
      }
      Serial.println();
    }
  }
  
  if((measurementData.distance_mm[27]<=300)&&((measurementData.distance_mm[24]-measurementData.distance_mm[27]>=30)&&(measurementData.distance_mm[31]-measurementData.distance_mm[27]>=30)))
  {
    digitalWrite(16, 0);
  digitalWrite(17, 1);
  digitalWrite(18, 0);
  digitalWrite(19, 1);
  delay(1000);
  digitalWrite(16, 0);
  analogWrite(17, 0);
  digitalWrite(18, 0);
  digitalWrite(19, 0);
  }
  if((measurementData.distance_mm[28]<=300)&&((measurementData.distance_mm[24]-measurementData.distance_mm[28]>=30)&&(measurementData.distance_mm[31]-measurementData.distance_mm[28]>=30)))
  {
    
    digitalWrite(16,0);
  digitalWrite(17,1);
  digitalWrite(18, 0);
  digitalWrite(19, 1);
  delay(1000);
  digitalWrite(16, 0);
  analogWrite(17, 0);
  digitalWrite(18, 0);
  digitalWrite(19, 0);
  }
  if((measurementData.distance_mm[26]<=300)&&((measurementData.distance_mm[24]-measurementData.distance_mm[26]>=30)&&(measurementData.distance_mm[31]-measurementData.distance_mm[26]>=30)))
  {
    digitalWrite(16, 0);
  digitalWrite(17, 1);
  digitalWrite(18, 0);
  digitalWrite(19, 1);
  delay(1000);
  digitalWrite(16, 0);
  analogWrite(17, 0);
  digitalWrite(18, 0);
  digitalWrite(19, 0);
  }
  if((measurementData.distance_mm[29]<=300)&&((measurementData.distance_mm[24]-measurementData.distance_mm[29]>=30)&&(measurementData.distance_mm[31]-measurementData.distance_mm[29]>=30)))
  {
    
    digitalWrite(16,0);
  digitalWrite(17,1);
  digitalWrite(18, 0);
  digitalWrite(19, 1);
  delay(1000);
  digitalWrite(16, 0);
  analogWrite(17, 0);
  digitalWrite(18, 0);
  digitalWrite(19, 0);
  }
       

  delay(5); //Small delay between polling
}

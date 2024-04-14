#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <nRF24L01.h>
#include <RF24.h>
#define I2C_SDA 20
#define I2C_SCL 21 
#include <SPI.h>
#include "printf.h"
#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX
//SparkFun_VL53L5CX myImager;
//VL53L5CX_ResultsData measurementData; // Result data class structure, 1356 byes of RAM
//int imageResolution = 0; //Used to pretty print output
//int imageWidth = 0; //Used to pretty print output

//////////////////////////////////////////
#define NRF_CHANNEL 76                 // you can choose in range 0-127
#define NRF_DATA_RATE RF24_250KBPS      // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
#define NRF_PA_LEVEL RF24_PA_LOW        // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm
#define NRF_PIPE  0x1234567899LL        // 
#define NRF_AUTO_ASK false              //
#define NRF_ENABLE_READING_PIPE false   //
#define NRF_READING_PIPE 0x9987654321LL //
//////////////////////////////////////////


RF24 radio(22, 28);  

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
struct radioData
{
  bool stable;
  int8_t x_acs;
  int8_t y_acs;
  int8_t z_acs;

  uint8_t mode;
  uint8_t channel;

  uint16_t button;
  
  uint16_t potData;
  uint16_t joyX;
  uint16_t joyY;

} telemetriData;

float target = -1000;
float pos = 0;

void GO_F(){
  digitalWrite(6, 0);
  digitalWrite(7, 1);
  digitalWrite(8, 0);
  digitalWrite(9, 1);  
}
void GO_B(){
  digitalWrite(6, 1);
  digitalWrite(7, 0);
  digitalWrite(8, 1);
  digitalWrite(9, 0);  
}
void GO_L(){
  digitalWrite(6, 1);
  digitalWrite(7, 0);
  digitalWrite(8, 0);
  digitalWrite(9, 1);  
}
void GO_R(){
  digitalWrite(6, 0);
  digitalWrite(7, 1);
  digitalWrite(8, 1);
  digitalWrite(9, 0);  
}

void NO_GO(){
  digitalWrite(6, 0);
  digitalWrite(7, 0);
  digitalWrite(8, 0);
  digitalWrite(9, 0);  
}

void setup(void)
{
  delay(2000);
  SPI.begin();
  radio.begin();                      //
  radio.setChannel(100);                               
  radio.setDataRate(RF24_1MBPS);                   
  radio.setPALevel(RF24_PA_LOW); 
  radio.setAutoAck(false);
  radio.openReadingPipe(1, 0x1234567899LL);
  
  mag.enableAutoRange(true);
  Wire.setSDA(20);
  Wire.setSCL(21);
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(9600);
  Serial.println("Initializing sensor board. This can take up to 10s. Please wait.");
  radio.printDetails();//if (myImager.begin() == false)
  //{
  //  Serial.println(F("Sensor not found - check your wiring. Freezing"));
  //  while (1) ;
  //}
  //myImager.setResolution(8*8); //Enable all 64 pads
  
  //imageResolution = myImager.getResolution(); //Query sensor for current resolution - either 4x4 or 8x8
  //imageWidth = sqrt(imageResolution); //Calculate printing width

  //myImager.startRanging();
  mag.begin();
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT); 

}

void loop(void)
{
  //if (myImager.isDataReady() == true)
  //{
  //  if (myImager.getRangingData(&measurementData)) //Read distance data into array
   // {
      //The ST library returns the data transposed from zone mapping shown in datasheet
      //Pretty-print data with increasing y, decreasing x to reflect reality
   //   for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
   //   {
   //     for (int x = imageWidth - 1 ; x >= 0 ; x--)
   //     {
   //       Serial.print("\t");
   //       Serial.print(measurementData.distance_mm[x + y]);
   //     }
   //     if(measurementData.distance_mm[24]-measurementData.distance_mm[27]>=30)&&(measurementData.distance_mm[31]-measurementData.distance_mm[27]>=30)
   //       pos=event.magnetic.x-18;
   //     if(measurementData.distance_mm[24]-measurementData.distance_mm[28]>=30)&&(measurementData.distance_mm[31]-measurementData.distance_mm[28]>=30)
   //       pos=event.magnetic.x-18;
   //     Serial.println();
   //   }
    //  Serial.println();
  //  }
 // }
  /* Get a new sensor event */
  if (radio.available()) {      // is there a payload?
      radio.read(&telemetriData, sizeof(telemetriData));  // fetch payload from FIFO
      Serial.println("radio");
      if((telemetriData.button == 0)&&(target >=-100))
      {
        sensors_event_t event;
        mag.getEvent(&event);
        pos=event.magnetic.x;
        if(pos > target)
        {
          while(pos > target)
          {
            sensors_event_t event;
            mag.getEvent(&event);
            pos=event.magnetic.x;
            GO_R();
            delay(100);
            NO_GO();
          }
        }
        else
        {
          while(pos > target)
          {
            sensors_event_t event;
            mag.getEvent(&event);
            pos=event.magnetic.x;
            GO_L();
            delay(100);
            NO_GO();
          }
        }
      }
      else if(telemetriData.button == 1)
      {
        GO_L();
        Serial.println("GO_L");
        delay(100);
        NO_GO();
      }
      else if(telemetriData.button == 10)
      {
        GO_R();
        Serial.println("GO_R");
        delay(100);
        NO_GO();
      }
      else if(telemetriData.button == 100)
      {
        GO_F();
        Serial.println("GO_F");
        delay(100);
        NO_GO();
      }
      else if(telemetriData.button >= 101)
      {
        GO_B();
        Serial.println("GO_B");
        delay(100);
        NO_GO();
      }
    }
  sensors_event_t event;
  mag.getEvent(&event);
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println(radio.available());
  delay(500);
}

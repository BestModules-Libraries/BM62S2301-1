/*****************************************************************
File:             readAirVelocity.ino
Description:      Please use 3.3V to power the sensor, 
                  otherwise there may be deviation in accuracy.
                  This function reads three types of data: 
                  Air Velocity AD Counts,m/s,and mph
******************************************************************/
#include "BM62S2301-1.h"

BM62S2301_1 AVS; //Please uncomment out this line of code if you use Wire
//BM62S2301_1 AVS(&Wire1);//Please uncomment out this line of code if you use Wire1 on BMduino
//BM62S2301_1 AVS(&Wire2);//Please uncomment out this line of code if you use Wire2 on BMduino

void setup()
{
  Serial.begin(9600);
  AVS.begin();
  Serial.println("Air Velocity Sensor Module");
  Serial.println("BM62S2301-1 has been powered on");
  Serial.println("===========================================");
}
  
  
void loop()
{
    Serial.print("ADC count: ");
    Serial.print(AVS.readRawAirVelocity()); 
    
    Serial.print("\tm/s: ");
    Serial.print(AVS.readAirVelocity(1));
    
    Serial.print("\tmph: ");
    Serial.println(AVS.readAirVelocity(2)); 
    
    delay(500); 
}

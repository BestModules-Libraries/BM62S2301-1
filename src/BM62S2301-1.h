/*****************************************************************
File:             BM62S2301-1.h
Author:           BEST MODULES CORP.
Description:      Define classes and required variables
Version：         V1.0.2   --2024-08-01
******************************************************************/
#ifndef _BM62S2301_1_H_
#define _BM62S2301_1_H_

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define BM62S2301_1_ADDRESS     0x28    // Note, the BME26M301 does not have an adjustable address.
#define CHECK_OK                0x01
#define CHECK_ERROR             0x02
#define TIMEOUT_ERROR           0x03
#define AIRFLOW_RANGE_7_MPS     0x00    // 
#define AIRFLOW_RANGE_15_MPS    0x01    // 

#define Normal_DATA_SIZE 3
#define Raw_DATA_SIZE 6

class BM62S2301_1
{
   public:
    BM62S2301_1(TwoWire *theWire =&Wire , uint8_t iic_addr=BM62S2301_1_ADDRESS);
    void begin();
    uint16_t readRawAirVelocity();
    float readAirVelocity(uint8_t uintChoose=1);
    
   private:
    void setRange(uint8_t range);
    uint8_t readBytes(uint8_t rbuf[], uint8_t rlen);
    uint8_t _i2cAddr;
    TwoWire *_wire = NULL;
    int Air_speed;
    float  Air_speed_mps;
    float  Air_speed_mph;
    uint8_t buff[6];    //  6 Bytes Buffer
    uint8_t  _range = AIRFLOW_RANGE_15_MPS; // defaults range
    SoftwareSerial *_softSerial = NULL ;
    float _mpsDataPoint[10] = {0, 1.00, 2.00, 3.00, 4.00, 5.00, 7.50, 10.00, 12.50,  15.00}; // defaults
    int _rawDataPoint[10] =  {409, 834, 1215, 1554, 1856, 2123, 2664, 3068, 3391, 3686}; // defaults
};
#endif

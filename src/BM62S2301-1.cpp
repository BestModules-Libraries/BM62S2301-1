/*****************************************************************
File:          BM62S2301-1.cpp
Author:        BEST MODULES CORP.
Description:   The sensor with IIC and obtain the corresponding value
Version：      V1.0.1   --2024-06-05
**********************************************************/
#include "BM62S2301-1.h"
/*************************************************
Description:  Constructor
Parameters:        *theWire: Wire object if your board has more than one I2C interface 
                   addr：Module IIC address              
Return:             
Others:             
*************************************************/
BM62S2301_1::BM62S2301_1(TwoWire *theWire, uint8_t iic_addr)
{
  _wire = theWire;
  _i2cAddr=iic_addr;
}

/**********************************************************
Description: Initialize IIC connection
Parameters:  range：0：sensor range  0~7.23m/s
                    1：sensor range  0~15.0m/s
Return:      void   
Others:      none
**********************************************************/
void BM62S2301_1::begin()
{
  if (_wire != NULL)
  {
      _wire->begin();
      setRange(AIRFLOW_RANGE_15_MPS);
  }
}



/**********************************************************
Description: read AirVelocity AD Count
Parameters:       none
Return:      Air Velocity AD Count(2byte)     
Others:     Read the digital output data of the AirVelocity Sensor  
**********************************************************/
uint16_t BM62S2301_1::readRawAirVelocity()
{
  uint8_t check;
  check = readBytes(buff,3);
  delay(10);
  if(check == CHECK_OK)
  {
    Air_speed = (buff[1]*256)+buff[2];
    
  }
  else 
  {
    Air_speed = 0;
  }
  return Air_speed;
}
/**********************************************************
Description: read AirVelocity MPH/MPS
Parameters:  1/2 
             1 return MPS  m/s
             2 return MPH
Return:      float data   
Others:      Read the output data of the AirVelocity Sensor  
**********************************************************/
float BM62S2301_1::readAirVelocity(uint8_t uintChoose)
{
  uint8_t check;
  uint8_t dataPointsNum = 10; 
  check = readBytes(buff,3);
  delay(10);
  if(check == CHECK_OK)
  {
    Air_speed = (buff[1]*256)+buff[2];
  }
  else 
  {
    Air_speed = 0;
  }
    int data_position = 0;
    if(Air_speed <= 409) Air_speed_mps = 0;
    else if(Air_speed<3686)
    {
      for( int i = 0 ; i < dataPointsNum ; i++)
      {
          if (Air_speed > _rawDataPoint[i])
          {
            data_position = i;
          }
      }
      int window_size = (_rawDataPoint[data_position+1] - _rawDataPoint[data_position]);
      int diff = (Air_speed - _rawDataPoint[data_position]);
      float percentage_of_window = ((float)diff / (float)window_size);

      float window_size_mps = (_mpsDataPoint[data_position+1] - _mpsDataPoint[data_position]);
      Air_speed_mps = _mpsDataPoint[data_position] + (window_size_mps*percentage_of_window); 
    }
    else
    {
      if(_range == AIRFLOW_RANGE_7_MPS)  Air_speed_mps = 7.23;
      if(_range == AIRFLOW_RANGE_15_MPS) Air_speed_mps =  15.00;
    }
  if(uintChoose==1)
  {
    return Air_speed_mps;    
  }
  if(uintChoose==2)
  {
    Air_speed_mph = Air_speed_mps*2.236936;
    return Air_speed_mph;
  }
    return 0;  
}
/**********************************************************
Description: Read data through I2C
Parameters: rbuf: Used to store received data
            rlen: Length of data to be read
Return:     0:OK
            1:CHECK_ERROR
            2: timeout error
Others: None
**********************************************************/
uint8_t BM62S2301_1::readBytes(uint8_t rbuf[], uint8_t rlen)
{
  uint8_t i = 0, checkSum = 0;
  _wire->requestFrom(_i2cAddr, rlen);
  if (_wire->available() == rlen)
  {
      for (i = 0; i < rlen; i++)
      {
          rbuf[i] = _wire->read();
      }
  }
  else
  {
      return TIMEOUT_ERROR;
  }
  /* Check Sum */
  checkSum = rbuf[0] + rbuf[1] + rbuf[2];
  if (checkSum == 0)
  {
      return CHECK_OK; // Check correct
  }
  else
  {
      return CHECK_ERROR; // Check error
  }
}
/**********************************************************
Description: Set sensor range(0~7.23m/s  or  0~15.0m/s)
Parameters:  range：0：sensor range  0~7.23m/s
                    1：sensor range  0~15.0m/s
Return:      void   
Others:     The sensor BM62S2301-1 has a range of 0-15m/s, 
            and a sensor with a range of 0-7.23m/s will be opened in the future
**********************************************************/
void BM62S2301_1::setRange(uint8_t range)
{
    const float mpsDataPoint_7_mps[9] = {0, 1.07, 2.01, 3.00, 3.97, 4.96, 5.98, 6.99, 7.23}; 
    const int rawDataPoint_7_mps[9] =  {409, 915, 1522, 2066, 2523, 2908, 3256, 3572, 3686};
    
    const float mpsDataPoint_15_mps[10] = {0, 1.00, 2.00, 3.00, 4.00, 5.00, 7.50, 10.00, 12.50,  15.00}; 
    const int rawDataPoint_15_mps[10] =  {409, 834, 1215, 1554, 1856, 2123, 2664, 3068, 3391, 3686}; 
    
    _range = range;
    if (_range == AIRFLOW_RANGE_7_MPS)
    {
      for (int i = 0 ; i < 9 ; i++)
      {
        _mpsDataPoint[i] = mpsDataPoint_7_mps[i];
        _rawDataPoint[i] = rawDataPoint_7_mps[i];
      }
    }
    else if (_range == AIRFLOW_RANGE_15_MPS)
    {
      for (int i = 0 ; i < 10 ; i++)
      {
        _mpsDataPoint[i] = mpsDataPoint_15_mps[i];
        _rawDataPoint[i] = rawDataPoint_15_mps[i];
      }
    }
}

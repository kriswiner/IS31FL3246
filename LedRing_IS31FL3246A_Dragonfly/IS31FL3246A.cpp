/* 07/02/2021 Copyright Tlera Corporation
 *  
 *  Created by Kris Winer
 *  
 This is a simple library for the IS31FL3246 36-led driver from Lumissil
 
 Library may be used freely and without limit with attribution.
 
*/

#include "IS31FL3246A.h"
#include "I2CDev.h"

IS31FL3246A::IS31FL3246A(I2Cdev* i2c_bus)
{
  _i2c_bus = i2c_bus;
}


  void IS31FL3246A::reset() // return to default registers/conditions
{
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_RESET, 0x00);
}


  void IS31FL3246A::disable()
  {
  uint8_t temp = _i2c_bus->readByte(IS31FL3246A_ADDRESS, IS31FL3246A_CONTROL);
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_CONTROL, temp & ~(0x01)); //clear bit 0 to shut down
  }


  void IS31FL3246A::enable()
  {
  uint8_t temp = _i2c_bus->readByte(IS31FL3246A_ADDRESS, IS31FL3246A_CONTROL);
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_CONTROL, temp | 0x01); //set bit 0 to enable
  }


  void IS31FL3246A::mode(uint8_t resolution, uint8_t frequency, uint8_t ledMode)
 {
  //set ledMode (bit 6), HF frequency (bits 4 and 5), and resolution (bit 1)
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_CONTROL, resolution << 1 | frequency << 4 | ledMode << 6); // bit 0 is set to disable here
 }


 void IS31FL3246A::setHFPWM(uint8_t channel, uint8_t FMS, uint8_t HFPWMLevel_H, uint8_t HFPWMLevel_L)
 {
     if(channel >= 1 && channel <= 36) {
     _i2c_bus->writeByte(IS31FL3246A_ADDRESS, (2*channel - 1), HFPWMLevel_L);  // set lower bytes of HF PWM level (0 - 255) for specified led channel
     _i2c_bus->writeByte(IS31FL3246A_ADDRESS, (2*channel), (FMS << 2) | HFPWMLevel_H);  // set freq control mode and upper 2 bits of HF PWM level for specified led channel
     }
 }


 void IS31FL3246A::setLFPWM(uint8_t channel, uint8_t PWMLevel)
 {
     if(channel >= 1 && channel <= 36) {
     _i2c_bus->writeByte(IS31FL3246A_ADDRESS, channel + 0x48, PWMLevel);  // set LF PWM level (0 - 255) for specified led channel
     }
 }


   void IS31FL3246A::setGlobalCurrent(uint8_t IR, uint8_t IG, uint8_t IB)
{
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_GCC_R, IR); // set red current maximum
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_GCC_G, IG); // set green current maximum
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_GCC_B, IB); // set blue current maximum
}


  void IS31FL3246A::PWMUpdate()
{
  _i2c_bus->writeByte(IS31FL3246A_ADDRESS, IS31FL3246A_UPDATE, 0x00); // update led array
}

/* 07/02/2021 Copyright Tlera Corporation
 *  
 *  Created by Kris Winer
 *  
 This is a simple library for the IS31FL3246 36-led driver from Lumissil
 
 Library may be used freely and without limit with attribution.
 
*/
  
#ifndef IS31FL3246A_h
#define IS31FL3246A_h

#include "Arduino.h"
#include "I2CDev.h"
#include <Wire.h>

/* IS31FL3246A registers
*  https://www.mouser.cn/pdfDocs/IS31FL3246_DS.pdf
*/
#define IS31FL3246A_CONTROL    0x00

#define IS31FL3246A_HFPWM01L   0x01
#define IS31FL3246A_HFPWM01H   0x02
#define IS31FL3246A_HFPWM02L   0x03
#define IS31FL3246A_HFPWM02H   0x04
#define IS31FL3246A_HFPWM03L   0x05
#define IS31FL3246A_HFPWM03H   0x06
#define IS31FL3246A_HFPWM04L   0x07
#define IS31FL3246A_HFPWM04H   0x08
#define IS31FL3246A_HFPWM05L   0x09
#define IS31FL3246A_HFPWM05H   0x0A
#define IS31FL3246A_HFPWM06L   0x0B
#define IS31FL3246A_HFPWM06H   0x0C
#define IS31FL3246A_HFPWM07L   0x0D
#define IS31FL3246A_HFPWM07H   0x0E
#define IS31FL3246A_HFPWM08L   0x0F
#define IS31FL3246A_HFPWM08H   0x10
#define IS31FL3246A_HFPWM09L   0x11
#define IS31FL3246A_HFPWM09H   0x12
#define IS31FL3246A_HFPWM10L   0x13
#define IS31FL3246A_HFPWM10H   0x14
#define IS31FL3246A_HFPWM11L   0x15
#define IS31FL3246A_HFPWM11H   0x16
#define IS31FL3246A_HFPWM12L   0x17
#define IS31FL3246A_HFPWM12H   0x18
#define IS31FL3246A_HFPWM13L   0x19
#define IS31FL3246A_HFPWM13H   0x1A
#define IS31FL3246A_HFPWM14L   0x1B
#define IS31FL3246A_HFPWM14H   0x1C
#define IS31FL3246A_HFPWM15L   0x1D
#define IS31FL3246A_HFPWM15H   0x1E
#define IS31FL3246A_HFPWM16L   0x1F
#define IS31FL3246A_HFPWM16H   0x20
#define IS31FL3246A_HFPWM17L   0x21
#define IS31FL3246A_HFPWM17H   0x22
#define IS31FL3246A_HFPWM18L   0x23
#define IS31FL3246A_HFPWM18H   0x24
#define IS31FL3246A_HFPWM19L   0x25
#define IS31FL3246A_HFPWM19H   0x26
#define IS31FL3246A_HFPWM20L   0x27
#define IS31FL3246A_HFPWM20H   0x28
#define IS31FL3246A_HFPWM21L   0x29
#define IS31FL3246A_HFPWM21H   0x2A
#define IS31FL3246A_HFPWM22L   0x2B
#define IS31FL3246A_HFPWM22H   0x2C
#define IS31FL3246A_HFPWM23L   0x2D
#define IS31FL3246A_HFPWM23H   0x2E
#define IS31FL3246A_HFPWM24L   0x2F
#define IS31FL3246A_HFPWM24H   0x30
#define IS31FL3246A_HFPWM25L   0x31
#define IS31FL3246A_HFPWM25H   0x32
#define IS31FL3246A_HFPWM26L   0x33
#define IS31FL3246A_HFPWM26H   0x34
#define IS31FL3246A_HFPWM27L   0x35
#define IS31FL3246A_HFPWM27H   0x36
#define IS31FL3246A_HFPWM28L   0x37
#define IS31FL3246A_HFPWM28H   0x38
#define IS31FL3246A_HFPWM29L   0x39
#define IS31FL3246A_HFPWM29H   0x3A
#define IS31FL3246A_HFPWM30L   0x3B
#define IS31FL3246A_HFPWM30H   0x3C
#define IS31FL3246A_HFPWM31L   0x3D
#define IS31FL3246A_HFPWM31H   0x3E
#define IS31FL3246A_HFPWM32L   0x3F
#define IS31FL3246A_HFPWM32H   0x40
#define IS31FL3246A_HFPWM33L   0x41
#define IS31FL3246A_HFPWM33H   0x42
#define IS31FL3246A_HFPWM34L   0x43
#define IS31FL3246A_HFPWM34H   0x44
#define IS31FL3246A_HFPWM35L   0x45
#define IS31FL3246A_HFPWM35H   0x46
#define IS31FL3246A_HFPWM36L   0x47
#define IS31FL3246A_HFPWM36H   0x48

#define IS31FL3246A_LFPWM01    0x49
#define IS31FL3246A_LFPWM02    0x4A
#define IS31FL3246A_LFPWM03    0x4B
#define IS31FL3246A_LFPWM04    0x4C
#define IS31FL3246A_LFPWM05    0x4D
#define IS31FL3246A_LFPWM06    0x4E
#define IS31FL3246A_LFPWM07    0x4F
#define IS31FL3246A_LFPWM08    0x50
#define IS31FL3246A_LFPWM09    0x51
#define IS31FL3246A_LFPWM10    0x52
#define IS31FL3246A_LFPWM11    0x53
#define IS31FL3246A_LFPWM12    0x54
#define IS31FL3246A_LFPWM13    0x55
#define IS31FL3246A_LFPWM14    0x56
#define IS31FL3246A_LFPWM15    0x57
#define IS31FL3246A_LFPWM16    0x58
#define IS31FL3246A_LFPWM17    0x59
#define IS31FL3246A_LFPWM18    0x5A
#define IS31FL3246A_LFPWM19    0x5B
#define IS31FL3246A_LFPWM20    0x5C
#define IS31FL3246A_LFPWM21    0x5D
#define IS31FL3246A_LFPWM22    0x5E
#define IS31FL3246A_LFPWM23    0x5F
#define IS31FL3246A_LFPWM24    0x60
#define IS31FL3246A_LFPWM25    0x61
#define IS31FL3246A_LFPWM26    0x62
#define IS31FL3246A_LFPWM27    0x63
#define IS31FL3246A_LFPWM28    0x64
#define IS31FL3246A_LFPWM29    0x65
#define IS31FL3246A_LFPWM30    0x66
#define IS31FL3246A_LFPWM31    0x67
#define IS31FL3246A_LFPWM32    0x68
#define IS31FL3246A_LFPWM33    0x69
#define IS31FL3246A_LFPWM34    0x6A
#define IS31FL3246A_LFPWM35    0x6B
#define IS31FL3246A_LFPWM36    0x6C

#define IS31FL3246A_UPDATE     0x6D

#define IS31FL3246A_GCC_G      0x6E
#define IS31FL3246A_GCC_R      0x6F
#define IS31FL3246A_GCC_B      0x70
#define IS31FL3246A_PH_DLY     0x71
#define IS31FL3246A_RESET      0x72

#define IS31FL3246A_ADDRESS    0x30 // when AD = LOW (default)

#define HF_32kHz  0x00 // HF frequency selections
#define HF_64kHz  0x01
#define HF_128kHz 0x10

#define HFRES_8bit  0x00 // HF resolution selections
#define HFRES_10bit 0x01

#define allChannels 0x00 // select led group control
#define RGBgroup    0x01

#define HFPandLFP 0x00 // select PWM frequency mode for each channel
#define HFPonly   0x01
#define DCmode    0x02
#define LEDoff    0x03

class IS31FL3246A
{
  public: 
  IS31FL3246A(I2Cdev* i2c_bus);
  void reset();
  void disable();
  void enable();
  void mode(uint8_t resolution, uint8_t frequency, uint8_t ledMode);
  void setHFPWM(uint8_t channel, uint8_t FMS, uint8_t HFPWMLevel_H, uint8_t HFPWMLevel_L);
  void setLFPWM(uint8_t channel, uint8_t PWMLevel);
  void setGlobalCurrent(uint8_t IR, uint8_t IG, uint8_t IB);
  void PWMUpdate();
  private:
  I2Cdev* _i2c_bus;
 };

#endif

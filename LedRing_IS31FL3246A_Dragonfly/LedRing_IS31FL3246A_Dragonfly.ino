/* 
 * 07/02/2021 Copyright Tlera Corporation
 *  
 *  Created by Kris Winer
 *  
 *  The IS31FL3246 is a 36-channel led controller ( https://www.mouser.cn/pdfDocs/IS31FL3246_DS.pdf).
 *  
 *  This sketch uses default SDA/SCL pins on the Dragonfly/Butterfly development boards, but
 *  should work on almost any dev board that supports I2C
 *
 *  Library may be used freely and without limit with attribution.
 */
#include "IS31FL3246A.h"

#define I2C_BUS    Wire               // Define the I2C bus (Wire instance) you wish to use

I2Cdev             i2c_0(&I2C_BUS);   // Instantiate the I2Cdev object and point to the desired I2C bus


const boolean invert = true;   // set true if common anode, false if common cathode
uint8_t color = 0;             // a value from 0 to 255 representing the hue
uint32_t R = 0, G = 0, B = 0;  // the Red Green and Blue color components
uint8_t brightness = 255;      // 255 is maximum brightness, but can be changed

byte PWM_Gamma64[64]=
{
  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
  0x08,0x09,0x0b,0x0d,0x0f,0x11,0x13,0x16,
  0x1a,0x1c,0x1d,0x1f,0x22,0x25,0x28,0x2e,
  0x34,0x38,0x3c,0x40,0x44,0x48,0x4b,0x4f,
  0x55,0x5a,0x5f,0x64,0x69,0x6d,0x72,0x77,
  0x7d,0x80,0x88,0x8d,0x94,0x9a,0xa0,0xa7,
  0xac,0xb0,0xb9,0xbf,0xc6,0xcb,0xcf,0xd6,
  0xe1,0xe9,0xed,0xf1,0xf6,0xfa,0xfe,0xff
};

// Configure led driver
uint8_t resolution = HFRES_8bit;  // set HF PWM resolution, either HFRES_8bit or HFRES_10bit
uint8_t frequency = HF_32kHz;     // set HF PWM frequency, either HF_32kHz, HF_64kHz, or HF_128kHz
uint8_t ledMode = allChannels;    // select individual control (allChannels) or RGB group control (RGBgroup)
//for white balance
uint8_t IR = 148;                 // set global red current maximum from 0 - 255, so IR/256 x Iset (16 mA)
uint8_t IG = 128;                 // set global green current maximum from 0 - 255
uint8_t IB = 128;                 // set global blue current maximum from 0 - 255

IS31FL3246A IS31FL3246A(&i2c_0); // instantiate IS31FL3246A class

void setup() {
  Serial.begin(115200);
  Serial. blockOnOverrun(false);
  delay(4000);
  
  I2C_BUS.begin();                // Set master mode, default on SDA/SCL for STM32L4
  I2C_BUS.setClock(400000);       // I2C frequency at 400 kHz
  delay(1000);
  
  Serial.println("Scan for I2C devices:");
  i2c_0.I2Cscan();                // should detect IS31FL3246 at 0x30
  delay(1000);

  IS31FL3246A.mode(resolution, frequency, ledMode); // set PWM frequency and output resolution and led mode
  IS31FL3246A.setGlobalCurrent(IR, IG, IB);
  Serial.println("End of Setup!");

} /* end of Setup */

void loop() {

  IS31FL3246A.enable();
  
  // Turn on leds slowly from 1 to 36 one at a time at 1/4 intensity
  for(uint8_t ii = 1; ii <= 36; ii++)
  {
    IS31FL3246A.setHFPWM(ii, HFPandLFP, 0, 0x64); //

    IS31FL3246A.setLFPWM(ii, 0x64); // on 1/4
    IS31FL3246A.PWMUpdate();
    delay(200);
    IS31FL3246A.setLFPWM(ii, 0x00); // off
    IS31FL3246A.PWMUpdate();
  }
  
  allLEDOff();
  IS31FL3246A.disable();
  delay(1000);
  Serial.println("End of individual led function test!");


  // led breathing mode test
  IS31FL3246A.enable();
 
  // Breath all leds slowly
  allLEDFadeOn(100);
  allLEDFadeOff(100);
  // then faster
  allLEDFadeOn(50);
  allLEDFadeOff(50);
  //then faster still
  allLEDFadeOn(10);
  allLEDFadeOff(10);
  //then smooth blink
  allLEDFadeOn(5);
  allLEDFadeOff(5);
  allLEDFadeOn(5);
  allLEDFadeOff(5);
  allLEDFadeOn(5);
  allLEDFadeOff(5);

  allLEDOff();
  IS31FL3246A.disable();
  delay(1000);
  Serial.println("End of breathing mode test!");


  IS31FL3246A.enable();
  
  // Fade on and fade off leds 1 to 36 one at a time
  for(uint8_t ii = 1; ii <=36; ii++)
  {
    for (int8_t j=0; j<=63; j++) {// one LED breath rising
      IS31FL3246A.setLFPWM(ii, PWM_Gamma64[j]);
      IS31FL3246A.PWMUpdate();
      delay(3);
    }
    for (int8_t j=63; j>=0; j--) {// all LED breath falling
      IS31FL3246A.setLFPWM(ii, PWM_Gamma64[j]);
      IS31FL3246A.PWMUpdate();
      delay(3);
    }
  }

  allLEDOff();
  IS31FL3246A.disable();
  delay(1000);
  Serial.println("End of individual fade test!");

  
  // ISSI demo test
  IS31FL3246A.enable();
  
  mode1(); // ISSI demo

  allLEDOff();
  IS31FL3246A.disable();
  delay(1000);
  Serial.println("End of ISSI Demo test!");


  // white comet growing in intensity test
  IS31FL3246A.enable();

  for(uint8_t j = 1; j < 8; j++)
  {
     for(uint8_t ii = 1; ii < 36; ii++)
     {
     IS31FL3246A.setHFPWM(ii, HFPandLFP, 0, j*32);  
     }

    for(uint8_t i = 4; i <= 12; i++)
    {
     IS31FL3246A.setLFPWM((i*3), 0xFF);     // full on
     IS31FL3246A.setLFPWM((i*3 - 1), 0xFF);  
     IS31FL3246A.setLFPWM((i*3 - 2), 0xFF);  
     
     IS31FL3246A.setLFPWM(((i - 1)*3), 128);     // half on
     IS31FL3246A.setLFPWM(((i - 1)*3 - 1), 128);  
     IS31FL3246A.setLFPWM(((i - 1)*3 - 2), 128);  
     
     IS31FL3246A.setLFPWM(((i - 2)*3), 64);     // quarter on
     IS31FL3246A.setLFPWM(((i - 2)*3 - 1), 64);  
     IS31FL3246A.setLFPWM(((i - 2)*3 - 2), 64);  
     
     IS31FL3246A.setLFPWM(((i - 3)*3), 32);     // eighth on
     IS31FL3246A.setLFPWM(((i - 3)*3 - 1), 32); 
     IS31FL3246A.setLFPWM(((i - 3)*3 - 2), 32);  
     IS31FL3246A.PWMUpdate();
     delay(100);
     
     allLEDOff();
    }
 }
 
  IS31FL3246A.disable();
  delay(1000);
  Serial.println("End of white comet test!");
  

  // Color wheel test
  IS31FL3246A.enable();

  for(uint8_t ii = 1; ii <= 36; ii++)
  {
  IS31FL3246A.setHFPWM(ii, HFPandLFP, 0, 128);  // half intensity
  }

  for(uint8_t j = 0; j < 3; j++) // cycle through the color wheel three times...
  {

  for (color = 0; color < 255; color++) { // Slew through the color spectrum

   hueToRGB(color, brightness);  // call function to convert hue to RGB

   // write the RGB values to the pins
   for(uint8_t i = 1; i <= 12; i++)
   {
    IS31FL3246A.setLFPWM((i*3) - 2, G); // green
    IS31FL3246A.setLFPWM((i*3) - 1, R); // red
    IS31FL3246A.setLFPWM((i*3) - 0, B); // blue
   }
    IS31FL3246A.PWMUpdate();
    delay(100);
   }
  }
  
  allLEDOff();
  IS31FL3246A.disable();
  delay(1000);
  Serial.println("End of color wheel test!");

} // end of main loop //



//Special customized functions *********************************************************

void mode1(void)  // Taken from ISSI Arduino code
{
  int8_t i = 0;
  int8_t j = 0;

  for(i=1; i<=12; i++) // First third
  {
    IS31FL3246A.setLFPWM((i*3),0xFF); //on
    IS31FL3246A.PWMUpdate();          //update PWM & control registers
    delay(80);//100ms
    IS31FL3246A.setLFPWM((i*3),0x00); //off  
    IS31FL3246A.PWMUpdate();          //update PWM & control registers
  }
  delay(500); //keep 0.5s
  
  for(i=1; i<=12; i++) // Second third
  {
    IS31FL3246A.setLFPWM((i*3 - 1),0xFF); //on 
    IS31FL3246A.PWMUpdate();              //update PWM & control registers
    delay(80);//100ms
    IS31FL3246A.setLFPWM((i*3 - 1),0x00); //off
    IS31FL3246A.PWMUpdate();              //update PWM & control registers
  }
  delay(500); //keep 0.5s
  
  for(i=1; i<=12; i++) // Last third
  {
    IS31FL3246A.setLFPWM((i*3 - 2),0xFF); //on
    IS31FL3246A.PWMUpdate();      //update PWM & control registers
    delay(80);//100ms
    IS31FL3246A.setLFPWM((i*3 - 2),0x00); //off
    IS31FL3246A.PWMUpdate();      //update PWM & control registers
  }
  delay(500); //keep 0.5s
}


void allLEDFadeOn(uint8_t speed) // fade speed in milliseconds
{
   for (int8_t j=0; j<=63; j++) // all LED breath rising
  {
     for(int8_t i=1; i<=0x24; i++)
    {
      IS31FL3246A.setLFPWM(i, PWM_Gamma64[j]); //set all PWM 
    }
    IS31FL3246A.PWMUpdate();      //update PWM & control registers
    delay(speed); 
  } 
    delay(500); //keep 0.5s
}


void allLEDFadeOff(uint8_t speed) // fade speed in milliseconds
{
   for (int8_t j=63; j>=0; j--) // all LED breath falling
  {
     for(int8_t i=1; i<=0x24; i++)
    {
      IS31FL3246A.setLFPWM(i, PWM_Gamma64[j]); //set all PWM 
    }
    IS31FL3246A.PWMUpdate();      //update PWM & control registers
    delay(speed); 
  } 
    delay(500); //keep 0.5s
}


void allLEDOff()
{
     for(int8_t i = 1; i <= 36; i++)
    {
      IS31FL3246A.setLFPWM(i, 0); //set all channels off
    }
    IS31FL3246A.PWMUpdate();      //update PWM & control registers
}


// Courtesy http://www.instructables.com/id/How-to-Use-an-RGB-LED/?ALLSTEPS
// function to convert a color to its Red, Green, and Blue components.

void hueToRGB(uint8_t hue, uint8_t brightness)
{
    uint16_t scaledHue = (hue * 6);
    uint8_t segment = scaledHue / 256; // segment 0 to 5 around the
                                            // color wheel
    uint16_t segmentOffset =
      scaledHue - (segment * 256); // position within the segment

    uint8_t complement = 0;
    uint16_t prev = (brightness * ( 255 -  segmentOffset)) / 256;
    uint16_t next = (brightness *  segmentOffset) / 256;

    if(invert)
    {
      brightness = 255 - brightness;
      complement = 255;
      prev = 255 - prev;
      next = 255 - next;
    }

    switch(segment ) {
    case 0:      // red
        R = brightness;
        G = next;
        B = complement;
    break;
    case 1:     // yellow
        R = prev;
        G = brightness;
        B = complement;
    break;
    case 2:     // green
        R = complement;
        G = brightness;
        B = next;
    break;
    case 3:    // cyan
        R = complement;
        G = prev;
        B = brightness;
    break;
    case 4:    // blue
        R = next;
        G = complement;
        B = brightness;
    break;
   case 5:      // magenta
    default:
        R = brightness;
        G = complement;
        B = prev;
    break;
    }
}

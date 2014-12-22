
/*
MTW RGB LED Strip Controller 

Included code here to show marlin table ouputs. NOT to be used in this code section.

#define mtwled_ready	        2       // Marlin Ready
#define mtwled_startup	        5	// Marlin startup
#define mtwled_temphit  	8	// Hotend is at target temp
#define mtwled_templow	        9	// Hotend heater is slightly lower than target temp
#define mtwled_temphigh  	10	// Hotend heater is slightly higher than target temp
#define mtwled_heateroff	13	// Hotend heater is off but still hot

*/


#include "MTWLPD8806.h"  // LED driver section
#include "SPI.h" // SPI Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
#include <avr/power.h>
#endif
#include <Wire.h>  // I2c Driver
// LPD8806-based RGB LED Modules in a strip

/*****************************************************************************/
int led = 13;

//------------
// Number of RGB LEDs in strand:
int nLEDs = 16;
// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 16;
int clockPin = 15;

// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs); // hardware spi
// LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
//LPD8806 strip = LPD8806(nLEDs);
//---------------

boolean newcode = false;	// flag for new i2c code available 
byte frame[]={0,0,0,0,0};
int framepos=0;
#define pattern  frame[1]
#define rled     frame[2]
#define gled     frame[3]
#define bled     frame[4]

boolean stripshow() {
  strip.show();
  return newcode;
}

void setup()
{
	pinMode(led, OUTPUT);			// setup on board led
	Wire.begin(21);				// join i2c bus with address #21
	Wire.onReceive(receiveEvent);	        // register event
	Serial.begin(115200);                   // start serial for output

  // Start up the LED strip
	strip.begin();					// Start Spi driven led strip

  // Update the strip, to start they are all 'off'
	strip.show();				
	delay(50);
//                        R     B     G   time
	colorWipe(strip.Color(100,   0,   0), 10);  // White slow on

//	while(newcode==false)  // hold here until communication with Marlin Established 
//          {
            // this isn't good. If we miss the initial marlin startup or get reset somehow we freeze here
            // I reccommend using the marlin startup and ready codes that already get handled and just
            // not do this in startup()
//          }

  		//colorWipe(strip.Color(0,   0,   0), 1);  // Flash Green for I2C Init
		delay(25);
		colorWipe(strip.Color(0,   80,   0), 1);  
                //delay(100);
}


void loop()
{
  switch(pattern) {
	  case 0:    // dont use this case
	  newcode=false;	
	  //lights0();
	  break;

	  case 1:    //  Special case.  all lights off
	  newcode=false;
	  //lights1();
	  break;

	  case 2:    // G28 color pattern call  2 leds per axis indication.  blue = not triggered, red = triggered
	  newcode=false;
	  endstops(rled, bled, gled); //  rled =X, bled =Y, gled=Z

	  break;

	  case 3:    // 
	  newcode=false;
	  //lights3();
	  break;

 	  case 4:    // 
	  newcode=false;
 	  //lights4();
 	  break;

 	  case 5:    // 
	  newcode=false;
 	  //lights5();
 	  break;

 	  case 6:    // 
	  newcode=false; 
 	  //lights6();
 	  break;

 	  case 7:    // 
	  newcode=false;
 	  //lights7();
 	  break; 
 
  	  case 8:    // 
	  newcode=false;
  	  //lights8();
  	  break;

	  case 9:    // 
	  newcode=false;
 	  //lights9();
 	  break; 
 
 
 	  case 10:    //  Solid Color
 	  newcode=false;
          colorWipe(strip.Color(rled,   bled,   gled), 1);  //  fast turn on of single color
          newcode=false;
  	  break;

 	  case 11:    //  Cylon
 	  newcode=false;
 	  scanner( rled,  bled,  gled, 75); // speed of the movement is hard coded.   
 	  break;

 	  case 12:    //  UFO Pulse
 	  newcode=false;
          ufopulse(rled,bled,gled);
 	  break;

 	  case 13:    //  Color Chase
 	  newcode=false;
          colorChase(strip.Color(rled, bled, gled), 50); // 
 	  break;

 	  case 14:    //  Color Cycle. color change all leds being the same color
 	  newcode=false;
          rainbowCycle(10);	//  
 	  break;

 	  case 15:    //  Color Chase Single Led
 	  newcode=false;
	  colorChase(strip.Color(rled, bled, gled), 50); // speed of the chase is hard coded not user selectable
 	  break;

 	  case 16:    //  Slow fill / Solid On
 	  newcode=false;
          colorWipe(strip.Color(rled,   bled,   gled), 100);  //   	  
  	  break;

 	  case 17:    //  Repeating Blink
 	  newcode=false;
          colorWipe(strip.Color(rled,   bled,   gled), 1);  //  fast turn on of single color	  //lights17();
          delay(200);
          colorWipe(strip.Color(0,  0,   0), 1);  //  Turn off lights
          delay(200);
 	  break;
 	  
 	  case 18:    // Rainbow
 	  newcode=false;
          rainbow(10);
 	  break;

 	  case 19:    // Theater chase rainbow 
 	  newcode=false;
          theaterChaseRainbow(70);
 	  break;




 	  case 90:    //
 	  newcode=false;
          colorWipe(strip.Color(0,   40,   0), 1); // RBG
 	  break;

 	  case 91:    //
 	  newcode=false;
          colorWipe(strip.Color(0,   60,   0), 1);  
 	  break;

 	  case 92:    //
 	  newcode=false;
          colorWipe(strip.Color(0,   80, 0), 1);  	  
 	  break;
 	  
 	  case 93:    //
 	  newcode=false;
          colorWipe(strip.Color(0,   100,  0), 1); 
 	  break;

 	  case 94:    //
 	  newcode=false;
          colorWipe(strip.Color(20,   40,   0), 1);  	
 	  break;

 	  case 95:    //
 	  newcode=false;
          colorWipe(strip.Color(40,   20,  0), 1);  	
 	  break;

 	  case 96:    //
 	  newcode=false;
          colorWipe(strip.Color(60,   0,   0), 1);  	
 	  break;

 	  case 97:    //
 	  newcode=false;
          colorWipe(strip.Color(80,  0,  0), 1);  	
 	  break;
 	  
 	  case 98:    //
 	  newcode=false;
          colorWipe(strip.Color(100,   0,  0), 1); 	
 	  break;

 	  case 99:    //
 	  newcode=false;
          colorWipe(strip.Color(120,   0,  0), 1);  	
 	  break;

    } //switch
  
}





// Color patterns to choose from based on i2c input to cpattern 

void sample() 
{

	// Send a simple pixel chase in...
	colorChase(strip.Color(127, 127, 127), 50); // White
	colorChase(strip.Color(127,   0,   0), 50); // Red
	colorChase(strip.Color(127, 127,   0), 50); // Yellow
	colorChase(strip.Color(  0, 127,   0), 50); // Green
	colorChase(strip.Color(  0, 127, 127), 50); // Cyan
	colorChase(strip.Color(  0,   0, 127), 50); // Blue
	colorChase(strip.Color(127,   0, 127), 50); // Violet

	// Send a theater pixel chase in...
	theaterChase(strip.Color(127, 127, 127), 50); // White
	theaterChase(strip.Color(127,   0,   0), 50); // Red
	theaterChase(strip.Color(127, 127,   0), 50); // Yellow
	theaterChase(strip.Color(  0, 127,   0), 50); // Green
	theaterChase(strip.Color(  0, 127, 127), 50); // Cyan
	theaterChase(strip.Color(  0,   0, 127), 50); // Blue
	theaterChase(strip.Color(127,   0, 127), 50); // Violet

	// Fill the entire strip with...
	colorWipe(strip.Color(127,   0,   0), 50);  // Red
	colorWipe(strip.Color(  0, 127,   0), 50);  // Green
	colorWipe(strip.Color(  0,   0, 127), 50);  // Blue

	rainbow(10);
	rainbowCycle(0);  // make it go through the cycle fairly fast
	theaterChaseRainbow(50);
}

void endstops(uint8_t x, uint8_t z, uint8_t y) { // pass endstop states instead of color values and display all 3 endstop states at once
  for(int i=0;i<16;i++) {
    switch(i) {
      case 0: case 1: case 13: case 14: // this is a X led
        x ? strip.setPixelColor(i,60,0,0) : strip.setPixelColor(i,0,60,0);
        break;
      case 2: case 3: case 10: case 11: // this is a Y led
        y ? strip.setPixelColor(i,60,0,0) : strip.setPixelColor(i,0,60,0);
        break;
      case 4: case 5: case 7: case 8: // this is a Z led
        z ? strip.setPixelColor(i,60,0,0) : strip.setPixelColor(i,0,60,0);
        break;
      default: // led is off
        strip.setPixelColor(i,0,0,0);
        break;
    }
  }
  stripshow();   // write all the pixels out
  sdelay(1);
}

void rainbow(uint8_t wait) {
	int i, j;
	
	for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
		for (i=0; i < strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel( (i + j) % 384));
                  if(newcode==true)
                  {
                   break; 
                  }
		}

		stripshow();   // write all the pixels out
		sdelay(wait);
	}
}

// Slightly different, this one makes the rainbow wheel equally distributed
// along the chain
void rainbowCycle(uint8_t wait) {
	uint16_t i, j;
	
	for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
		for (i=0; i < strip.numPixels(); i++) {
			// tricky math! we use each pixel as a fraction of the full 384-color wheel
			// (thats the i / strip.numPixels() part)
			// Then add in j which makes the colors go around per pixel
			// the % 384 is to make the wheel cycle around
			strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
                  if(newcode==true)
                  {
                   break; 
                  }

		}
		stripshow();   // write all the pixels out
		sdelay(wait);
	}
}


void ufopulse(uint8_t r, uint8_t g, uint8_t b) {  
  
  
}


// Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait) {
	int i;

	for (i=0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, c);
		stripshow();
		sdelay(wait);
	}
}

// Chase one dot down the full strip.
void colorChase(uint32_t c, uint8_t wait) {
	int i;

	// Start by turning all pixels off:
	for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

	// Then display one pixel at a time:
	for(i=0; i<strip.numPixels(); i++) {
		strip.setPixelColor(i, c); // Set new pixel 'on'
		stripshow();              // Refresh LED states
		strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
		sdelay(wait);
	}

	stripshow(); // Refresh to turn off last pixel
}


//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
	for (int j=0; j<10; j++) {  //do 10 cycles of chasing
		for (int q=0; q < 3; q++) {
			for (int i=0; i < strip.numPixels(); i=i+3) {
				strip.setPixelColor(i+q, c);    //turn every third pixel on
                  if(newcode==true)
                  {
                   break; 
                  }

			}
			stripshow();
			
			sdelay(wait);
			
			for (int i=0; i < strip.numPixels(); i=i+3) {
				strip.setPixelColor(i+q, 0);        //turn every third pixel off
                  if(newcode==true)
                  {
                   break; 
                  }

			}
		}
	}
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
	for (int j=0; j < 384; j++) {     // cycle all 384 colors in the wheel
		for (int q=0; q < 3; q++) {
			for (int i=0; i < strip.numPixels(); i=i+3) {
				strip.setPixelColor(i+q, Wheel( (i+j) % 384));    //turn every third pixel on
                  if(newcode==true)
                  {
                   break; 
                  }

			}
			stripshow();
			
			sdelay(wait);
			
			for (int i=0; i < strip.numPixels(); i=i+3) {
				strip.setPixelColor(i+q, 0);        //turn every third pixel off
                  if(newcode==true)
                  {
                   break; 
                  }

			}
		}
	}
}
/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r



void sdelay(uint8_t wait) // special delay to break out of functions on new I2C data
{
	for (int z=0; z < wait; z++)
	{
		
	delay(1);
		if (newcode==true)	// test if we have new I2C input
		{
			break;
		}
	}
}



uint32_t Wheel(uint16_t WheelPos)
{
	byte r, g, b;
	switch(WheelPos / 128)
	{
		case 0:
		r = 127 - WheelPos % 128;   //Red down
		g = WheelPos % 128;      // Green up
		b = 0;                  //blue off
		break;
		case 1:
		g = 127 - WheelPos % 128;  //green down
		b = WheelPos % 128;      //blue up
		r = 0;                  //red off
		break;
		case 2:
		b = 127 - WheelPos % 128;  //blue down
		r = WheelPos % 128;      //red up
		g = 0;                  //green off
		break;
	}
	return(strip.Color(r,g,b));
}

// "Larson scanner" = Cylon/KITT bouncing light effect
void scanner(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
	int i, j, pos, dir;

	pos = 0;
	dir = 1;

	for(i=0; i<((strip.numPixels()-1) * 8); i++) {
		// Draw 5 pixels centered on pos.  setPixelColor() will clip
		// any pixels off the ends of the strip, no worries there.
		// we'll make the colors dimmer at the edges for a nice pulse
		// look
		strip.setPixelColor(pos - 2, strip.Color(r/4, g/4, b/4));
		strip.setPixelColor(pos - 1, strip.Color(r/2, g/2, b/2));
		strip.setPixelColor(pos, strip.Color(r, g, b));
		strip.setPixelColor(pos + 1, strip.Color(r/2, g/2, b/2));
		strip.setPixelColor(pos + 2, strip.Color(r/4, g/4, b/4));

		if(stripshow()) break;
		delay(wait);
		// If we wanted to be sneaky we could erase just the tail end
		// pixel, but it's much easier just to erase the whole thing
		// and draw a new one next time.
		for(j=-2; j<= 2; j++)
		strip.setPixelColor(pos+j, strip.Color(0,0,0));
		// Bounce off ends of strip
		pos += dir;
		if(pos < 0) {
			pos = 1;
			dir = -dir;
			} else if(pos >= strip.numPixels()) {
			pos = strip.numPixels() - 2;
			dir = -dir;
		}
	}
}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
// portions contributed by OhmEye for the

void receiveEvent(int bytesRemaining)  // section may have to switch to slow i2c input if toasts spi led output.
{
  byte sbyte;
  
  while(bytesRemaining) {
    sbyte=Wire.read();        // read next byte
    bytesRemaining--;
    if(framepos>0) {          // we're within a frame
      frame[framepos]=sbyte;
      if(framepos>3) {        // this is last byte of frame, reset frame, flag newcode and exit
        framepos=0;
        newcode=true;
        return;
      } else {                // this is not the last byte in frame, advance position
        framepos++;
      }
    } else {                  // we're not within a frame yet, wait for start marker
      if(sbyte==250) {        // we got a start marker
        framepos=1;          // advance position
      }
    }
  } //while
}



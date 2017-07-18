/*
 * Nathan Steward
 * 7/17/2017
 * Published under MIT liscense
 * 
 * TODO:
 * add in DS3231 RTC compatibility
 * add menu
 * add battery monitor code
 */

#include <Time.h>
#include <TimeLib.h> //temp libs for time

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


void setup()   {                
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display(); //redo loaded art so it's like "something clock"
  delay(200); //stay for a little
  display.clearDisplay(); //clear and end setup
} //end setup


void loop() {
  byte XCENTER = 128/4 - 1;
  byte YCENTER = 64/2 - 1;
  byte hr = 7;
  byte mn = 54;
  byte sc = 0;
  byte dy = 0;
  byte mnth = 0;
  uint8_t yr = 0;
  char ap;
  setTime(hr,mn,sc,dy,mnth,yr);
  
  while(1){ 
    timeGet(hr,mn,sc,ap);
    drawFace(XCENTER,YCENTER,ap);
    setHands(XCENTER,YCENTER,hr,mn,sc);
    setBattery();
    display.display();
    delay(50);
    display.clearDisplay();
    
  }
} //end loop


void timeGet(byte& getHour, byte& getMinute, byte& getSecond, char& amPm){ 
  //pings for time, whatever method that may be
  getHour = hourFormat12();
  getMinute = minute();
  getSecond = second();
} //end timeGet


void drawFace(byte xcenter,byte ycenter,char amPm){ 
  //draws circle or whatever will be backdrop
  display.drawCircle(xcenter, ycenter, 31, WHITE);
  display.fillCircle(xcenter, ycenter, 3, WHITE); //just for aesthetic
  //write rest of face  
  
} //end drawFace


void setHands(byte xcenter, byte ycenter, byte drawHour, byte drawMinute, byte drawSecond){ 
  //does calculations for hand lines while setting
  double pi = 3.14159; //needed local constant
  byte rhr = 14; //radius of hour hand
  byte rmn = 22; //radius of minute hand
  byte rsc = 31; //radius of second hand
  
  /*
   * maths
   * ~>take sin or cos of the percentage of the circle w/r/t
   * the percentage of seconds/minutes/hours passed out of 60/60/12 and multiply by
   * radius to find value, given circle is centered at (0,0). add offsets xcenter/ycenter 
   * to adjust position
   * 
   * find y coordinate
   * cos(2pi * minutes or seconds /60)* radius
   * cos(2pi * hours/12)* radius
   * 
   * find x coordinate
   * sin(2pi* minutes or seconds /60)* radius
   * sin(2pi* hours/12)* radius
   */
  
  display.drawLine(xcenter, ycenter, int (sin(2*pi*drawHour/12)*rhr)+xcenter, int (-cos(2*pi*drawHour/12)*rhr)+ycenter, WHITE);//hour
  display.drawLine(xcenter, ycenter, int (sin(2*pi*drawMinute/60)*rmn)+xcenter, int (-cos(2*pi*drawMinute/60)*rmn)+ycenter, WHITE);//minute
  display.drawLine(xcenter, ycenter, int (sin(2*pi*drawSecond/60)*rsc)+xcenter, int (-cos(2*pi*drawSecond/60)*rsc)+ycenter, WHITE);//second
} //end setTime

void setBattery(){ //seperate funtion to call battery levels
  display.fillRect(82,40,3,14,WHITE);
  display.drawRoundRect(86,37,39,20,3,WHITE);
  display.drawRoundRect(85,36,41,22,4,WHITE);
  //sequentially shows percentage in 20 percent portions
  display.fillRoundRect(89,40,5,14,2,WHITE);
  display.fillRoundRect(96,40,5,14,2,WHITE);
  display.fillRoundRect(103,40,5,14,2,WHITE);
  display.fillRoundRect(110,40,5,14,2,WHITE);
  display.fillRoundRect(117,40,5,14,2,WHITE);
} //end setBattery


#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <SPI.h>


#define TFT_CS    10
#define TFT_RST   8  
#define TFT_DC    9 

#define TFT_SCLK 13   
#define TFT_MOSI 11   

//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup(void) {
  tft.initR(INITR_BLACKTAB);  
  tft.fillScreen(ST7735_BLACK); 
 
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(0);
  tft.setCursor(30,80);
  tft.println("Hello World!");  
  delay(1000);
  
}

void loop() {

  tft.fillScreen(ST7735_WHITE); 
  delay(1000);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(0);
  tft.setCursor(30,80);
  tft.println("Hey you! You got it!");  

  delay(500);
}

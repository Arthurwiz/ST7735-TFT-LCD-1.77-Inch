#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8
#define TFT_SCLK 13   
#define TFT_MOSI 11   

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#include <stdio.h>

#include <DHT.h>

#define DHTPIN 7 // DHT11 data pin is connected to Arduino 7 pin.
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#if defined(__SAM3X8E__)

#undef __FlashStringHelper::F(string_literal)

#define F(string_literal) string_literal

#endif

//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//Black theme

#define COLOR1 ST7735_WHITE
#define COLOR2 ST7735_BLACK

//White theme

//#define COLOR1 ST7735_BLACK
//#define COLOR2 ST7735_WHITE

int text_color_humidex;

float humidity, temperature, humidex;

String message;

void setup(void)
{
    Serial.begin(9600);
    
    // Initialize device.
    dht.begin();
    Serial.println("DHT Sensor Initalized");

    tft.initR(INITR_BLACKTAB); // initialize a ST7735S chip, black tab
    tft.fillScreen(COLOR2);
}

void testdrawtext(char* text, uint16_t color)
{
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.setTextWrap(true);
    tft.print(text);
}

void loop()
{
    // get data from DHT-11
    humidity = dht.readHumidity();

    temperature = dht.readTemperature();
    Serial.print("humidity:   "); Serial.println(humidity);
    Serial.print("temperature:   "); Serial.println(temperature);

    //humidex is calculated
    humidex = calculate_humidex(temperature, humidity);

    // Table
    tft.drawRect(0, 0, 128, 160, COLOR1);
    tft.drawLine(0, 50, 128, 50, COLOR1);
    tft.drawLine(0, 100, 128, 100, COLOR1);

    // data is outputed
    temperature_to_lcd(temperature, 4);
    humidity_to_lcd(humidity, 55);
    humidex_to_lcd(humidex, 105);
}


// outputs temperature to LCD

void temperature_to_lcd(float temperature, unsigned char text_position)
{
    int text_color;
    tft.setCursor(4, text_position);
    tft.setTextColor(COLOR1, COLOR2);
    tft.setTextSize(1);

    tft.print("Temperature:");
    tft.setTextSize(3);
    if (temperature > 0)
    {
        text_color = ST7735_BLUE;
    }
    else
    {
        text_color = ST7735_BLUE;
    }

    tft.setCursor(1, text_position + 20);
    fix_number_position(temperature);
    tft.setTextColor(text_color, COLOR2);
    tft.print(temperature, 1);
    tft.setCursor(108, text_position + 20);
    tft.print("C");
    tft.drawChar(90, text_position + 20, 247, text_color, COLOR2, 2); //degree symbol

}

//outputs humidity to LCD

void humidity_to_lcd(float humidity, unsigned char text_position)
{
    tft.setTextColor(COLOR1, COLOR2);
    tft.setCursor(4, text_position);
    tft.setTextSize(1);
    tft.println("Humidity:");
    tft.setTextSize(3);
    tft.setCursor(1, text_position + 20);
    fix_number_position(humidity);
    tft.print(humidity, 1);
    tft.print(" %");
}

//outputs Humidex to LCD

void humidex_to_lcd(float humidex, unsigned char text_position)
{
    tft.setCursor(4, text_position);
    tft.setTextSize(1);
    tft.println("Humidex [Feel Like]:");
    tft.setTextSize(3);

    tft.setCursor(1, text_position + 17);

    if ((humidex >= 21) && (temperature < 44))
    {
        fix_number_position(humidex);

        get_humidex_color_warning_message(humidex);

        tft.setTextColor(text_color_humidex, COLOR2);

        tft.print(humidex, 1);

        tft.setCursor(108, text_position + 17);

        tft.print("C");

        tft.drawChar(90, text_position + 17, 247, text_color_humidex, COLOR2, 2); //degree symbol

        tft.setCursor(3, text_position + 43);

        tft.setTextSize(1);

        tft.print(message);

    }
    else
    {
        tft.print(" --.-");

        tft.setCursor(108, text_position + 17);

        tft.print("C");

        tft.drawChar(90, text_position + 17, 247, COLOR1, COLOR2, 2); //degree symbol

        tft.setCursor(1, text_position + 43);

        tft.setTextSize(1);

        tft.println(" ");

    };

}

// aligs number to constant position

void fix_number_position(float number)
{
    if ((number >= -40) && (number < -9.9))
    {
        ;
    }

    if ((number >= -9.9) && (number < 0.0))
    {
        tft.print(" ");
    }

    if ((number >= 0.0) && (number < 9.9))
    {
        tft.print(" ");
    }

    if ((number >= 9.9) && (number < 99.9))
    {
        tft.print(" ");
    }

    if ((number >= 99.9) && (number < 151))
    {
        tft.print("");
    }
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

//function to calculete Humidex

float calculate_humidex(float temperature, float humidity)
{

    float e;

    e = (6.112 * pow(10, (7.5 * temperature / (237.7 + temperature))) * humidity / 100); //vapor pressure

    float humidex = temperature + 0.55555555 * (e - 10.0); //humidex

    return humidex;

}

// Setting text color and message based on Humidex value

void get_humidex_color_warning_message(float humidex)
{

    if ((humidex >= 21) && (humidex < 27))
    {

        text_color_humidex = Color565(0, 137, 0);

        message = "No discomfort ";

    } // dark green

    if ((humidex >= 27) && (humidex < 35))
    {

        text_color_humidex = Color565(76, 255, 0); // light green

        message = "Some discomfort ";

    }

    if ((humidex >= 35) && (humidex < 40))
    {

        text_color_humidex = Color565(255, 255, 0);

        message = "Great discomfort ";

    } // yellow

    if ((humidex >= 40) && (humidex < 46))
    {

        text_color_humidex = Color565(255, 140, 0);

        message = "Health risk ";

    } //light orange

    if ((humidex >= 46) && (humidex < 54))
    {

        text_color_humidex = Color565(221, 128, 0);

        message = "Great health risk ";

    } //dark orange

    if ((humidex >= 54))
    {

        text_color_humidex = Color565(255, 0, 0);

        message = "Heat stroke danger ";

    } // red

}

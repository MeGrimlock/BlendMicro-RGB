#include "Adafruit_WS2801.h"
#include <SPI.h>
#include <EEPROM.h>
#include <boards.h>
#include <RBL_nRF8001.h>

uint8_t dataPin  = 10;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 9;    // Green wire on Adafruit Pixels
uint8_t leds = 1;
Adafruit_WS2801 strip = Adafruit_WS2801(leds, dataPin, clockPin);

int R = 0;
int G = 0;
int B = 0;
String buff = "";

char rgb[12];




void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  ble_set_name("SinergiaTech");
  ble_begin();
  colorWipe(Color(255, 0, 0) );
  strip.show();
  delay(2000);
  colorWipe(Color(0, 0, 0) );
  strip.show();
  Serial.begin(57600);

}

void loop() {

  buff = "";
  while (ble_available() > 0) {
    buff += (char)ble_read();
  }
  if (buff != "") {
    //Serial.println(buff);
    char tmp[12];
    int i = 0;
    buff.toCharArray(tmp, buff.length() + 1);
    //Serial.println(tmp);
    char * data = strtok(tmp, ",");
    while (data) {
      rgb[i++] = atoi(data);
      data = strtok(NULL, ",");
    }
    R = (int)rgb[0];
    G = (int)rgb[1];
    B = (int)rgb[2];
  }

  //Serial.print("Rojo: ");
  //Serial.print(R, DEC);
  //Serial.print(" - Verde: ");
  //Serial.print(G, DEC);
  //Serial.print(" - Azul: ");
  //Serial.println(B, DEC);
  colorWipe(Color(R, G, B) );
  ble_do_events();
  delay(1);

}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

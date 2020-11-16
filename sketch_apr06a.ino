#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN 6
#define LED_COUNT 144
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int Timer;
int Blinktime;
int Star;

void setup() {

  Serial.begin(9600);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  #endif
  strip.begin();
  strip.setBrightness(15);

}

void loop() {
  if(millis() - Timer >= Blinktime){
    strip.setPixelColor(Star,0 ,0 ,0); // Turns the previous Star to off
    Star = random(LED_COUNT);
    strip.setPixelColor(Star, 255, 255, 0); //anables the next Star on a random position
    Timer= millis();
    Blinktime=  random(200, 2000);
    Serial.println(Blinktime); //To check if the random function has a problem
    strip.show();
    123
  }

}

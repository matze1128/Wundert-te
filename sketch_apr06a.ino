#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN 6
#define LED_COUNT 144
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int Star;
int Schnuppenpos;
unsigned long Schnuppentimer;
int Starnumber = 50;
int StarPositions[50];
unsigned long StarDuration[50];
unsigned long StarTimeSet[50];
int CurrentBrightness[50];

void setup() {

  Serial.begin(9600);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  #endif
  strip.begin();
  strip.setBrightness(15);

}

void loop() {
  Sternhimmel();
  StarBrightness();
  speedcheck();
  strip.show();
  }



void Sternhimmel(){
  
    for(int i= 0; i<Starnumber; i++){
      //delay(10);
      if(millis() - StarTimeSet[i] >= StarDuration[i]){
        strip.setPixelColor(StarPositions[i],0 ,0 ,0); // Turns the previous Star to off
        StarPositions[i]= random(LED_COUNT);
        //strip.setPixelColor(StarPositions[i], 255, 255, 0); //anables the next Star on a random position
        StarDuration[i]= random(2000, 10000);
        StarTimeSet[i]=millis();
      }
    }
}


void StarBrightness(){
  for(int i= 0; i<Starnumber; i++){
    CurrentBrightness[i]= ((255000/StarDuration[i])*(millis()-StarTimeSet[i]))/1000;
    strip.setPixelColor(StarPositions[i], CurrentBrightness[i], CurrentBrightness[i], 0); 
  }
}

void speedcheck(){
  if(millis()- Schnuppentimer >= 1){
    Schnuppenpos++;
    strip.setPixelColor(Schnuppenpos, 0, 0, 255);
    strip.setPixelColor(Schnuppenpos-1,0,0,0);
    Schnuppentimer= millis();
    if(Schnuppenpos == 144){
      Schnuppenpos= 0;
    }
  }
}
    
   

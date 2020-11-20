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
int Starnumber = 10;
int StarPositions[50];
unsigned long StarDuration[50];
unsigned long StarTimeSet[50];
int CurrentBrightness[50];
int MaxStarTime = 10000;
int MinStarTime = 400;

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
      if(millis() - StarTimeSet[i] >= StarDuration[i]){
        strip.setPixelColor(StarPositions[i],0 ,0 ,0); // Turns the previous Star to off
        StarPositions[i]= random(LED_COUNT);
        StarDuration[i]= random(MinStarTime, MaxStarTime);
        StarTimeSet[i]=millis();
      }
    }
}


void StarBrightness(){
  for(int i= 0; i<Starnumber; i++){
    if(millis()-StarTimeSet[i] <= (StarDuration[i] / 2)){
      CurrentBrightness[i]= ((255000/StarDuration[i])*(millis()-StarTimeSet[i]))/500;              // rise Brightness
      strip.setPixelColor(StarPositions[i], CurrentBrightness[i], CurrentBrightness[i], 0);
    }
    else{
       CurrentBrightness[i]= 510 - ((255000/StarDuration[i])*(millis()-StarTimeSet[i]))/500;         // lower Brightness
       strip.setPixelColor(StarPositions[i], CurrentBrightness[i], CurrentBrightness[i], 0);
    }
  }
}

void ShootingStar(){
  if(millis()- Schnuppentimer >= 10){
    Schnuppenpos++;
    strip.setPixelColor(Schnuppenpos, 0, 0, 255);
    strip.setPixelColor(Schnuppenpos-1,0,0,0);
    Schnuppentimer= millis();
    if(Schnuppenpos == LED_COUNT){
      Schnuppenpos= 0;
    }
  }
}
    
   

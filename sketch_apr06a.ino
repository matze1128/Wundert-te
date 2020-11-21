#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN 6
#define LED_COUNT 144
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


int Starnumber = 4;
int StarPositions[50];
unsigned long StarDuration[50];
unsigned long StarTimeSet[50];
int CurrentBrightness[50];
int MaxStarTime = 10000;
int MinStarTime = 400;

#define RA 255000 //*1000 (0-255)
#define GA 255000 //*1000
#define BA 10000  //*1000

int FRA;
int FGA;
int FBA;
int Length= 50;
int Schnuppenpos;
unsigned long Schnuppentimer;
int maxVerweildauer= 70;
int Vorgluehen= 2;
int Schnuppenstaerke= 700; //zwischen 0 und 1000

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
  ShootingStar();
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
    for(int i= Schnuppenpos; i> Schnuppenpos- Vorgluehen; i--){
      if(i>0){
        strip.setPixelColor(i, GA/1000, RA/1000, BA/1000);   //set first Pixels on full brightness
      }
    }
    for(int i= Schnuppenpos- Vorgluehen; i> Schnuppenpos- Length; i--){
      FRA = (RA - (RA/maxVerweildauer) * i) /1000;
      FGA = (GA - (GA/maxVerweildauer) * i) /1000;
      FBA = (BA - (BA/maxVerweildauer) * i) /1000;
      strip.setPixelColor(i, strip.Color(map(Schnuppenstaerke, 0, 1000, 0 ,FGA), map(Schnuppenstaerke, 0, 1000, 0, FRA), FBA * 0.8));
    }
    strip.setPixelColor(Schnuppenpos- Length, 0, 0, 0);

    
    Schnuppentimer= millis();
    if(Schnuppenpos == maxVerweildauer + Length){
      Schnuppenpos= 0;
    }
  }
}
    
   

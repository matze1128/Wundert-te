#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define BUTTON_PIN A1
#define LED_PIN 6
#define LED_COUNT 144
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
String valueNames[] =               {"Sternanzahl", "Max. Sterndauer", "Min. Sterndauer", "Max. Cometlength", "Max. Cometbreak", "Min. cometbreak", "Max. comettime", "cometluminosuity", "Main comet size", "Min.cometspeed"};
unsigned long values[] =            {5            , 20               , 6                , 25                , 25               , 5                , 144             , 10                , 2                , 4                };
unsigned long maxvalues[] =         {50           , 200              , values[1]        , 40                , 200              , values[4]        , LED_COUNT       , 100               , 5                , 15               };
unsigned long minvalues[] =         {0            , 1                , 1                , 8                 , 1                , 1                , 40              , 10                , 1                , 1                };
unsigned long fertigevalues[] =     {0            , 20000            , 6000             , 0                 , 25000            , 5000             , 0               , 0                 , 0                , 0                };

                                   //0              1                   2                3                   4                   5                  6                  7                   8                 9
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int StarPositions[50];
int CurrentBrightness[50];
int maxVerweildauer;
int Schnuppenstaerke= 700;//zwischen 0 und 1000
int Length= 15; 
#define RA 255000 //*1000 (0-255)
#define GA 255000 //*1000
#define BA 10000  //*1000
int FRA;
int FGA;
int FBA;
unsigned long StarDuration[50];
unsigned long StarTimeSet[50];
int Schnuppenpos;
unsigned long Schnuppentimer;
unsigned long Schnuppenpausentimer;
int Schnuppenpause;
int cometspeed;
unsigned long tasterTimer;
int Menupunkt = 0;
unsigned long refreshTimer;
int letzterstatus; // Um beim erhöhen von Menupunkt nicht direkt den neuen Menupunkt zu verändern
int letzterstatusvergleich;

int on = true;
int mode = 2;
int potiPin = A0;
int rotVar = 0 ;            // Variable zum speichern der Werte für die Ausgabe
int gruenVar = 0 ;          // Variable zum speichern der Werte für die Ausgabe
int blauVar = 0 ;           // Variable zum speichern der Werte für die Ausgabe
int potiVar ;           // Variable zum speichern des Potentiometereingangs  
int oldPotiVar = 0;
int oldBrightness = 0;

void setup() {

  Serial.begin(9600);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  #endif
  pinMode(BUTTON_PIN, INPUT);
  pinMode(A2, INPUT_PULLUP);
  strip.begin();
  lcd.begin(16, 2);
}




void loop() 
{
  Serial.println(analogRead(A2));
  setMode();
  if(mode == 1)
  {
    Sternhimmel();
  }
  if(mode == 2)
  {
    setColorBrightness();
    oneColor();
  }
  strip.show();
}

void setColorBrightness()
{
  if(oldBrightness - analogRead(A3) >= 10 || oldBrightness - analogRead(A3) <= -10)
  {
  strip.setBrightness(map(analogRead(A3), 0, 1024, 0, 101));
  oldBrightness = analogRead(A3);
  }
}


void setMode()    // zwischen modes wechseln
{
  if(analogRead(A2) < 500)
  {
    mode = 2;
  }
  else
  {
    mode = 1;
  }
}

void oneColor()
{
  if(potiVar - analogRead(A0) >= 10 || potiVar - analogRead(A0) <= -10)
  {
  potiVar = analogRead(A0);
  }
  
  if(potiVar < 341)      // Unteres Drittel des Potenziometerbereichs (0-340)
  {                  
    potiVar = (potiVar * 3) / 4;   // Den Wert auf 0-255 umrechnen
 
    rotVar = 256 - potiVar;        // Rot von an nach aus
    gruenVar = potiVar;            // Grün von aus nach an
    blauVar = 0;                   // Blau aus
  }
  else if(potiVar < 682)     // Mittleres Drittel des Potenziometerbereichs (341-681)
  {
    potiVar = ( (potiVar-341) * 3) / 4;     // Den Wert auf 0-255 umrechnen
 
    rotVar = 0;                    // Rot aus
    gruenVar = 256 - potiVar;      // Grün von an nach aus
    blauVar = potiVar;             // Blau von aus nach an
  }
  else      // Oberes Drittel des Potenziometerbereichs (682-1023)
  {
    potiVar = ( (potiVar-683) * 3) / 4;     // Den Wert auf 0-255 umrechnen
 
    rotVar = potiVar;              // Rot von aus nach an
    gruenVar = 0;                  // Grün aus
    blauVar = 256 - potiVar;       // Blau von an nach aus
  }
  
  for(int i = 0;i <= LED_COUNT;i++)
  {
    strip.setPixelColor(i, gruenVar, rotVar, blauVar);
  }
}



void Sternhimmel()
{
  strip.setBrightness(values[7]);
  Stars();
  StarBrightness();
  ShootingStar();
  Taster();
  valuesinmillisek();
  LCDpanel();
}

void LCDpanel(){
  lcd.setCursor(0,0);
  lcd.print(valueNames[Menupunkt]);
  lcd.setCursor(valueNames[Menupunkt].length(),0);
  lcd.print("     ");
  lcd.setCursor(0,1);
  lcd.print(values[Menupunkt]);                                   
  if(values[Menupunkt] < 10)         // überschüssige alte zahlen entfernen
  {
    lcd.setCursor(1, 1);
    lcd.print("     ");
  }
  if(values[Menupunkt] < 100)
  {
    lcd.setCursor(2, 1);
    lcd.print("     ");
  }
}




void valuesinmillisek()
{
  if(Menupunkt == 1 || Menupunkt == 2 || Menupunkt == 4 || Menupunkt == 5)
  {
     fertigevalues[Menupunkt] = values[Menupunkt] * 1000;
  }
}

void Stars(){
  
    for(int i= 0; i<values[0]; i++){
      if(millis() - StarTimeSet[i] >= StarDuration[i]){
        strip.setPixelColor(StarPositions[i],0 ,0 ,0); // Turns the previous Star to off
        StarPositions[i]= random(LED_COUNT);
        StarDuration[i]= random(fertigevalues[2], fertigevalues[1]);
        StarTimeSet[i]=millis();
      }
    }
}


void StarBrightness(){
  for(int i= 0; i<values[0]; i++){
    if(millis()-StarTimeSet[i] <= (StarDuration[i] / 2)){
      CurrentBrightness[i]= ((255000/StarDuration[i])*(millis()-StarTimeSet[i]))/500;              // rise Brightness
      strip.setPixelColor(StarPositions[i], CurrentBrightness[i], CurrentBrightness[i], 0);
    }
    else{
       CurrentBrightness[i]= 510 - ((255000/StarDuration[i])*(millis()-StarTimeSet[i]))/500;         // lower Brightness
       strip.setPixelColor(StarPositions[i], CurrentBrightness[i], CurrentBrightness[i], 0);
    }
  }
  for(int i = values[0];i <= 50; i++)
  {
    if(CurrentBrightness[i] > 0)
    {
       CurrentBrightness[i]= 0;         // lower Brightness
       strip.setPixelColor(StarPositions[i], CurrentBrightness[i], CurrentBrightness[i], 0);
    }
  }
  strip.show();
}

void ShootingStar(){
  if(millis()- Schnuppentimer >= cometspeed && millis() - Schnuppenpausentimer >= Schnuppenpause){
    Schnuppenpos++;
    for(int i= Schnuppenpos; i> Schnuppenpos- values[8]; i--){
      if(i>0 && i<maxVerweildauer - 6){
        strip.setPixelColor(i, GA/1000, RA/1000, BA/1000);   //set first Pixels on full brightness
      }
    }
    for(int i= Schnuppenpos- values[8]; i> Schnuppenpos- Length; i--){
      if(i<maxVerweildauer){
        FRA = (RA - ((RA / maxVerweildauer) * i)) /1000;
        FGA = (GA - ((GA / maxVerweildauer) * i)) /1000;
        FBA = (BA - ((BA / maxVerweildauer) * i)) /1000;
        strip.setPixelColor(i, FGA, FRA, FBA);
      }  
    }
    strip.setPixelColor(Schnuppenpos- Length, 0, 0, 0);
    Schnuppentimer= millis();
    if(Schnuppenpos == maxVerweildauer + Length){
      Length = random(4, values[3]);
      maxVerweildauer= random(Length + 20, values[6]);
      Schnuppenpos= random(maxVerweildauer - Length * 0.5);
      Schnuppenpausentimer= millis();
      Schnuppenpause= random(fertigevalues[5], fertigevalues[4]);
      cometspeed= random(values[9]);
      

      
    }
  }
}

void Taster(){
  if(millis() - tasterTimer > 200 && (digitalRead(BUTTON_PIN) == 1))
  {
      if(Menupunkt < 9)     // 1 ersetzen!
      {
        Menupunkt++;
      }
      else
      {
        Menupunkt = 0;
      }
      tasterTimer = millis();
      letzterstatus = analogRead(A0);
      letzterstatusvergleich = 0;

      if(Menupunkt == 7)   // Fixed problem with reset of Schnuppenpause
      {
        Schnuppenpause = millis();
      }

  }
  if(letzterstatus - analogRead(A0) >= 20 || letzterstatus - analogRead(A0) <= -20)
  {
    letzterstatusvergleich = 1;
  }
  if(millis() - refreshTimer >201 && letzterstatusvergleich == 1)
  {
    values[Menupunkt] = map(analogRead(A0), 0, 1024, minvalues[Menupunkt], maxvalues[Menupunkt] + 1);
    refreshTimer = millis();
  }
}

   

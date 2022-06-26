#include "Keyboard.h"
#include "Mouse.h"
#define WHEEL_INCREMENT 20

#define RELAY_PIN 8
#define GND_PIN 6
#define SUPPLY_PIN 7

#define LED_COUNT 12
#define UPDATES_PER_SECOND 40
#include <FastLED.h>

#define LED_PIN     9
#define NUM_LEDS    12
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

CRGBPalette16 colorPalette;
CRGBPalette16 colorPalette2;
CRGBPalette16 colorPaletteRB;


// Includes enum definition of GES_* return values from readGesture()
#include "RevEng_PAJ7620.h"

// Create gesture sensor driver object
RevEng_PAJ7620 sensor = RevEng_PAJ7620();




void setup() {
  Serial.begin(9600);
  Serial.println("PAJ7620 sensor demo: Recognizing all 9 gestures.");
  
  //=========Gesture sensor init============
  if( !sensor.begin() )           // return value of 0 == success
  {
    Serial.print("PAJ7620 I2C error - halting");
    while(true) { }
  }
  Serial.println("PAJ7620 init: OK");

  //=========Led strip init============
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  colorPalette = RainbowColors_p;//CRGBPalette16(CRGB::Red, CRGB::Blue);
  colorPalette2 = RainbowStripeColors_p;//CRGBPalette16(CRGB::Red, CRGB::Blue);
  colorPaletteRB = CRGBPalette16(CRGB::Red, CRGB::Blue);


  //=========Mouse keyboard===========
  //Mouse.begin();
  //Keyboard.begin();
  
  //==========Relay===================
  pinMode(RELAY_PIN,OUTPUT);
  digitalWrite(RELAY_PIN,HIGH);
  //add a GND pin
  pinMode(GND_PIN,OUTPUT);
  digitalWrite(GND_PIN,LOW);
  //add a 5V pin
  pinMode(SUPPLY_PIN,OUTPUT);
  digitalWrite(SUPPLY_PIN,HIGH);
}

void loop() {
    Gesture gesture;                  // Gesture is an enum type from RevEng_PAJ7620.h
    gesture = sensor.readGesture();   // Read back current gesture (if any) of type Gesture
  
    switch (gesture)
    {
      case GES_FORWARD:
        {
          Serial.println("Forwards");
          run_forward_animation();
          break;
        }
  
      case GES_BACKWARD:
        {
          Serial.println("Backwards");
          run_backward_animation();
          break;
        }
  
      case GES_LEFT:
        {
          Serial.println("Left");
          run_right_animation();
//          Keyboard.write(KEY_RIGHT_ARROW);
          break;
        }
  
      case GES_RIGHT:
        {
          Serial.println("Right");
          run_left_animation();
//          Keyboard.write(KEY_LEFT_ARROW);
          break;
        }
  
      case GES_UP:
        {
          Serial.println("SelectRobotScrollDown");
//          Keyboard.write(KEY_UP_ARROW);
//          Mouse.move(0, 0, WHEEL_INCREMENT);
          run_up_animation();
          break;
        }
  
      case GES_DOWN:
        {
          Serial.println("SelectRobotScrollUp");
//          Keyboard.write(KEY_DOWN_ARROW);
//          Mouse.move(0, 0, -WHEEL_INCREMENT);
          run_down_animation();
          break;
        }
  
      case GES_CLOCKWISE:
        {
          Serial.println("TurnClockwise");
          switch_relay_on();
          run_clockwise_animation();
          run_clockwise_animation();
          break;
        }
  
      case GES_ANTICLOCKWISE:
        {
          Serial.println("TurnCounterClockwise");
          switch_relay_off();
          run_anticlockwise_animation();
          run_anticlockwise_animation();
          break;
        }
  
      case GES_WAVE:
        {
          Serial.println("Quit");
          run_wave_animation();
          break;
        }
  
      case GES_NONE:
        {
          break;
        }
    }
  
//    if( gesture != GES_NONE )
//    {
//      Serial.println("Clear");
//      Serial.print(", Code: ");
//      Serial.println(gesture);
//    }
  
    delay(1000 / UPDATES_PER_SECOND);
    FastLED.show();
}


//---------------------------------------------//
void run_clockwise_animation(){
  
  for(int i=0;i<LED_COUNT;i++){
    leds[i] = ColorFromPalette( colorPalette, int(i*255/LED_COUNT));
    FastLED.show();
    leds[i] = CRGB::Black;
    delay(1000 / UPDATES_PER_SECOND);
  }
  FastLED.show();
}

void run_anticlockwise_animation(){
    for(int i=LED_COUNT-1;i>=0;i--){
      leds[i] = ColorFromPalette( colorPalette, int(i*255/LED_COUNT));
      FastLED.show();
      leds[i] = CRGB::Black;
      delay(1000 / UPDATES_PER_SECOND);
    }
    FastLED.show();
}


//---------------------------------------------//
void run_left_animation(){
  //5,4,3,2,1,0 avec 6,7,8,9,10,11
  int indexesPart1[7] = {1,2,3,4,5,6,7};
  int indexesPart2[7] = {1,0,11,10,9,8,7};
  
  for(int i=0;i<7;i++){
    int indexPart1 = indexesPart1[i];
    int indexPart2 = indexesPart2[i];
    
    leds[indexPart1] = ColorFromPalette( colorPalette, int(i*255/6));
    leds[indexPart2] = ColorFromPalette( colorPalette, int(i*255/6));
    FastLED.show();
    leds[indexPart1] = CRGB::Black;
    leds[indexPart2] = CRGB::Black;

    delay(2*1000 / UPDATES_PER_SECOND);
  }
  
  FastLED.show();
}

void run_right_animation(){
  //0,1,2,3,4,5 avec 11,10,9,8,7,6
  
  int indexesPart1[7] = {5,6,5,4,3,2,1};
  int indexesPart2[7] = {7,8,9,10,11,0,1};
  
  for(int i=0;i<7;i++){
    int indexPart1 = indexesPart1[i];
    int indexPart2 = indexesPart2[i];
    
    leds[indexPart1] = ColorFromPalette( colorPalette, int(i*255/6));
    leds[indexPart2] = ColorFromPalette( colorPalette, int(i*255/6));
    FastLED.show();
    leds[indexPart1] = CRGB::Black;
    leds[indexPart2] = CRGB::Black;

    delay(2*1000 / UPDATES_PER_SECOND);
  }
  
  FastLED.show();
    
}

//---------------------------------------------//
void run_up_animation(){
  //3,4,5,6,7,8 avec 3,2,1,0,11,10,9
    int indexesPart1[7] = {10,9,8,7,6,5,4};
    int indexesPart2[7] = {10,11,0,1,2,3,4};
  
  for(int i=0;i<7;i++){
    int indexPart1 = indexesPart1[i];
    int indexPart2 = indexesPart2[i];
    
  
    
    leds[indexPart1] = ColorFromPalette( colorPalette, int(i*255/6));
    leds[indexPart2] = ColorFromPalette( colorPalette, int(i*255/6));
    FastLED.show();
    leds[indexPart1] = CRGB::Black;
    leds[indexPart2] = CRGB::Black;

    delay(2*1000 / UPDATES_PER_SECOND);
  }
  
  FastLED.show();
  
}

void run_down_animation(){
  //3,4,5,6,7,8 avec 3,2,1,0,11,10,9
  int indexesPart1[7] = {4,5,6,7,8,9,10};
  int indexesPart2[7] = {4,3,2,1,0,11,10};
  
  for(int i=0;i<7;i++){
    int indexPart1 = indexesPart1[i];
    int indexPart2 = indexesPart2[i];
    
    leds[indexPart1] = ColorFromPalette( colorPalette, int(i*255/6));
    leds[indexPart2] = ColorFromPalette( colorPalette, int(i*255/6));
    FastLED.show();
    leds[indexPart1] = CRGB::Black;
    leds[indexPart2] = CRGB::Black;

    delay(2*1000 / UPDATES_PER_SECOND);
  }
  
  FastLED.show();
}

//---------------------------------------------//
void run_forward_animation(){
  FastLED.setBrightness(0);
    
    for(int j=0;j<LED_COUNT;j++){
      leds[j] = CRGB::Purple;
    }
    
    for(int i=0;i<255;i++){
      FastLED.setBrightness(i);
      FastLED.show();
      delay(1);
     }
    for(int i=255;i>=0;i--){
      FastLED.setBrightness(i);
      FastLED.show();
      delay(1);
     }
     FastLED.setBrightness(255);
    for(int j=0;j<LED_COUNT;j++){
      leds[j] = CRGB::Black;
    }
}

void run_backward_animation(){
  FastLED.setBrightness(0);
    
    for(int j=0;j<LED_COUNT;j++){
      leds[j] = CRGB::Red;
    }
    
    for(int i=0;i<255;i++){
      FastLED.setBrightness(i);
      FastLED.show();
      delay(1);
     }
    for(int i=255;i>=0;i--){
      FastLED.setBrightness(i);
      FastLED.show();
      delay(1);
     }
     FastLED.setBrightness(255);
    for(int j=0;j<LED_COUNT;j++){
      leds[j] = CRGB::Black;
    }
}

void run_wave_animation(){
  int indexesPart1[7] = {5,6,5,4,3,2,1};
  int indexesPart2[7] = {7,8,9,10,11,0,1};
  
  for(int i=0;i<7;i++){
    int indexPart1 = indexesPart1[i];
    int indexPart2 = indexesPart2[i];
    
    leds[indexPart1] = CRGB::Pink;
    leds[indexPart2] = CRGB::Pink;
    FastLED.show();
    leds[indexPart1] = CRGB::Black;
    leds[indexPart2] = CRGB::Black;

    delay(0.75*1000 / UPDATES_PER_SECOND);
  }
  int indexesPart3[7] = {1,2,3,4,5,6,7};
  int indexesPart4[7] = {1,0,11,10,9,8,7};
  
  for(int i=0;i<7;i++){
    int indexPart3 = indexesPart3[i];
    int indexPart4 = indexesPart4[i];
    
    leds[indexPart3] = CRGB::Purple;
    leds[indexPart4] = CRGB::Purple;
    FastLED.show();
    leds[indexPart3] = CRGB::Black;
    leds[indexPart4] = CRGB::Black;

    delay(0.75*1000 / UPDATES_PER_SECOND);
  }
  
  for(int i=0;i<7;i++){
    int indexPart1 = indexesPart1[i];
    int indexPart2 = indexesPart2[i];
    
    leds[indexPart1] = CRGB::Pink;
    leds[indexPart2] = CRGB::Pink;
    FastLED.show();
    leds[indexPart1] = CRGB::Black;
    leds[indexPart2] = CRGB::Black;

    delay(0.75*1000 / UPDATES_PER_SECOND);
  }
  
  for(int i=0;i<7;i++){
    int indexPart3 = indexesPart3[i];
    int indexPart4 = indexesPart4[i];
    
    leds[indexPart3] = CRGB::Purple;
    leds[indexPart4] = CRGB::Purple;
    FastLED.show();
    leds[indexPart3] = CRGB::Black;
    leds[indexPart4] = CRGB::Black;

    delay(0.75*1000 / UPDATES_PER_SECOND);
  }
  FastLED.show();
  
}

//-------------------------------------------------//
void switch_relay_on(){
  digitalWrite(RELAY_PIN,LOW);
}
void switch_relay_off(){
  digitalWrite(RELAY_PIN,HIGH);
}

#include "NeoPixelParallel.h"
#include "Particle.h"
#include "ColorUtils.h"
#include "Animation.h"
#include "Disco.h"
#include "Rain.h"
#include "Led.h"
#include "Wire.h"
#include "MultiBoom.h"
#include "ModeIndicator.h"
#include "Button.h"
#include "TopLight.h"
#include "Cylon.h"
#include "StaticColor.h"
#include "Breath.h"
#include "Checkers.h"

#if defined(__AVR_ATmega32U4__)
#define ARDUINO_IS_PRO_MICRO  1
#else
#define ARDUINO_IS_PRO_MICRO  0
#endif

#if (ARDUINO_IS_PRO_MICRO)
Led led(17);
#define PIN_BUTTON_A  9
#define PIN_BUTTON_B  8
#define PIN_BUTTON_C  7
#else
Led led(7);
#define PIN_BUTTON_A  14
#define PIN_BUTTON_B  15
#define PIN_BUTTON_C  16
#endif

#define ROWS_NUM  24
#define STRIPS_NUM  3

//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
MultiNeoPixel strip = MultiNeoPixel(STRIPS_NUM, ROWS_NUM, NEO_GRB + NEO_KHZ800);

static const uint32_t thresholdMs = 150;
static const int16_t thresholdG = 1200;


Button buttonA(PIN_BUTTON_A);
Button buttonB(PIN_BUTTON_B);
Button buttonC(PIN_BUTTON_C);

Disco disco(strip, false);
Rain rain(strip, true);
ParticleSystem particles(strip, false);
MultiBoom boom(strip, true);
Cylon cylon(strip, false);
Checkers checkers(strip, false);

#define STATIC_LENGTH   ROWS_NUM

TopLight redLight(strip, buttonC, Color(255, 0, 0), 4, false);
TopLight greenLight(strip, buttonC, Color(0, 255, 0), 4, false);
TopLight blueLight(strip, buttonC, Color(0, 0, 255), 4, false);
StaticColor staticRed(strip, Color(255, 0, 0), 0, STATIC_LENGTH, false);
StaticColor staticGreen(strip, Color(0, 255, 0), 0, STATIC_LENGTH, false);
StaticColor staticBlue(strip, Color(0, 0, 255), 0, STATIC_LENGTH, false);
StaticColor staticCyan(strip, Color(0, 190, 190), 0, STATIC_LENGTH, false);
StaticColor staticYellow(strip, Color(190, 190, 0), 0, STATIC_LENGTH, false);
StaticColor staticMagenta(strip, Color(190, 0, 190), 0, STATIC_LENGTH, false);
StaticColor staticDarkRed(strip, Color(40, 0, 0), 0, STATIC_LENGTH, false);
StaticColor staticDarkGreen(strip, Color(0, 40, 0), 0, STATIC_LENGTH, false);
StaticColor staticDarkBlue(strip, Color(0, 0, 40), 0, STATIC_LENGTH, false);
Breath breathPurple(strip, Color(128, 0, 128), 0, STATIC_LENGTH, false);
Breath breathGreen(strip, Color(0, 128, 128), 0, STATIC_LENGTH, false);

Animation* s_Animations[] = {
  &disco,
  &rain,  
  &particles,
  &cylon,
  &checkers,
  &breathPurple,
  &breathGreen,
  &staticRed,
  &staticGreen,
  &staticBlue,
  &staticYellow,
  &staticCyan,
  &staticMagenta,
  &staticDarkRed,
  &staticDarkGreen,
  &staticDarkBlue,
  &redLight,
  &greenLight,
  &blueLight,
  &boom
};


static const int s_AnimationsCount = sizeof(s_Animations) / sizeof(Animation*);

Animation* s_IdleAnimations[] = {
  &rain,
  &cylon,
  &disco,
  &checkers,
  &particles,
  &breathPurple,
  &breathGreen,
  &staticRed,
  &staticGreen,
  &staticBlue,
  &staticYellow,
  &staticCyan,
  &staticMagenta,  
  &staticDarkRed,
  &staticDarkGreen,
  &staticDarkBlue,
};
#define IDLE_COUNT (sizeof(s_IdleAnimations) / sizeof(Animation*))

Animation* s_MotionAnimations[] = {
  &boom,
  &redLight,
  &greenLight,
  &blueLight,
};
#define MOTION_COUNT (sizeof(s_MotionAnimations) / sizeof(Animation*))


volatile uint8_t modeValA = 0;
volatile uint8_t modeValB = 0;

#define MODES_NUM_A   IDLE_COUNT
#define MODES_NUM_B   (MOTION_COUNT + 1)

#define MODE_A_FIRST  0
#define MODE_A_LAST   (MODE_A_FIRST + MODES_NUM_A - 1)

#define MODE_B_FIRST  (ROWS_NUM - MODES_NUM_B - 1)
#define MODE_B_LAST   (MODE_B_FIRST + MODES_NUM_B - 1)

ModeIndicator modeA(strip, &modeValA, true, MODE_A_FIRST, MODE_A_LAST);
ModeIndicator modeB(strip, &modeValB, true, MODE_B_FIRST, MODE_B_LAST);

static void setModeA(uint8_t modeA) {
  for (Animation** a = s_IdleAnimations; a != s_IdleAnimations + IDLE_COUNT; ++a) {
    (*a)->setActive(false);
  }
  s_IdleAnimations[modeA]->setActive(true);
}

static void setModeB(uint8_t modeB) {
  for (Animation** a = s_MotionAnimations; a != s_MotionAnimations + MOTION_COUNT; ++a) {
    (*a)->setActive(false);
  }
  if (modeB < MODES_NUM_B) {
    s_MotionAnimations[modeB]->setActive(true);  
  }
}


static void cycleModeA() {
  modeValA = (modeValA + 1) % MODES_NUM_A;
  setModeA(modeValA);
}

static void cycleModeB() {
  modeValB = (modeValB + 1) % MODES_NUM_B;
  setModeB(modeValB);
}



// Common main-loop state. consider struct/class.
unsigned long last_update = 0;
unsigned long current_time = 0;

void setup() {
  //Serial.begin(9600);
  randomSeed(analogRead(8));

  setModeA(0);
  setModeB(0);

//  Serial.begin(38400);
  Wire.begin();
  led.begin();

  buttonA.begin();
  buttonB.begin();
  buttonC.begin();

  for (Animation** a = s_Animations; a != s_Animations + s_AnimationsCount; ++a) {
    (*a)->begin();
  }

  modeA.begin();
  modeB.begin();

  strip.setModeAny();
  strip.clearAll();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  led.on();
  delay(5000);
  led.off();
  
  last_update = millis();
}

void onStep() {
  //random_blips(3, 10);
  if (boom.isActive()) {
    boom.explodeOne((float)random(40, 150));
  }
}

static uint32_t frame = 0;

void loop() {
  current_time = millis();

  buttonA.read();
  buttonB.read();
  buttonC.read();

  if (buttonA.shouldHandleOn()) {
    cycleModeA();
    modeB.forceChange();
  }

  if (buttonB.shouldHandleOn()) {
    cycleModeB();
    modeA.forceChange();
  }
  
  led.set(buttonC.isPressed());
  if (buttonC.shouldHandleOn()) {
    onStep();
  }

  strip.multAll(7, 10);

  bool isModeIndication = false;

  if ((modeA.shouldDraw()) || (modeB.shouldDraw())) {
    isModeIndication = true;
    modeA.draw();
    modeB.draw();
  }

  if (!isModeIndication) {
    for (Animation** a = s_Animations; a != s_Animations + s_AnimationsCount; ++a) {
      if ((*a)->isActive()) {
        (*a)->draw();
      }
    }
  }

  last_update = current_time;

  strip.show();

  int16_t waitTime = 33 - (int16_t)(millis() - current_time);
  if (waitTime < 5) {
    waitTime = 5;  // Minimal delay - necessary!
  }
  delay(waitTime); // important to have this!  

  frame++;
}


#define INC_MOD(x, lim) (x)++; if ((x) >= (lim)) x=0;

/*
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<768; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) % 768));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<768*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) % 768));
    }
    strip.show();
    delay(wait);
  }
}

*/

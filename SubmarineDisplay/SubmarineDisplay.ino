/**
 * Submarine Torpedo Game
 * 
 * copyright Jeremy Franklin-Ross, Pacific Studios 2020
 */
#include <FastLED.h>


// BOAT LENGTHS & SPEEDS
#define WARSHIP_LENGTH                  5
#define MERCHANT_SHIP_LENGTH            4
#define WARSHIP_SPEED_MS_INTERVAL    100
#define MARCHANT_SPEED_MS_INTERVAL   200

// TORPEDO LENGTHS & SPEEDS
#define SLOW_TORPEDO_LEN             3 
#define FAST_TORPEDO_LEN             2
#define FAST_TORPEDO_SPEED_MS_INTER  50
#define SLOW_TORPEDO_SPEED_MS_INTER  100

// BUTTON, HIT, and MISS LIGHT FLASH SPEEDS
#define BOAT_LED_FLASH_MS_INTERVAL   750
#define TORPEDO_LED_FLASH_MS_INTERVAL 600
#define START_LED_FLASH_MS_INTERVAL  250
#define FIRE_LED_FLASH_MS_INTERVAL   250
#define HIT_FLASH_MS_INTERVAL        250
#define HIT_FLASH_COUNT              4
#define MISS_FLASH_COUNT 1
#define MISS_FLASH_INTERVAL 1000

// Timeout for input select and attact mode
#define INPUT_TIMEOUT_INTERVAL 15000
// 15 minutes in millis
#define INPUT_ATTRACT_TIMEOUT_INTERVAL 900000

// INPUT PINS for buttons and torpedo selectors
#define FIRE_BUTTON              0
#define TORPEDO_FAST_BUTTON      3
#define TORPEDO_SLOW_BUTTON      4
#define START_BUTTON             5
#define MERCHANT_VESSEL_BUTTON   6
#define WARSHIP_BUTTON           7
#define TORPEDO_0_SELECT             A0
#define TORPEDO_1_SELECT             A1
#define TORPEDO_2_SELECT             A2
#define TORPEDO_3_SELECT             A3
#define TORPEDO_4_SELECT             A4
#define TORPEDO_5_SELECT             A5

// Simple light output pins for button lights and 
// the hit indicators lights
#define MERCHANT_VESSEL_BUTTON_LED   22
#define WARSHIP_BUTTON_LED           23
#define TORPEDO_FAST_BUTTON_LED      24
#define TORPEDO_SLOW_BUTTON_LED      25
#define START_BUTTON_LED             26
#define FIRE_BUTTON_LED              27
#define STRIP_0_HIT_LED              A8
#define STRIP_1_HIT_LED              A9
#define STRIP_2_HIT_LED              A10
#define STRIP_3_HIT_LED              A11
#define STRIP_4_HIT_LED              A12
#define STRIP_5_HIT_LED              A13

//THE neopixel data pin
#define LED_DATA_PIN 53
#define PIXEL_COUNT              201

// Colors
#define SLOW_TORPEDO_CRGB CRGB::Maroon
#define FAST_TORPEDO_CRGB CRGB::Maroon
#define WARSHIP_CRGB CRGB::Teal
#define MERCHANT_SHIP_CRGB CRGB::Chartreuse
#define SHIP_MISS_CRGB CRGB::Green

    
// Strip brightnetss 1 to 255
#define LED_BRIGHTNESS           255

// Boat strip begin and end positions
#define BOAT_STRIP_BEGIN_POS     0
#define BOAT_STRIP_END_POS       63

// For each torpedo LED strip position defines positions for begin, end, 
// and the point it intersects with boat strip. We also define the 
// position where the boat strip intersects with the torpedo strip.

#define TORPEDO_STRIP_0_BEGIN_POS 64
#define TORPEDO_STRIP_0_INTERSECT_POS 83
#define BOAT_STRIP_0_INTERSECT_POS 21
#define TORPEDO_STRIP_0_END_POS 86
#define TORPEDO_STRIP_1_BEGIN_POS 87
#define TORPEDO_STRIP_1_INTERSECT_POS 103
#define BOAT_STRIP_1_INTERSECT_POS 28
#define TORPEDO_STRIP_1_END_POS 106
#define TORPEDO_STRIP_2_BEGIN_POS 107
#define TORPEDO_STRIP_2_INTERSECT_POS 122
#define BOAT_STRIP_2_INTERSECT_POS 34
#define TORPEDO_STRIP_2_END_POS 126
#define TORPEDO_STRIP_3_BEGIN_POS 127
#define TORPEDO_STRIP_3_INTERSECT_POS 143
#define BOAT_STRIP_3_INTERSECT_POS 40
#define TORPEDO_STRIP_3_END_POS 146
#define TORPEDO_STRIP_4_BEGIN_POS 147
#define TORPEDO_STRIP_4_INTERSECT_POS 166
#define BOAT_STRIP_4_INTERSECT_POS 48
#define TORPEDO_STRIP_4_END_POS 169
#define TORPEDO_STRIP_5_BEGIN_POS 170
#define TORPEDO_STRIP_5_INTERSECT_POS 196
#define BOAT_STRIP_5_INTERSECT_POS 59
#define TORPEDO_STRIP_5_END_POS 200

// number of torpedo strips
#define TORPEDO_STRIP_COUNT 6

// Game states, for a low key state machine
enum GAME_STATE {
  DEMO_TEST,
  WAITING_FOR_BOAT_AND_TORPEDO_SELECTION,
  WAITING_FOR_BOAT_SELECTION_ONLY,
  WAITING_FOR_TORPEDO_SELECTION_ONLY,
  WAITING_FOR_START,
  WAITING_FOR_TORPEDO_FIRE,
  WAITING_FOR_TORPEDO_END,
  TORPEDO_HIT,
  TORPEDO_MISS,
  TORPEDO_MISS_WAITING_FOR_BOAT,
  BOAT_COMPLETE_WAITING_FOR_TORPEDO,
  BOAT_COMPLETE,
  RESET_GAME,
  ATTRACT_INITIATE,
  ATTRACT_WAITNG_FOR_FIRE,
  ATTRACT_WAITING_FOR_TORPEDO_END
};

// Inital game state is waiting or boat
GAME_STATE currentGameState = WAITING_FOR_BOAT_AND_TORPEDO_SELECTION;

// Define the array of leds
CRGB leds[PIXEL_COUNT];

// timers for select timeout and attact mode
long startedSelectionTimeOutTimer = millis();
long startedAttractTimeOutTimer = millis();
long fireTorpedoTime = millis();

//HIT leds for each strip
int stripHitLEDs[6] = {STRIP_0_HIT_LED, STRIP_1_HIT_LED, STRIP_2_HIT_LED, 
                       STRIP_3_HIT_LED, STRIP_4_HIT_LED, STRIP_5_HIT_LED};

// multidimensional array holds all the torpedo and boat strip intersection data
int strip_locations[6][4] = {
  {TORPEDO_STRIP_0_BEGIN_POS,TORPEDO_STRIP_0_INTERSECT_POS,BOAT_STRIP_0_INTERSECT_POS,TORPEDO_STRIP_0_END_POS}, 
  {TORPEDO_STRIP_1_BEGIN_POS,TORPEDO_STRIP_1_INTERSECT_POS,BOAT_STRIP_1_INTERSECT_POS,TORPEDO_STRIP_1_END_POS}, 
  {TORPEDO_STRIP_2_BEGIN_POS,TORPEDO_STRIP_2_INTERSECT_POS,BOAT_STRIP_2_INTERSECT_POS,TORPEDO_STRIP_2_END_POS}, 
  {TORPEDO_STRIP_3_BEGIN_POS,TORPEDO_STRIP_3_INTERSECT_POS,BOAT_STRIP_3_INTERSECT_POS,TORPEDO_STRIP_3_END_POS}, 
  {TORPEDO_STRIP_4_BEGIN_POS,TORPEDO_STRIP_4_INTERSECT_POS,BOAT_STRIP_4_INTERSECT_POS,TORPEDO_STRIP_4_END_POS}, 
  {TORPEDO_STRIP_5_BEGIN_POS,TORPEDO_STRIP_5_INTERSECT_POS,BOAT_STRIP_5_INTERSECT_POS,TORPEDO_STRIP_5_END_POS}  
  };

int getTorpedoStripBegin(int torpedo) {
  return strip_locations[torpedo][0];
}

int getTorpedoStripIntersect(int torpedo) {
  return strip_locations[torpedo][1];
}

int getBoatStripIntersect(int torpedo) {
  return strip_locations[torpedo][2];
}

int getTorpedoStripEnd(int torpedo) {
  return strip_locations[torpedo][3];
}

// SELECTED TORPEDO TRACK 0-5
int currentSelectedTrack = 0; 

int getTorpedoStripBegin() {
  return getTorpedoStripBegin(currentSelectedTrack);
}

int getTorpedoStripIntersect() {
  return getTorpedoStripIntersect(currentSelectedTrack); 
}

int getBoatStripIntersect() {
  return getBoatStripIntersect(currentSelectedTrack);
}

int getTorpedoStripEnd() {
  return getTorpedoStripEnd(currentSelectedTrack);
}

int getBoatStripBegin() {
  return BOAT_STRIP_BEGIN_POS;
}

int getBoatStripEnd() {
  return BOAT_STRIP_END_POS;
}

enum BOAT_SELECT_STATES {
  merchant_selected,
  warship_selected,
  no_boat_selected
};

enum TORPEDO_SELECT_STATES {
  fast_selected,
  slow_selected,
  no_torpedo_selected
};

BOAT_SELECT_STATES currentBoatSelectState = no_boat_selected;
TORPEDO_SELECT_STATES currentTorpedoSelectState = no_torpedo_selected;

CRGB getBoatColor() {
  switch(currentBoatSelectState) {
    case merchant_selected : return MERCHANT_SHIP_CRGB;
    case warship_selected : return WARSHIP_CRGB;
    default: return WARSHIP_CRGB;
  }
}

int getBoatLen() {
  switch(currentBoatSelectState) {
    case merchant_selected : return MERCHANT_SHIP_LENGTH;
    case warship_selected : return WARSHIP_LENGTH;
    default: return 0;
  }
}

int getBoatSpeed() {
  switch(currentBoatSelectState) {
    case merchant_selected : return MARCHANT_SPEED_MS_INTERVAL;
    case warship_selected : return WARSHIP_SPEED_MS_INTERVAL;
    default: return 0;
  }
}

CRGB getTorpedoColor() {
  switch(currentTorpedoSelectState) {
    case fast_selected : return FAST_TORPEDO_CRGB;
    case slow_selected : return SLOW_TORPEDO_CRGB;
    default: return FAST_TORPEDO_CRGB;
  }
}


int getTorpedoLen() {
  switch(currentTorpedoSelectState) {
    case slow_selected : return SLOW_TORPEDO_LEN;
    case fast_selected : return FAST_TORPEDO_LEN;
    default: return 0;
  }
}

int getTorpedoSpeed() {
  switch(currentTorpedoSelectState) {
    case slow_selected : return SLOW_TORPEDO_SPEED_MS_INTER;
    case fast_selected : return FAST_TORPEDO_SPEED_MS_INTER;
    default: return 0;
  }
}





// Tracking position of torpedo
int torpedoTailCurrentPos = -1;

// Tracking position of boat
int boatTailCurrentPos = -1;

void clearLEDSection(int ledStart, int ledEnd) {
    for(int i = ledStart; i <= ledEnd; i++) { 
      leds[i] = CRGB::Black;
    }
}

void clearTorpedoStrip(int trackNum) {
  clearLEDSection(getTorpedoStripBegin(trackNum), getTorpedoStripEnd(trackNum));
}

void clearUnselectedTorpedoStrips() {
  for (int i = 0; i < TORPEDO_STRIP_COUNT; i++) {
    if (i != currentSelectedTrack) {
      clearTorpedoStrip(i);
    }
  }
}

void clearAllTorpedoStrips() {
  for (int i = 0; i < TORPEDO_STRIP_COUNT; i++) {
    clearTorpedoStrip(i);
  }
}

void clearSelectedTorpedoStrip() {
    clearLEDSection(getTorpedoStripBegin(), getTorpedoStripEnd());
}

//used when selecting track
void lightTorpedoInitialPosition() {
  clearUnselectedTorpedoStrips();
  clearSelectedTorpedoStrip();


  torpedoTailCurrentPos = getTorpedoStripBegin();
  for (int i = getTorpedoStripBegin(); i < (getTorpedoLen() + getTorpedoStripBegin()); i++) {
    leds[i]=getTorpedoColor();
  }
}

void clearBoatStrip() {
  clearLEDSection(getBoatStripBegin(), getBoatStripEnd());
}

void lightBoatInitialPosition() {
  clearBoatStrip();

  boatTailCurrentPos = getBoatStripBegin();
  for (int i = getBoatStripBegin(); i < (getBoatLen() + getBoatStripBegin()); i++) {
    leds[i]=getBoatColor();
  }
}

void selectTrack(int track) {
    currentSelectedTrack = track;
}

// select track and updates display
// return true means track selection changed
boolean selectTrack() {
  int selectCount = 0;
  int lastSelectedTrack = currentSelectedTrack;
  if (digitalRead(TORPEDO_0_SELECT) == LOW) {
    selectTrack(0);
    selectCount++;
  } 
  
  if (digitalRead(TORPEDO_1_SELECT) == LOW) {
    selectTrack(1);
    selectCount++;
  }
  
  if (digitalRead(TORPEDO_2_SELECT) == LOW) {
    selectTrack(2);
    selectCount++;
  }
  
  if (digitalRead(TORPEDO_3_SELECT) == LOW) {
    selectTrack(3);
    selectCount++;
  }
  
  if (digitalRead(TORPEDO_4_SELECT) == LOW) {
    selectTrack(4);
    selectCount++;
  }
  
  if (digitalRead(TORPEDO_5_SELECT) == LOW) {
    selectTrack(5);
    selectCount++;
  } 

  //log error if selectedCount > 1
  if (selectCount > 1) {
    Serial.println("ERROR: More than one track selected. Selected highest.");
  } 

  if (lastSelectedTrack == currentSelectedTrack) {
    return false;
  }
  lightTorpedoInitialPosition();
  return true;
}

boolean merchantIsIlluminated = false;
boolean slowTorpedoIsIlluminated = false;
boolean startIsIlluminated = false;
boolean fireIsIlluminated = false;
long lastBoatFlashTime = -1;
long lastTorpedoFlashTime = -1;
long lastStartButtonFlashTime = -1;
long lastFireButtonFlashTime = -1;


// Note side effects on ledIsIlluminated and last_alternate
void flashLED(boolean &ledIsIlluminated, long &last_alternate, long interval, int ledA) {
    if (millis() - last_alternate > interval) {  
      last_alternate = millis();
      if (ledIsIlluminated) {
        digitalWrite(ledA, LOW);
        ledIsIlluminated = false;
      } else {
        digitalWrite(ledA, HIGH);
        ledIsIlluminated = true;
      }
    }  
}

void flashStartLED() {
  flashLED(startIsIlluminated, lastStartButtonFlashTime, START_LED_FLASH_MS_INTERVAL, START_BUTTON_LED);
}

void flashFireLED() {
  flashLED(fireIsIlluminated, lastFireButtonFlashTime, FIRE_LED_FLASH_MS_INTERVAL, FIRE_BUTTON_LED);
}

void flashAlternateLEDs(boolean &ledIsIlluminated, long &last_alternate, long interval, int ledA, int ledB) {
    if (millis() - last_alternate > interval) {  
      last_alternate = millis();
      if (ledIsIlluminated) {
        digitalWrite(ledA, LOW);
        digitalWrite(ledB, HIGH);   
        ledIsIlluminated = false;
      } else {
        digitalWrite(ledA, HIGH);
        digitalWrite(ledB, LOW);   
        ledIsIlluminated = true;
      }
    }  
}

void flashTorpedoButtons() {
  flashAlternateLEDs(slowTorpedoIsIlluminated, lastTorpedoFlashTime, TORPEDO_LED_FLASH_MS_INTERVAL, 
                    TORPEDO_SLOW_BUTTON_LED, TORPEDO_FAST_BUTTON_LED);
}
                    
void flashBoatButtons() {
  flashAlternateLEDs(merchantIsIlluminated, lastBoatFlashTime, BOAT_LED_FLASH_MS_INTERVAL, 
                    MERCHANT_VESSEL_BUTTON_LED, WARSHIP_BUTTON_LED);
}

void resetButtonLEDs() {
  digitalWrite(START_BUTTON_LED, LOW);
  digitalWrite(FIRE_BUTTON_LED, LOW);
  digitalWrite(TORPEDO_SLOW_BUTTON_LED, LOW);
  digitalWrite(TORPEDO_FAST_BUTTON_LED, LOW);
  digitalWrite(MERCHANT_VESSEL_BUTTON_LED, LOW);
  digitalWrite(WARSHIP_BUTTON_LED, LOW);
  
  merchantIsIlluminated = false;
  slowTorpedoIsIlluminated = false;
  startIsIlluminated = false;
  fireIsIlluminated = false;
  lastBoatFlashTime = -1;
  lastTorpedoFlashTime = -1;
  lastStartButtonFlashTime = -1;
  lastFireButtonFlashTime = -1;
}


void updateBoatButtonLEDs() {
  switch(currentBoatSelectState) {
    case merchant_selected : 
        digitalWrite(MERCHANT_VESSEL_BUTTON_LED, HIGH);
        digitalWrite(WARSHIP_BUTTON_LED, LOW);    
        return;
    case warship_selected : 
        digitalWrite(MERCHANT_VESSEL_BUTTON_LED, LOW);
        digitalWrite(WARSHIP_BUTTON_LED, HIGH);    
        return;
    default: 
        digitalWrite(MERCHANT_VESSEL_BUTTON_LED, LOW);
        digitalWrite(WARSHIP_BUTTON_LED, LOW);    
  }
}


void updateTorpedButtonLEDs() {
  switch(currentTorpedoSelectState) {
    case fast_selected : 
        digitalWrite(TORPEDO_FAST_BUTTON_LED, HIGH);
        digitalWrite(TORPEDO_SLOW_BUTTON_LED, LOW);    
        return;
    case slow_selected : 
        digitalWrite(TORPEDO_FAST_BUTTON_LED, LOW);
        digitalWrite(TORPEDO_SLOW_BUTTON_LED, HIGH);    
        return;
    default: 
        digitalWrite(TORPEDO_FAST_BUTTON_LED, LOW);
        digitalWrite(TORPEDO_SLOW_BUTTON_LED, LOW);    
  }
}



boolean selectBoat() {
  int selectCount = 0;
  BOAT_SELECT_STATES lastSelected = currentBoatSelectState;

  if (digitalRead(MERCHANT_VESSEL_BUTTON) == LOW) {
    Serial.println("PRESSED MERCHANT_VESSEL_BUTTON");
    selectCount++;
    currentBoatSelectState = merchant_selected;
  } 
  
  if (digitalRead(WARSHIP_BUTTON) == LOW) {
    Serial.println("PRESSED WARSHIP_BUTTON");
    selectCount++;
    currentBoatSelectState = warship_selected;
  }

  //log error if selectedCount > 1
  if (selectCount > 1) {
    Serial.println("ERROR: More than one SHIP selected. Selected warship.");
  } 
  if (lastSelected == currentBoatSelectState) {
    return false; 
  } 

  //update selection buttons
  updateBoatButtonLEDs();
  //display boat
  lightBoatInitialPosition();
  return true;
}


boolean selectTorpedo() {
  int selectCount = 0;
  TORPEDO_SELECT_STATES lastSelected = currentTorpedoSelectState;

  if (digitalRead(TORPEDO_FAST_BUTTON) == LOW) {
    Serial.println("PRESSED TORPEDO_FAST_BUTTON");
    selectCount++;
    currentTorpedoSelectState = fast_selected;
  } 
  
  if (digitalRead(TORPEDO_SLOW_BUTTON) == LOW) {
    Serial.println("PRESSED TORPEDO_SLOW_BUTTON");
    selectCount++;
    currentTorpedoSelectState = slow_selected;
  }

  //log error if selectedCount > 1
  if (selectCount > 1) {
    Serial.println("ERROR: More than one TORPEDO selected. Selected slow.");
  } 
  if (lastSelected == currentTorpedoSelectState) {
    return false; 
  } 

  //update selection buttons
  lightTorpedoInitialPosition();
  updateTorpedButtonLEDs();
  return true;
}

void selectRandomBoat() {
  if (random(2)) {
    currentBoatSelectState = merchant_selected;
  } else {
    currentBoatSelectState = warship_selected;
  }
   
  //update selection buttons
  updateBoatButtonLEDs();
  //display boat
  lightBoatInitialPosition();
}

void selectRandomTorpedo() {
  if (random(2)) {
    currentTorpedoSelectState = fast_selected;   
  } else {
    currentTorpedoSelectState = slow_selected;
  }

  //update selection buttons
  lightTorpedoInitialPosition();
  updateTorpedButtonLEDs();
}


//note this has side effect of updating spritePos ... should return spritePos instead?
boolean advanceSprite(int &spritePos, int spriteLen, CRGB spriteColor, int stripBegin, int stripEnd) {
  // increment position
  if (spritePos == -1) {
    spritePos = stripBegin;
    for (int i = 0; i < spriteLen ; i++) {
      leds[spritePos + i] = spriteColor;
    }
    
  } else {
    spritePos++;  
  }
  
  if (spritePos > stripEnd) {
     leds[stripEnd] = CRGB::Black; 
    spritePos = -1; 
    return false;
  }

  if ((spritePos - 1) >= stripBegin) {
    leds[spritePos -1] = CRGB::Black;  
  }
  
  if (spritePos + spriteLen - 1 <= stripEnd) {
    leds[spritePos + spriteLen - 1] = spriteColor;
  }
  return true;
}

long last_ship_increment_ms = -1;

// ret val of false means ran out of room
boolean advanceShip() {
  // check time 
  if (millis() - last_ship_increment_ms > getBoatSpeed()) {
    last_ship_increment_ms = millis();
    // increment position
    return advanceSprite(boatTailCurrentPos, getBoatLen(), getBoatColor(), getBoatStripBegin(), getBoatStripEnd());
  }
  return true;
}

long last_torpedo_increment_ms = -1;

boolean advanceTorpedo() {
// check time
  if (millis() - last_torpedo_increment_ms > getTorpedoSpeed()) {
    last_torpedo_increment_ms = millis();
    // increment position
    return advanceSprite(torpedoTailCurrentPos, getTorpedoLen(), getTorpedoColor(), getTorpedoStripBegin(), getTorpedoStripEnd());
  }
    return true;
}


// Determine if any torpedo pixel intersection of the given torpedo track
boolean torpedoAtIntersection(int torpedoTailPos, int torpedoLen, int torpedoStripIntersect) {
  for (int torpedoPixel = torpedoTailPos; torpedoPixel < torpedoTailPos + torpedoLen; torpedoPixel++) {
    if (torpedoPixel == torpedoStripIntersect) {
        return true;
    }
  }
  return false;
}


// Determine if any torpedo pixel intersection of the current torpedo strip
boolean torpedoAtIntersection() {
  return torpedoAtIntersection(torpedoTailCurrentPos, getTorpedoLen(), getTorpedoStripIntersect());
}

// Determine if any boat pixel is at the given boat strip intersection 
boolean boatAtIntersection(int boatStripIntersect) {
  for (int boatPixel = boatTailCurrentPos; boatPixel < boatTailCurrentPos + getBoatLen(); boatPixel++) {
    if (boatPixel == boatStripIntersect) {
        return true;
    }
  }
  return false;
}


// Determine if any boat pixel is at the boat strip intersection of the current torpedo track
boolean boatAtIntersection() {
  return boatAtIntersection(getBoatStripIntersect());
}

boolean isCollision() {
  return (torpedoAtIntersection() && boatAtIntersection());
}


void displayImpactSequence(int track) {
  Serial.println("HIT!");
  
  for (int i = 0; i < HIT_FLASH_COUNT; i++) {
    digitalWrite(stripHitLEDs[track], HIGH);
    delay(HIT_FLASH_MS_INTERVAL);
    digitalWrite(stripHitLEDs[track], LOW);
    delay(HIT_FLASH_MS_INTERVAL);    
  }

}

void displayImpactSequence() {
  displayImpactSequence(currentSelectedTrack);
}

void displayMissSequence() {
//  Serial.println("MISS!");

  for (int flashCount = 0; flashCount < MISS_FLASH_COUNT; flashCount ++) {
    for (int i = getBoatStripBegin(); i < getBoatStripEnd() + 1; i++) {
       leds[i] = SHIP_MISS_CRGB;
    }
    
    FastLED.show(); 
    delay(MISS_FLASH_INTERVAL);
    
    for (int i = getBoatStripBegin(); i < getBoatStripEnd() + 1; i++) {
       leds[i] = CRGB::Black;
    }
    
    FastLED.show(); 
    delay(MISS_FLASH_INTERVAL);
  }

}

// Reset torpedo positiona and clear LEDS
void resetTorpedo() {
  torpedoTailCurrentPos = -1;
  clearSelectedTorpedoStrip();
}

// Reset boat positiona and clear LEDS
void resetBoat() {
  boatTailCurrentPos = -1;
  clearBoatStrip();
}


void setup() {
  Serial.begin(115200);
  Serial.println("resetting");
  LEDS.addLeds<WS2812,LED_DATA_PIN,GRB>(leds,PIXEL_COUNT);
  LEDS.setBrightness(LED_BRIGHTNESS);

  // Show start stops and intersections for debug purposes
  leds[BOAT_STRIP_BEGIN_POS] = CRGB::Green;
  leds[BOAT_STRIP_END_POS] = CRGB::Red;
  leds[TORPEDO_STRIP_0_BEGIN_POS] = CRGB::Green;
  leds[TORPEDO_STRIP_0_END_POS] = CRGB::Red;
  leds[TORPEDO_STRIP_1_BEGIN_POS] = CRGB::Green;
  leds[TORPEDO_STRIP_1_END_POS] = CRGB::Red;
  leds[TORPEDO_STRIP_2_BEGIN_POS] = CRGB::Green;
  leds[TORPEDO_STRIP_2_END_POS] = CRGB::Red;
  leds[TORPEDO_STRIP_3_BEGIN_POS] = CRGB::Green;
  leds[TORPEDO_STRIP_3_END_POS] = CRGB::Red;
  leds[TORPEDO_STRIP_4_BEGIN_POS] = CRGB::Green;
  leds[TORPEDO_STRIP_4_END_POS] = CRGB::Red;
  leds[TORPEDO_STRIP_5_BEGIN_POS] = CRGB::Green;
  leds[TORPEDO_STRIP_5_END_POS] = CRGB::Red;
  leds[TORPEDO_STRIP_0_INTERSECT_POS] = CRGB::Yellow;
  leds[BOAT_STRIP_0_INTERSECT_POS] = CRGB::Blue;
  leds[TORPEDO_STRIP_1_INTERSECT_POS] = CRGB::Yellow;
  leds[BOAT_STRIP_1_INTERSECT_POS] = CRGB::Blue;
  leds[TORPEDO_STRIP_2_INTERSECT_POS] = CRGB::Yellow;
  leds[BOAT_STRIP_2_INTERSECT_POS] = CRGB::Blue;
  leds[TORPEDO_STRIP_3_INTERSECT_POS] = CRGB::Yellow;
  leds[BOAT_STRIP_3_INTERSECT_POS] = CRGB::Blue;
  leds[TORPEDO_STRIP_4_INTERSECT_POS] = CRGB::Yellow;
  leds[BOAT_STRIP_4_INTERSECT_POS] = CRGB::Blue;
  leds[TORPEDO_STRIP_5_INTERSECT_POS] = CRGB::Yellow;
  leds[BOAT_STRIP_5_INTERSECT_POS] = CRGB::Blue;

  

  FastLED.show(); 
  delay(2500);

  clearAllTorpedoStrips();
  clearBoatStrip();


  currentBoatSelectState = BOAT_SELECT_STATES::no_boat_selected;

  //set up inputs and outputs
  pinMode(TORPEDO_0_SELECT, INPUT_PULLUP);
  pinMode(TORPEDO_1_SELECT, INPUT_PULLUP);
  pinMode(TORPEDO_2_SELECT, INPUT_PULLUP);
  pinMode(TORPEDO_3_SELECT, INPUT_PULLUP);
  pinMode(TORPEDO_4_SELECT, INPUT_PULLUP);
  pinMode(TORPEDO_5_SELECT, INPUT_PULLUP);
  pinMode(MERCHANT_VESSEL_BUTTON, INPUT_PULLUP);
  pinMode(WARSHIP_BUTTON, INPUT_PULLUP);
  pinMode(TORPEDO_FAST_BUTTON, INPUT_PULLUP);
  pinMode(TORPEDO_SLOW_BUTTON, INPUT_PULLUP);
  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(FIRE_BUTTON, INPUT_PULLUP);

  pinMode(MERCHANT_VESSEL_BUTTON_LED, OUTPUT);
  pinMode(WARSHIP_BUTTON_LED, OUTPUT);
  pinMode(TORPEDO_FAST_BUTTON_LED, OUTPUT);
  pinMode(TORPEDO_SLOW_BUTTON_LED, OUTPUT);
  pinMode(START_BUTTON_LED, OUTPUT);
  pinMode(FIRE_BUTTON_LED, OUTPUT);

  pinMode(STRIP_0_HIT_LED, OUTPUT);
  pinMode(STRIP_1_HIT_LED, OUTPUT);
  pinMode(STRIP_2_HIT_LED, OUTPUT);
  pinMode(STRIP_3_HIT_LED, OUTPUT);
  pinMode(STRIP_4_HIT_LED, OUTPUT);
  pinMode(STRIP_5_HIT_LED, OUTPUT);
  
}

//test
int torpedoTailCurrentPos0 = -1;
int torpedoTailCurrentPos1 = -1;
int torpedoTailCurrentPos2 = -1;
int torpedoTailCurrentPos3 = -1;
int torpedoTailCurrentPos4 = -1;
int torpedoTailCurrentPos5 = -1;

void demoTestAdvance() {
  //  advanceShip();
  advanceSprite(boatTailCurrentPos, getBoatLen(), getBoatColor(), getBoatStripBegin(), getBoatStripEnd());
  advanceSprite(torpedoTailCurrentPos0, getTorpedoLen(), getTorpedoColor(), getTorpedoStripBegin(0), getTorpedoStripEnd(0));
  advanceSprite(torpedoTailCurrentPos1, getTorpedoLen(), getTorpedoColor(), getTorpedoStripBegin(1), getTorpedoStripEnd(1));
  advanceSprite(torpedoTailCurrentPos2, getTorpedoLen(), getTorpedoColor(), getTorpedoStripBegin(2), getTorpedoStripEnd(2));
  advanceSprite(torpedoTailCurrentPos3, getTorpedoLen(), getTorpedoColor(), getTorpedoStripBegin(3), getTorpedoStripEnd(3));
  advanceSprite(torpedoTailCurrentPos4, getTorpedoLen(), getTorpedoColor(), getTorpedoStripBegin(4), getTorpedoStripEnd(4));
  advanceSprite(torpedoTailCurrentPos5, getTorpedoLen(), getTorpedoColor(), getTorpedoStripBegin(5), getTorpedoStripEnd(5));

  if (torpedoAtIntersection(torpedoTailCurrentPos0, getTorpedoLen(), getTorpedoStripIntersect(0))
      && boatAtIntersection(getBoatStripIntersect(0))) {
    Serial.println("0 HIT");  
  }

  if (torpedoAtIntersection(torpedoTailCurrentPos1, getTorpedoLen(), getTorpedoStripIntersect(1))
      && boatAtIntersection(getBoatStripIntersect(1))) {
    Serial.println("1 HIT");
  }

  if (torpedoAtIntersection(torpedoTailCurrentPos2, getTorpedoLen(), getTorpedoStripIntersect(2))
      && boatAtIntersection(getBoatStripIntersect(2))) {
    Serial.println("2 HIT");
  }

  if (torpedoAtIntersection(torpedoTailCurrentPos3, getTorpedoLen(), getTorpedoStripIntersect(3))
      && boatAtIntersection(getBoatStripIntersect(3))) {
    Serial.println("3 HIT");
  }

  if (torpedoAtIntersection(torpedoTailCurrentPos4, getTorpedoLen(), getTorpedoStripIntersect(4))
      && boatAtIntersection(getBoatStripIntersect(4))) {
    Serial.println("4 HIT");
  }

  if (torpedoAtIntersection(torpedoTailCurrentPos5, getTorpedoLen(), getTorpedoStripIntersect(5))
      && boatAtIntersection(getBoatStripIntersect(5))) {
    Serial.println("5 HIT");
  }

}

void resetSelectTimeOutTime() {
  startedSelectionTimeOutTimer = millis();
}

boolean isSelectionTimedOut() {
  return (millis() - startedSelectionTimeOutTimer) > INPUT_TIMEOUT_INTERVAL;
}


void resetAttractTimeOutTime() {
  startedAttractTimeOutTimer = millis();
}

boolean isAttractTimedOut() {
  return (millis() - startedAttractTimeOutTimer) > INPUT_ATTRACT_TIMEOUT_INTERVAL;
}

boolean isAnyButtonPressed() {
  return digitalRead(MERCHANT_VESSEL_BUTTON) == LOW ||
         digitalRead(WARSHIP_BUTTON) == LOW || 
         digitalRead(TORPEDO_FAST_BUTTON) == LOW || 
         digitalRead(TORPEDO_SLOW_BUTTON) == LOW || 
         digitalRead(FIRE_BUTTON) == LOW || 
         digitalRead(START_BUTTON) == LOW; 
}


void loop() {

switch(currentGameState) {
    case DEMO_TEST : 
          //TEST CODE HERE
           break;
    case WAITING_FOR_BOAT_AND_TORPEDO_SELECTION :  ;
//          Serial.println("WAITING_FOR_BOAT_AND_TORPEDO_SELECTION");
           // poll boat select
           if (!selectBoat()) {
              // flash boat select
              flashBoatButtons();
           } else {
              resetSelectTimeOutTime(); 
              currentGameState = WAITING_FOR_TORPEDO_SELECTION_ONLY;
              break;
           }

           // poll torpedo select
           if (!selectTorpedo()) {
              // flash torpedo select
              flashTorpedoButtons();
           } else {
              resetSelectTimeOutTime(); 
              currentGameState = WAITING_FOR_BOAT_SELECTION_ONLY;
              break;
           }

            //check for torpedo track changes
            if (selectTrack()) {
               resetSelectTimeOutTime(); 
            }

            // Go into Attract Mode after delay
            if (isAttractTimedOut()) {
              currentGameState = ATTRACT_INITIATE;
            }
         
             
            break;
    case WAITING_FOR_BOAT_SELECTION_ONLY :  ;
//           Serial.println("WAITING_FOR_BOAT_SELECTION_ONLY");   
            // bail after input time
            if (isSelectionTimedOut()) {
              currentGameState = RESET_GAME;
            }
         
            // poll torpedo select
            if (selectTorpedo()) {
              resetSelectTimeOutTime(); 
            }
            
           if (!selectBoat()) {
              // flash boat select
              flashBoatButtons();
           } else {
              resetSelectTimeOutTime(); 
              currentGameState = WAITING_FOR_START;
              break;
           }

            //check for torpedo track changes
            selectTrack();

             break;
    case WAITING_FOR_TORPEDO_SELECTION_ONLY :  ;
//               Serial.println("WAITING_FOR_TORPEDO_SELECTION_ONLY");
            // bail after input time
            if (isSelectionTimedOut()) {
              currentGameState = RESET_GAME;
            }

            // check for boat changes
            if (selectBoat()) {
              resetSelectTimeOutTime(); 
            }
            
           // poll torpedo select
           if (!selectTorpedo()) {
              // flash torpedo select
              flashTorpedoButtons();
           } else {
              resetSelectTimeOutTime(); 
              currentGameState = WAITING_FOR_START;
              break;
           }
           
            //check for torpedo track changes
            selectTrack();
            
             break;
    case WAITING_FOR_START :  ;
//                   Serial.println("WAITING_FOR_START");
            // bail after input time
            if (isSelectionTimedOut()) {
              currentGameState = RESET_GAME;
            }

            //check start button
            if (digitalRead(START_BUTTON) == LOW) {
              resetSelectTimeOutTime(); 
              currentGameState = WAITING_FOR_TORPEDO_FIRE;
              digitalWrite(START_BUTTON_LED, LOW);
              break;
            } else {
              //flash start button
              flashStartLED();
            }
            // check for boat changes
            if (selectBoat()) {
              resetSelectTimeOutTime(); 
            }
            
            //change torpedo
            if (selectTorpedo()) {
              resetSelectTimeOutTime(); 
            }
            
            //check for torpedo track changes
            if (selectTrack()) {
              resetSelectTimeOutTime(); 
            }

             break;
    case WAITING_FOR_TORPEDO_FIRE :  
//            Serial.println("WAITING_FOR_TORPEDO_FIRE");
           // bail after input time
            if (isSelectionTimedOut()) {
              currentGameState = RESET_GAME;
            }

            // poll fire button
            if (digitalRead(FIRE_BUTTON) == LOW) {
	          resetSelectTimeOutTime(); 
              currentGameState = WAITING_FOR_TORPEDO_END;
            } else {
              //flash Fire button
              flashFireLED();              
            }

            //check for torpedo track changes
            selectTrack();

            // TODO check timeout?
            
            // advance boat
            if (!advanceShip()) {
              currentGameState = RESET_GAME; 
            }
             break;
    case WAITING_FOR_TORPEDO_END :  ;
//              Serial.println("WAITING_FOR_TORPEDO_END");

            // advance boat 
            if (!advanceShip()) {
              currentGameState = BOAT_COMPLETE_WAITING_FOR_TORPEDO;
            }
            // advance torpedo 
            if (!advanceTorpedo()) {
              currentGameState = TORPEDO_MISS;
            }
            // check for hit
            if (isCollision()) {
              currentGameState = TORPEDO_HIT;
            }
             break;
    case TORPEDO_HIT :  ;
            // play hit sequence
            displayImpactSequence();
            //reset 
            resetTorpedo();
            resetBoat();
                        
            // reset to 
             currentGameState = RESET_GAME; 

             break;
    case TORPEDO_MISS :  ;
            // play miss sequence
            displayMissSequence(); 
            // TO run boat to end, change next to TORPEDO_MISS_WAITING_FOR_BOAT
//            currentGameState = TORPEDO_MISS_WAITING_FOR_BOAT; 
            currentGameState = RESET_GAME; 
            
            break;
    case TORPEDO_MISS_WAITING_FOR_BOAT :  ;
            if (!advanceShip()) {
              currentGameState = RESET_GAME; 
            }
            break;
    case BOAT_COMPLETE_WAITING_FOR_TORPEDO :  ;
            // advance torpedo 
            if (!advanceTorpedo()) {
              currentGameState = TORPEDO_MISS;
            }

            break;
    case RESET_GAME :  ;
        currentBoatSelectState = no_boat_selected;
        currentTorpedoSelectState = no_torpedo_selected;
        clearAllTorpedoStrips();
        clearBoatStrip();
        resetButtonLEDs();
        resetSelectTimeOutTime(); 
        resetAttractTimeOutTime();
        currentGameState = WAITING_FOR_BOAT_AND_TORPEDO_SELECTION;
        break;
    case ATTRACT_INITIATE :
      selectRandomBoat();
      selectRandomTorpedo();
      // set a random torpedo fire time within next few seconds
      fireTorpedoTime = millis() + random(5000); 

      currentGameState = ATTRACT_WAITNG_FOR_FIRE;
      break;
    case ATTRACT_WAITNG_FOR_FIRE :
      // abort if anybutton is touched
      if (isAnyButtonPressed() || selectTrack()) {
        currentGameState = RESET_GAME;
      }
      
      if (millis() > fireTorpedoTime) {
        currentGameState = WAITING_FOR_TORPEDO_END; 
      } else {
        //flash Fire button
        flashFireLED();              
      }
      
      if (!advanceShip()) {
        currentGameState = RESET_GAME; 
      }
      
      break;
    case ATTRACT_WAITING_FOR_TORPEDO_END :
      // abort if anybutton is touched
      if (isAnyButtonPressed()) {
        currentGameState = RESET_GAME;
      }

      // advance boat 
      if (!advanceShip()) {
        currentGameState = RESET_GAME; 
      }
      // advance torpedo 
      if (!advanceTorpedo()) {
        currentGameState = TORPEDO_MISS;
      }
      // check for hit
      if (isCollision()) {
        currentGameState = TORPEDO_HIT;
      }
      break;
     }
        
    FastLED.show(); 
}

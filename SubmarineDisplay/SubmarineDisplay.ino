#include <FastLED.h>

// TODO move all fire test code to 'example display mode" func 30m
// TODO add torpedo track select switch
// TODO add lights and flash sequence for collisions 1 h 
// TODO add boat start button  
// TODO add boat type select switch
// TODO add torpedo type select switch
// TODO fix red/green FastLED callibration
// TODO connect to actual selectors and buttons and lights
// TODO make fritzing diagram 
// TODO review narrative and implement gaps
// TODO implement game wait personality sequence
// TODO fix underscore variables to camel case

#define PIXEL_COUNT 200

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
//#define DATA_PIN 7
#define DATA_PIN 53
#define CLOCK_PIN 13

#define FIRE_BUTTON              0
#define TORPEDO_FAST_BUTTON      3
#define TORPEDO_SLOW_BUTTON      4
#define START_BUTTON             5
#define MERCHANT_VESSEL_BUTTON   6
#define WARSHIP_BUTTON           7


#define MERCHANT_VESSEL_BUTTON_LED   22
#define WARSHIP_BUTTON_LED           23
#define TORPEDO_FAST_BUTTON_LED      24
#define TORPEDO_SLOW_BUTTON_LED      25
#define START_BUTTON_LED             26
#define FIRE_BUTTON_LED              27


#define TORPEDO_0_SELECT             30
#define TORPEDO_1_SELECT             31
#define TORPEDO_2_SELECT             32
#define TORPEDO_3_SELECT             33
#define TORPEDO_4_SELECT             34
#define TORPEDO_5_SELECT             35


#define WARSHIP_LEN                  5
#define MERCHANT_SHIP_LEN            4
#define WARSHIP_SPEED_MS_INTERVAL    100
#define MARCHANT_SPEED_MS_INTERVAL   200
//TODO try larger slow torpedo
#define SLOW_TORPEDO_LEN             3 
#define FAST_TORPEDO_LEN             2
#define FAST_TORPEDO_SPEED_MS_INTER  50
#define SLOW_TORPEDO_SPEED_MS_INTER  100

#define BOAT_LED_FLASH_MS_INTERVAL          750
#define TORPEDO_LED_FLASH_MS_INTERVAL       600
#define START_LED_FLASH_MS_INTERVAL          250
#define FIRE_LED_FLASH_MS_INTERVAL          250


#define TORPEDO_CRGB CRGB::White
#define BOAT_CRGB CRGB::Blue


// LEDS IN STRIP
// 64 //boat
// 23 //0 
// 20 //1
// 19 //2
// 20 //3
// 23 //4
// 31 //5
#define PIXEL_COUNT              201
#define BOAT_STRIP_BEGIN_POS     0
#define BOAT_STRIP_END_POS       63
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

#define TRACK_COUNT 6


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
  RESET_GAME
};

//TODO change 
GAME_STATE currentGameState = WAITING_FOR_BOAT_AND_TORPEDO_SELECTION;
//GAME_STATE currentGameState = DEMO_TEST;

// Define the array of leds
CRGB leds[PIXEL_COUNT];


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
int selected_track = 0; 

int getTorpedoStripBegin() {
  return getTorpedoStripBegin(selected_track);
}

int getTorpedoStripIntersect() {
  return getTorpedoStripIntersect(selected_track); 
}

int getBoatStripIntersect() {
  return getBoatStripIntersect(selected_track);
}

int getTorpedoStripEnd() {
  return getTorpedoStripEnd(selected_track);
}

int getBoatStripBegin() {
  return BOAT_STRIP_BEGIN_POS;
}

int getBoatStripEnd() {
  return BOAT_STRIP_END_POS;
}


enum boat_select_states {
  merchant_selected,
  warship_selected,
  no_boat_selected
};

enum torpedo_select_states {
  fast_selected,
  slow_selected,
  no_torpedo_selected
};

boat_select_states boat_select_state = no_boat_selected;
torpedo_select_states torpedo_select_state = no_torpedo_selected;


int getBoatLen() {
  switch(boat_select_state) {
    case merchant_selected : return MERCHANT_SHIP_LEN;
    case warship_selected : return WARSHIP_LEN;
    default: return 0;
  }
}

int getBoatSpeed() {
  switch(boat_select_state) {
    case merchant_selected : return MARCHANT_SPEED_MS_INTERVAL;
    case warship_selected : return WARSHIP_SPEED_MS_INTERVAL;
    default: return 0;
  }
}


int getTorpedoLen() {
  switch(torpedo_select_state) {
    case slow_selected : return SLOW_TORPEDO_LEN;
    case fast_selected : return FAST_TORPEDO_LEN;
    default: return 0;
  }
}

int getTorpedoSpeed() {
  switch(torpedo_select_state) {
    case slow_selected : return SLOW_TORPEDO_SPEED_MS_INTER;
    case fast_selected : return FAST_TORPEDO_SPEED_MS_INTER;
    default: return 0;
  }
}



//TODO Move
#define DEFAULT_SELECTION_TIMEOUT 2000
#define DEFAULT_LIGHT_ALTERNATION_RATE 1000

long last_selection_time = -1;
long selection_timeout = DEFAULT_SELECTION_TIMEOUT;
long standby_light_alternation_rate = DEFAULT_LIGHT_ALTERNATION_RATE;


int torpedo_tail_current_pos = -1;



int boat_tail_current_pos = -1;

void clearLEDSection(int ledStart, int ledEnd) {
    for(int i = ledStart; i <= ledEnd; i++) { 
      leds[i] = CRGB::Black;
    }
}

void clearTorpedoStrip(int trackNum) {
  clearLEDSection(getTorpedoStripBegin(trackNum), getTorpedoStripEnd(trackNum));
}

void clearUnselectedTorpedoStrips() {
  for (int i = 0; i < TRACK_COUNT; i++) {
    if (i != selected_track) {
      clearTorpedoStrip(i);
    }
  }
}

void clearAllTorpedoStrips() {
  for (int i = 0; i < TRACK_COUNT; i++) {
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


  torpedo_tail_current_pos = getTorpedoStripBegin();
  for (int i = getTorpedoStripBegin(); i < (getTorpedoLen() + getTorpedoStripBegin()); i++) {
    leds[i]=TORPEDO_CRGB;
  }
}

void clearBoatStrip() {
  clearLEDSection(getBoatStripBegin(), getBoatStripEnd());
}

void lightBoatInitialPosition() {
  clearBoatStrip();

  boat_tail_current_pos = getBoatStripBegin();
  for (int i = getBoatStripBegin(); i < (getBoatLen() + getBoatStripBegin()); i++) {
    leds[i]=BOAT_CRGB;
  }
}

void selectTrack(int track) {
    selected_track = track;
}

// select track and updates display
// return true means track selection changed
boolean selectTrack() {
  //TODO detect selector switch states then call selectTrack(int track)
  int selectCount = 0;
  int lastSelectedTrack = selected_track;
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

  if (lastSelectedTrack == selected_track) {
    return false;
  }
  lightTorpedoInitialPosition();
  return true;
}

boolean merchantIsIlluminated = false;
boolean slowTorpedoIsIlluminated = false;
boolean startIsIlluminated = false;
boolean fireIsIlluminated = false;
long last_boat_flash = -1;
long last_torpedo_flash = -1;
long last_start_button_flash = -1;
long last_fire_button_flash = -1;

#define START_BUTTON_LED             26
#define FIRE_BUTTON_LED              27



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
  flashLED(startIsIlluminated, last_start_button_flash, START_LED_FLASH_MS_INTERVAL, START_BUTTON_LED);
}

void flashFireLED() {
  flashLED(fireIsIlluminated, last_fire_button_flash, FIRE_LED_FLASH_MS_INTERVAL, FIRE_BUTTON_LED);
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
  flashAlternateLEDs(slowTorpedoIsIlluminated, last_torpedo_flash, TORPEDO_LED_FLASH_MS_INTERVAL, 
                    TORPEDO_SLOW_BUTTON_LED, TORPEDO_FAST_BUTTON_LED);
}
                    
void flashBoatButtons() {
  flashAlternateLEDs(merchantIsIlluminated, last_boat_flash, BOAT_LED_FLASH_MS_INTERVAL, 
                    MERCHANT_VESSEL_BUTTON_LED, WARSHIP_BUTTON_LED);
//  if (millis() - last_boat_flash > BOAT_LED_FLASH_MS_INTERVAL) {  
//    if (merchantIsIlluminated) {
//      digitalWrite(MERCHANT_VESSEL_BUTTON_LED, LOW);
//      digitalWrite(WARSHIP_BUTTON_LED, HIGH);   
//      merchantIsIlluminated = false;
//    } else {
//      digitalWrite(MERCHANT_VESSEL_BUTTON_LED, HIGH);
//      digitalWrite(WARSHIP_BUTTON_LED, LOW);   
//      merchantIsIlluminated = true;
//    }
//  }  
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
  last_boat_flash = -1;
  last_torpedo_flash = -1;
  last_start_button_flash = -1;
  last_fire_button_flash = -1;
}


void updateBoatButtonLEDs() {
  switch(boat_select_state) {
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
  switch(torpedo_select_state) {
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
  //TODO detect selector switch states then call selectTrack(int track)
  int selectCount = 0;
  boat_select_states lastSelected = boat_select_state;

  if (digitalRead(MERCHANT_VESSEL_BUTTON) == LOW) {
    Serial.println("PRESSED MERCHANT_VESSEL_BUTTON");
    selectCount++;
    boat_select_state = merchant_selected;
  } 
  
  if (digitalRead(WARSHIP_BUTTON) == LOW) {
    Serial.println("PRESSED WARSHIP_BUTTON");
    selectCount++;
    boat_select_state = warship_selected;
  }

  //log error if selectedCount > 1
  if (selectCount > 1) {
    Serial.println("ERROR: More than one SHIP selected. Selected warship.");
  } 
  if (lastSelected == boat_select_state) {
    return false; 
  } 

  //update selection buttons
  updateBoatButtonLEDs();
  //display boat
  lightBoatInitialPosition();
  return true;
}


boolean selectTorpedo() {
  //TODO detect selector switch states then call selectTrack(int track)
  int selectCount = 0;
  torpedo_select_states lastSelected = torpedo_select_state;

  if (digitalRead(TORPEDO_FAST_BUTTON) == LOW) {
    Serial.println("PRESSED TORPEDO_FAST_BUTTON");
    selectCount++;
    torpedo_select_state = fast_selected;
  } 
  
  if (digitalRead(TORPEDO_SLOW_BUTTON) == LOW) {
    Serial.println("PRESSED TORPEDO_SLOW_BUTTON");
    selectCount++;
    torpedo_select_state = slow_selected;
  }

  //log error if selectedCount > 1
  if (selectCount > 1) {
    Serial.println("ERROR: More than one TORPEDO selected. Selected slow.");
  } 
  if (lastSelected == torpedo_select_state) {
    return false; 
  } 

  //update selection buttons
//  updateTorpedoButtonLEDs(); TODO
  //display boat
  lightTorpedoInitialPosition();
  updateTorpedButtonLEDs();
  return true;
}



//note this has side effect of updating spritePos ... should return spritePos instead?
boolean advanceSprite(int &spritePos, int spriteLen, CRGB spriteColor, int stripBegin, int stripEnd) {
   // check time TODO
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
    //RESET game state TODO
     leds[stripEnd] = CRGB::Black; 
    spritePos = -1; 
    //TODO play miss sequence
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
    return advanceSprite(boat_tail_current_pos, getBoatLen(), BOAT_CRGB, getBoatStripBegin(), getBoatStripEnd());
  }
  return true;
}

long last_torpedo_increment_ms = -1;

boolean advanceTorpedo() {
// check time
  if (millis() - last_torpedo_increment_ms > getTorpedoSpeed()) {
    last_torpedo_increment_ms = millis();
    // increment position
    return advanceSprite(torpedo_tail_current_pos, getTorpedoLen(), TORPEDO_CRGB, getTorpedoStripBegin(), getTorpedoStripEnd());
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
  return torpedoAtIntersection(torpedo_tail_current_pos, getTorpedoLen(), getTorpedoStripIntersect());
}

// Determine if any boat pixel is at the given boat strip intersection 
boolean boatAtIntersection(int boatStripIntersect) {
  for (int boatPixel = boat_tail_current_pos; boatPixel < boat_tail_current_pos + getBoatLen(); boatPixel++) {
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
  //TODO flash LED button
  // animate star

  Serial.println("HIT!");
  //TODO Remove:
  delay(1000);
}

void displayImpactSequence() {
  displayImpactSequence(selected_track);
}

void displayMissSequence() {
  //TODO any miss sequence?

  Serial.println("MISS!");
  //TODO Remove:
  delay(1000);

}



//TODO delete this
void fadeall() { for(int i = 0; i < PIXEL_COUNT; i++) { leds[i].nscale8(250); } }



void resetTorpedo() {
  torpedo_tail_current_pos = -1;
  clearSelectedTorpedoStrip();
}
void resetBoat() {
  boat_tail_current_pos = -1;
  clearBoatStrip();
}


void setup() {
    Serial.begin(115200);
  Serial.println("resetting");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,PIXEL_COUNT);
//  LEDS.setBrightness(84);
  LEDS.setBrightness(1);
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

//#define TORPEDO_STRIP_0_INTERSECT_POS 78
//#define BOAT_STRIP_0_INTERSECT_POS 20

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

  //TODO Clear all, not just section
  clearLEDSection(TORPEDO_STRIP_2_BEGIN_POS, TORPEDO_STRIP_2_END_POS);


  boat_select_state = boat_select_states::no_boat_selected;

//  boat_select_state = warship_selected; // TODO delete, this is only for test 
//  torpedo_select_state = fast_selected; // TODO delete, this is only for test 
//  selectTrack(5);// TODO delete, this is only for test 

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

  
//  currentGameState = DEMO_TEST;
}

//test
int torpedo_tail_current_pos0 = -1;
int torpedo_tail_current_pos1 = -1;
int torpedo_tail_current_pos2 = -1;
int torpedo_tail_current_pos3 = -1;
int torpedo_tail_current_pos4 = -1;
int torpedo_tail_current_pos5 = -1;

void demoTestAdvance() {
  //  advanceShip();
  advanceSprite(boat_tail_current_pos, getBoatLen(), BOAT_CRGB, getBoatStripBegin(), getBoatStripEnd());
  advanceSprite(torpedo_tail_current_pos0, getTorpedoLen(), TORPEDO_CRGB, getTorpedoStripBegin(0), getTorpedoStripEnd(0));
  advanceSprite(torpedo_tail_current_pos1, getTorpedoLen(), TORPEDO_CRGB, getTorpedoStripBegin(1), getTorpedoStripEnd(1));
  advanceSprite(torpedo_tail_current_pos2, getTorpedoLen(), TORPEDO_CRGB, getTorpedoStripBegin(2), getTorpedoStripEnd(2));
  advanceSprite(torpedo_tail_current_pos3, getTorpedoLen(), TORPEDO_CRGB, getTorpedoStripBegin(3), getTorpedoStripEnd(3));
  advanceSprite(torpedo_tail_current_pos4, getTorpedoLen(), TORPEDO_CRGB, getTorpedoStripBegin(4), getTorpedoStripEnd(4));
  advanceSprite(torpedo_tail_current_pos5, getTorpedoLen(), TORPEDO_CRGB, getTorpedoStripBegin(5), getTorpedoStripEnd(5));

  if (torpedoAtIntersection(torpedo_tail_current_pos0, getTorpedoLen(), getTorpedoStripIntersect(0))
      && boatAtIntersection(getBoatStripIntersect(0))) {
    Serial.println("0 HIT");  
  }

  if (torpedoAtIntersection(torpedo_tail_current_pos1, getTorpedoLen(), getTorpedoStripIntersect(1))
      && boatAtIntersection(getBoatStripIntersect(1))) {
    Serial.println("1 HIT");
  }

  if (torpedoAtIntersection(torpedo_tail_current_pos2, getTorpedoLen(), getTorpedoStripIntersect(2))
      && boatAtIntersection(getBoatStripIntersect(2))) {
    Serial.println("2 HIT");
  }

  if (torpedoAtIntersection(torpedo_tail_current_pos3, getTorpedoLen(), getTorpedoStripIntersect(3))
      && boatAtIntersection(getBoatStripIntersect(3))) {
    Serial.println("3 HIT");
  }

  if (torpedoAtIntersection(torpedo_tail_current_pos4, getTorpedoLen(), getTorpedoStripIntersect(4))
      && boatAtIntersection(getBoatStripIntersect(4))) {
    Serial.println("4 HIT");
  }

  if (torpedoAtIntersection(torpedo_tail_current_pos5, getTorpedoLen(), getTorpedoStripIntersect(5))
      && boatAtIntersection(getBoatStripIntersect(5))) {
    Serial.println("5 HIT");
  }

}


void loop() {

//  advanceShip();

switch(currentGameState) {
    case DEMO_TEST : 
      Serial.println("Demo test");
      digitalWrite(MERCHANT_VESSEL_BUTTON_LED, HIGH);
      delay(200);
      digitalWrite(WARSHIP_BUTTON_LED, HIGH);
      delay(200);
      digitalWrite(TORPEDO_FAST_BUTTON_LED, HIGH);
      delay(200);
      digitalWrite(TORPEDO_SLOW_BUTTON_LED, HIGH);
      delay(200);
      digitalWrite(START_BUTTON_LED, HIGH);
      delay(200);
      digitalWrite(FIRE_BUTTON_LED, HIGH);
      delay(200);



      digitalWrite(MERCHANT_VESSEL_BUTTON_LED, LOW);
      delay(50);
      digitalWrite(WARSHIP_BUTTON_LED, LOW);
      delay(50);
      digitalWrite(TORPEDO_FAST_BUTTON_LED, LOW);
      delay(50);
      digitalWrite(TORPEDO_SLOW_BUTTON_LED, LOW);
      delay(50);
      digitalWrite(START_BUTTON_LED, LOW);
      delay(50);
      digitalWrite(FIRE_BUTTON_LED, LOW);
      delay(50);

           // demoTestAdvance();
//            flashFireLED();         
//                        flashStartLED();              
//     
//               flashBoatButtons();
//              flashTorpedoButtons();

             break;       // and exits the switch
    case WAITING_FOR_BOAT_AND_TORPEDO_SELECTION :  ;
          // CLEAR LEDs
          // TODO 
          
          // Set Boat to beginning
          // TODO 
          
          Serial.println("WAITING_FOR_BOAT_AND_TORPEDO_SELECTION");
           // poll boat select
           if (!selectBoat()) {
              // flash boat select
              flashBoatButtons();
           } else {
              currentGameState = WAITING_FOR_TORPEDO_SELECTION_ONLY;
              break;
           }

           // poll torpedo select
           if (!selectTorpedo()) {
              // flash torpedo select
              flashTorpedoButtons();
           } else {
              currentGameState = WAITING_FOR_BOAT_SELECTION_ONLY;
              break;
           }




            //check for torpedo track changes
            selectTrack();
             
             break;
    case WAITING_FOR_BOAT_SELECTION_ONLY :  ;
           Serial.println("WAITING_FOR_BOAT_SELECTION_ONLY");
            
            
            
            // poll torpedo select
            selectTorpedo();
            
           if (!selectBoat()) {
              // flash boat select
              flashBoatButtons();
           } else {
              currentGameState = WAITING_FOR_START;
              break;
           }

            //check for torpedo track changes
            selectTrack();

            // check for timeout
            
             break;
    case WAITING_FOR_TORPEDO_SELECTION_ONLY :  ;
               Serial.println("WAITING_FOR_TORPEDO_SELECTION_ONLY");
            // check for boat changes
            selectBoat();
            
           // poll torpedo select
           if (!selectTorpedo()) {
              // flash torpedo select
              flashTorpedoButtons();
           } else {
              currentGameState = WAITING_FOR_START;
              break;
           }


            //check for torpedo track changes
            selectTrack();
            
            //check for timeout
             break;
    case WAITING_FOR_START :  ;
                   Serial.println("WAITING_FOR_START");


            
            //check start button
            if (digitalRead(START_BUTTON) == LOW) {
              currentGameState = WAITING_FOR_TORPEDO_FIRE;
              digitalWrite(START_BUTTON_LED, LOW);
              break;
            } else {
              //flash start button
              flashStartLED();
            }
            // check for boat changes
            selectBoat();
            
            //change torpedo
            selectTorpedo();
            
            //check for torpedo track changes
            selectTrack();

            // check for timeout
             break;
    case WAITING_FOR_TORPEDO_FIRE :  
            Serial.println("WAITING_FOR_TORPEDO_FIRE");
            // poll fire button
            if (digitalRead(FIRE_BUTTON) == LOW) {
              currentGameState = WAITING_FOR_TORPEDO_END;
            } else {
              //flash Fire button
              flashFireLED();              
            }

            //check for torpedo track changes
            selectTrack();

            // check timeout
            // TODO
            
            // advance boat
            if (!advanceShip()) {
              // todo what to do if boat completes w/o fire? Reset or scroll boat again?
              currentGameState = RESET_GAME; 
            }
             break;
    case WAITING_FOR_TORPEDO_END :  ;
              Serial.println("WAITING_FOR_TORPEDO_END");

            // advance boat 
            if (!advanceShip()) {
              currentGameState = BOAT_COMPLETE_WAITING_FOR_TORPEDO; // TODO let Torpedo travel state?
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
             currentGameState = RESET_GAME; //todo any clean up state needed?

             break;
    case TORPEDO_MISS :  ;
            // play miss sequence
            displayMissSequence(); 
            currentGameState = TORPEDO_MISS_WAITING_FOR_BOAT; 
            
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
        boat_select_state = no_boat_selected;
        torpedo_select_state = no_torpedo_selected;
        clearAllTorpedoStrips();
        clearBoatStrip();
        resetButtonLEDs();
        currentGameState = WAITING_FOR_BOAT_AND_TORPEDO_SELECTION;
        break;

}


  

    FastLED.show(); 
//TODO kill (for testing)
 // delay(200);

  static uint8_t hue = 0;



}

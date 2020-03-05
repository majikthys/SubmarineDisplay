#include <FastLED.h>



#define PIXEL_COUNT 200

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
//#define DATA_PIN 7
#define DATA_PIN 53
#define CLOCK_PIN 13

#define MERCHANT_VESSEL_BUTTON   1
#define WARSHIP_BUTTON           2
#define TORPEDO_FAST_BUTTON      3
#define TORPEDO_SLOW_BUTTON      4
#define START_BUTTON             5
#define FIRE_BUTTON              6

#define MERCHANT_VESSEL_BUTTON_LED   8
#define WARSHIP_BUTTON_LED           9
#define TORPEDO_FAST_BUTTON_LED      10
#define TORPEDO_SLOW_BUTTON_LED      11
#define START_BUTTON_LED             12
#define FIRE_BUTTON_LED              13
#define LED_STRIP_PIN                7

#define TORPEDO_0_SELECT             14
#define TORPEDO_1_SELECT             15
#define TORPEDO_2_SELECT             16
#define TORPEDO_3_SELECT             17
#define TORPEDO_4_SELECT             18
#define TORPEDO_5_SELECT             19


#define WARSHIP_LEN                  4
#define MERCHANT_SHIP_LEN            4
#define WARSHIP_SPEED_MS_INTERVAL    200
#define MARCHANT_SPEED_MS_INTERVAL   100
#define SLOW_TORPEDO_LEN             2
#define FAST_TORPEDO_LEN             2
#define FAST_TORPEDO_SPEED_MS_INTER  200
#define SLOW_TORPEDO_SPEED_MS_INTER  100

#define SHIP_COLOR 255, 0, 0

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
  getTorpedoStripBegin(selected_track);
}

int getTorpedoStripIntersect() {
  getTorpedoStripIntersect(selected_track); 
}

int getBoatStripIntersect() {
  getBoatStripIntersect(selected_track);
}

int getTorpedoStripEnd() {
  getTorpedoStripEnd(selected_track);
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

boolean torpedo_launched = false;
void launchTorpedo() {
  torpedo_launched = true;
  
// TODO set selected_track
}

int torpedo_tail_current_pos = -1;

void lightTorpedo() {
  if (torpedo_tail_current_pos > getTorpedoStripEnd()) {
    //RESET torpedo fire state 
    torpedo_launched = false;
    return;
  }
  for (int torpedoPixel = torpedo_tail_current_pos; torpedoPixel < torpedo_tail_current_pos + getTorpedoLen(); torpedoPixel++) {
    if (getTorpedoStripBegin() <= torpedoPixel <= getTorpedoStripEnd()) {
        // TODO light torpedoPixel
    }
  }
}

int boat_tail_current_pos = -1;

void lightBoat() {
  if (boat_tail_current_pos > getBoatStripEnd()) {
    //TODO RESET game state
    boat_tail_current_pos = -1; 
    //TODO play miss sequence
    return;
  }
  for (int boatPixel = boat_tail_current_pos; boatPixel < boat_tail_current_pos + getBoatLen(); boatPixel++) {
    if (getBoatStripBegin() <= boatPixel <= getBoatStripEnd()) {
        // TODO light torpedoPixel
    }
  }
}
void selectTrack(int track) {
  if (!torpedo_launched) {
    selected_track = track;
    torpedo_tail_current_pos = getTorpedoStripBegin(track);
  }
}

void selectTrack() {
  //TODO detect selector switch states then call selectTrack(int track)
  int selectCount = 0;
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

  //TODO log error if selectedCount > 1
  
}


long last_torpedo_increment_ms = -1;
boolean advanceTorpedo() {
// check time
  // increment position
  // light correct LEDs (deal w/ LED run on and run off)
}

long last_ship_increment_ms = -1;

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
    return;
  }

  if ((spritePos - 1) >= stripBegin) {
    leds[spritePos -1] = CRGB::Black;  
    
  }
  
  if (spritePos + spriteLen - 1 > stripEnd) {
    return;
  } else {
    leds[spritePos + spriteLen - 1] = spriteColor;
  }
 
}


boolean advanceShip() {
  // check time TODO
  // increment position
  if (boat_tail_current_pos == -1) {
    boat_tail_current_pos = getBoatStripBegin();
    for (int i = 0; i < getBoatLen() ; i++) {
      leds[boat_tail_current_pos + i] = CRGB::Blue;
    }
    
  } else {
    boat_tail_current_pos++;  
  }
  
  if (boat_tail_current_pos > getBoatStripEnd()) {
    //RESET game state TODO
     leds[getBoatStripEnd()] = CRGB::Black; 
    boat_tail_current_pos = -1; 
    //TODO play miss sequence
    return;
  }

  if ((boat_tail_current_pos - 1) >= getBoatStripBegin()) {
    leds[boat_tail_current_pos -1] = CRGB::Black;  
    
  }
  
  if (boat_tail_current_pos + getBoatLen() - 1 > getBoatStripEnd()) {
    return;
  } else {
    leds[boat_tail_current_pos + getBoatLen() - 1] = CRGB::Blue;
  }

  
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
}

void displayImpactSequence() {
  displayImpactSequence(selected_track);
}

void displayMissSequence() {

}

void clearAllRGB() {
  
}

boolean inGameMode() {
  //TODO
  clearAllRGB();
  selectTrack();
  lightTorpedo();
  advanceTorpedo();
  if (!advanceShip()) {
    //TODO reset game
  }
  if (isCollision()) {
    displayImpactSequence();
    //TODO reset game
  }
}

//TODO delete this
void fadeall() { for(int i = 0; i < PIXEL_COUNT; i++) { leds[i].nscale8(250); } }


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

  boat_select_state = warship_selected; // TODO delete, this is only for test 
  torpedo_select_state = fast_selected; // TODO delete, this is only for test 


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

}

//test
int torpedo_tail_current_pos0 = -1;
int torpedo_tail_current_pos1 = -1;
int torpedo_tail_current_pos2 = -1;
int torpedo_tail_current_pos3 = -1;
int torpedo_tail_current_pos4 = -1;
int torpedo_tail_current_pos5 = -1;

void loop() {

//  advanceShip();
  advanceSprite(boat_tail_current_pos, getBoatLen(), CRGB::Blue, getBoatStripBegin(), getBoatStripEnd());
  advanceSprite(torpedo_tail_current_pos0, getTorpedoLen(), CRGB::Red, getTorpedoStripBegin(0), getTorpedoStripEnd(0));
  advanceSprite(torpedo_tail_current_pos1, getTorpedoLen(), CRGB::Red, getTorpedoStripBegin(1), getTorpedoStripEnd(1));
  advanceSprite(torpedo_tail_current_pos2, getTorpedoLen(), CRGB::Red, getTorpedoStripBegin(2), getTorpedoStripEnd(2));
  advanceSprite(torpedo_tail_current_pos3, getTorpedoLen(), CRGB::Red, getTorpedoStripBegin(3), getTorpedoStripEnd(3));
  advanceSprite(torpedo_tail_current_pos4, getTorpedoLen(), CRGB::Red, getTorpedoStripBegin(4), getTorpedoStripEnd(4));
  advanceSprite(torpedo_tail_current_pos5, getTorpedoLen(), CRGB::Red, getTorpedoStripBegin(5), getTorpedoStripEnd(5));

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


    FastLED.show(); 

  delay(200);

  static uint8_t hue = 0;




//clearAllRGB();
//selectTrack();
//lightTorpedo();

}

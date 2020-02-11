


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


#define PIXEL_COUNT              90?
#define BOAT_STRIP_BEGIN_POS     0
#define BOAT_STRIP_END_POS       60
#define TORPEDO_STRIP_0_BEGIN_POS 61
#define TORPEDO_STRIP_0_INTERSECT_POS 78
#define BOAT_STRIP_0_INTERSECT_POS 20
#define TORPEDO_STRIP_0_END_POS 80
#define TORPEDO_STRIP_1_BEGIN_POS 81
#define TORPEDO_STRIP_1_INTERSECT_POS 98
#define BOAT_STRIP_1_INTERSECT_POS 25
#define TORPEDO_STRIP_1_END_POS 90
#define TORPEDO_STRIP_2_BEGIN_POS 91
#define TORPEDO_STRIP_2_INTERSECT_POS 108
#define BOAT_STRIP_2_INTERSECT_POS 35
#define TORPEDO_STRIP_2_END_POS 110
#define TORPEDO_STRIP_3_BEGIN_POS 111
#define TORPEDO_STRIP_3_INTERSECT_POS 128
#define BOAT_STRIP_3_INTERSECT_POS 40
#define TORPEDO_STRIP_3_END_POS 130
#define TORPEDO_STRIP_4_BEGIN_POS 131
#define TORPEDO_STRIP_4_INTERSECT_POS 148
#define BOAT_STRIP_4_INTERSECT_POS 45
#define TORPEDO_STRIP_4_END_POS 150
#define TORPEDO_STRIP_5_BEGIN_POS 151
#define TORPEDO_STRIP_5_INTERSECT_POS 168
#define BOAT_STRIP_5_INTERSECT_POS 50
#define TORPEDO_STRIP_5_END_POS 170

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
  selectTrack(1);
}


long last_torpedo_increment_ms = -1;
boolean advanceTorpedo() {
// check time
  // increment position
  // light correct LEDs (deal w/ LED run on and run off)
}

long last_ship_increment_ms = -1;
boolean advanceShip() {
// check time
  // increment position
  // light correct LEDs (deal w/ LED run on and run off)
}

// Determine if any torpedo pixel intersection of the current torpedo strip
boolean torpedoAtIntersection() {
  for (int torpedoPixel = torpedo_tail_current_pos; torpedoPixel < torpedo_tail_current_pos + getTorpedoLen(); torpedoPixel++) {
    if (torpedoPixel == getTorpedoStripIntersect()) {
        return true;
    }
  }
  return false;
}

// Determine if any boat pixel is at the boat strip intersection of the current torpedo track
boolean boatAtIntersection() {
  for (int boatPixel = boat_tail_current_pos; boatPixel < boat_tail_current_pos + getBoatLen(); boatPixel++) {
    if (boatPixel == getBoatStripIntersect()) {
        return true;
    }
  }
  return false;
}
boolean isCollision() {
  return (torpedoAtIntersection() && boatAtIntersection());
}

void displayImpactSequence() {

}

boolean inGameMode() {
  //TODO
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



void setup() {
  // put your setup code here, to run once:

}

void loop() {

selectTrack();
lightTorpedo();

  
}

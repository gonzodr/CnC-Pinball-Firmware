////////////////////////////////////
// Cheech & Chong Pinball
// Firmware version 4
////////////////////////////////////
// Catalog:
// Ballhandler - 640
// Timers - 1000
// Light handling - 1090
// Init mode and player select - 1240
// Init table - 1425
// Data send - 1505
// Score - 1530
// Initlights - 1550
// Flippers - 1630
// Slingshots - 1670
// CnC - 1765
// Loop - 1990
// Weed - 2050
// Fish Tank - 2360
// DAVE - 2520
// Gate - 2680
// POPs - 2890
// Bonus X - 3060
// Weed Spinner - 3220
// UFO - 3470
// Cheech and Chong Targets - 3740
// Collectives - 4020
// Gift - 4100
// Bridge Low - 4150
// Bridge High - 4335
// Add Player - 4520
// Hurry Up mode - 4550
// Tilt - 4610

#include <FastLED.h>          // For the leds
#include <Wire.h>             // For send data to Servo UNO
#include <wavTrigger.h>       // Serial1-en fut (TX1 = 18-as pin)! A wavTrigger.h library-ban __WT_USE_SERIAL1__ legyen aktiv!

//////////////////////////////////////////////////////////////////////
// !!! PROBAPADI SZIMULATOR MOD (f_sim_mode.ino) !!!
// Probapadi teszthez vedd ki a kommentet - ELES GEPRE SOHA ne keruljon
// fel bekapcsolva! (sim modban a golyo-infrak szimulalt erteket adnak,
// a valodi erzekeloket a firmware NEM olvassa!)
// A main agon MINDIG kikommentelve tartjuk, mert a gepbeli F7-es
// firmware update automatikusan a main-t huzza es flasheli!
//#define SIM_MODE
#ifdef SIM_MODE
int simForceLottery = 0; // a kovetkezo UFO-lotto kenyszeritett erteke (7 = SpaceCoke)
#endif
//////////////////////////////////////////////////////////////////////

#define NUM_LEDS 115
#define DATA_PIN 3
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB
#define UPDATES_PER_SECOND 100
CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

wavTrigger wTrig;

String incomeMsg = "";



/////////////////////////////////////////////////////////////////
/// Variables for the gate
// Pins
int gateSwitch1 = 45; // gate 1
int gateSwitch2 = 44; // gate 2
int gateSwitch3 = 40; // gate 3
// Integers
int bonusx = 0;
// Booleans
boolean gatesw1 = LOW;
boolean gatesw2 = LOW;
boolean gatesw3 = LOW;
boolean gateamb1 = LOW;
boolean gateamb2 = LOW;
boolean gateamb3 = LOW;
boolean gateoffsw = LOW;
boolean gatetimesw = LOW;
boolean gsrtimesw = LOW;
boolean gsltimesw = LOW;
    // Bonus multiplier
boolean bonusx1sw = LOW;
boolean bonusx2sw = LOW;
boolean bonusx3sw = LOW;
boolean bonusx4sw = LOW;
// Timers
unsigned long gateambtimer = 0;
unsigned long gateofftimer = 0;
unsigned long gatetimer = 0;
// Arrays
int gatearr[] = { 0, 0, 0, 0, 0 };
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for the POPs
// Pins
int pop1Coil = 33; // output pop1
int pop2Coil = 36; // output pop 2
int pop3Coil = 32; // output pop3
int pop1Switch = 34; // input pop 1
int pop2Switch = 35; // input pop 2
int pop3Switch = 39; // input pop 3
// Booleans
boolean pop1LogicBool = LOW;
boolean pop2LogicBool = LOW;
boolean pop3LogicBool = LOW;
boolean popsw1 = LOW;
boolean popsw2 = LOW;
boolean popsw3 = LOW;
// Timers
unsigned long poptimer1 = 0;
unsigned long poptimer2 = 0;
unsigned long poptimer3 = 0;
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
/// Variables for the Bridges
// Pins
int bridgeLowSwitch = 53; // Bridgelow
int bridgeHighSwitch = 52; // BridgeHigh
// Booleans
boolean BridgeLowEffbool = LOW;
boolean BrdgHighActive = 0;
boolean BrdgHighSw = 0;
boolean BrdgLowActive = 0;
boolean BrdgLowSw = 0;
// Integers
int BridgeLowEffcounter = 0;
int comboCounter = 0;
// Timers
unsigned long comboTimerH = 0;
unsigned long comboTimerL = 0;
unsigned long BrdgLowT = 0;
unsigned long BrdgHighT = 0;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for the Fish tank
// Pins
int fishTankSwitch1 = 16; // Fish
int fishTankSwitch2 = 17; // tank
// Booleans
boolean fishoff = LOW;
// Integers
int fishTankLightState1 = 0;
int fishTankLightState2 = 0;
int beerCollect = 0;
// Timers
unsigned long fishtimer = 0;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for CnC
// Pins
int cncLetterC1st = 14; // Letter 1st C
int cncLetterN = 15; // Letter n
int cncLetterC3rd = 25; // Letter 3rd C
// Booleans
boolean cncoff = LOW;
boolean cnchurryswitch1 = LOW;
boolean cnchurryswitch2 = LOW;
boolean cnchurryswitch3 = LOW;
// Integers
int cncswitch1 = 0;
int cncswitch2 = 0;
int cncswitch3 = 0;
// Timers
unsigned long cnctimer = 0;
unsigned long cnchurryTimer1 = 0;
unsigned long cnchurryTimer2 = 0;
unsigned long cnchurryTimer3 = 0;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for Dave
// Pins
int daveLaneSwitchA = 50; // A (18-rol atkotve! 18/19 = Serial1 a wavTriggernek)
                          // NEM a 42-re: azon a (kodban nem hasznalt, de
                          // BEKOTOTT) Cheech Alley kapcsolo vezeteke ul!
int daveLaneSwitchD = 49; // D (19-rol atkotve!)
int daveLaneSwitchV = 22; // V
int daveLaneSwitchE = 23; // E
// Booleans
boolean daveoff = LOW;
boolean slltimesw = LOW;
boolean slrtimesw = LOW;
// Integers
// Timers
unsigned long davetimer = 0;
unsigned long slltimer = 0;
unsigned long slrtimer = 0;
// Arrays
int davearr[] = { 0, 0, 0, 0, 0, 0 };
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for Slingshots
// Pins
int rightSlingshotCoil = 29; // Right Slingshot
int leftSlingshotCoil = 28; // Left Slingshot
int leftSlingshotSwitch = 31; // Left Slingshot
int rightSlingshotSwitch = 4; // Right Slingshot
// Booleans
boolean slingl = LOW;
boolean slingr = LOW;
// Timers
unsigned long slingrtimer = 0;
unsigned long slingltimer = 0;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for Flipper Bats
// Pins
int leftFlipperBat = 41; // Left flipper
int rightFlipperBat = 24; // Right flipper
int rightflipperButton = 26; // Right flipper button
int leftFlipperButton = 27; // Left flipper button
// Booleans
boolean flipl = LOW;
boolean flipr = LOW;
// Timers
unsigned long flipltimer = 0;
unsigned long fliprtimer = 0;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for Spinner
// Pins
int spinnerSwitch = 38; // get High spinner
int a8 = 2;
// Integers
int cigar = 0;
// Booleans
boolean weedspsw = LOW;
// Integers
int spinnersw = 0;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for Weedtable
// Pins

// Arrays
int weedmeter[] = { 180, 180, 180, 180, 180 }; // index 1..4 = players, [0] unused


/////////////////////////////////////////////////////////////////
/// Variables for Balltrough
// Pins
int ballTroughCoil = 5; // ball trough
int a1 = 1;
int a2 = 2;
int a3 = 7;
int a4 = 4;
int a5 = 5;
int a6 = 6;
int a7 = 3;
int a10 = 10;
// Booleans
boolean ballsaversw = HIGH; // Turn on the ballsaver
boolean BallReadState = LOW;
boolean ballHandlerSkip = 0;
boolean shoot = LOW;
boolean aftermulti = LOW;
boolean startmus = LOW; // Start game music when ball ejected
boolean sidelaneBallsaverSw = LOW;
boolean maxBallSw = LOW; // SpaceCoke: az utolso tarolt golyo keslelteto kapcsoloja
// Integers
int miv1 = 0;
int miv2 = 0;
int miv3 = 0;
int miv4 = 0;
int miv5 = 0;
int ball1 = 0;
int ball2 = 0;
int ball3 = 0;
int ball4 = 0;
int ball5 = 0;
int ballsavetime = 15000;
int extraball = 0;
int multiball = 0;
int ballPresent1 = 0;
int ballPresent2 = 0;
int ballPresent3 = 0;
int ballPresent4 = 0;
int ballPresent5 = 0;
int shootfail = 0;
int BIS;
int BIP;
int player = 1;       // Player
int ball = 1;         // Actual Ball number
int numofplayers = 1; // Number of players
// Timers
unsigned long shoottimer = 0;
unsigned long shoottimer2 = 0;
unsigned long ballsavetimer = 0;
unsigned long ballHandlerSkipTimer = 0;
unsigned long maxBallSwTimer = 0;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for Cheech and Chong 
// Pins
int chongSwitch = 7; // Chong Trigger
int cheechSwitch = 12; // Cheech Trigger
// Boolean
boolean chongLightActiveSw = LOW;
boolean cheechLightActiveSw = LOW;
boolean chongoffsw = LOW;
boolean cheechoffsw = LOW;
boolean chongactive = LOW;
boolean CollectSw = 0;
// Integers
int cheechspeech = 0;
int chongspeech = 0;
// Timers
unsigned long cheechoffswtimer = 0;
unsigned long chongoffswtimer = 0;
// Arrays
int cheechCollectives[] = { 0, 0, 0, 0, 0 };
int chongCollectives[] = { 0, 0, 0, 0, 0 };
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for Shooterlane 
// Pins
int shooterLaneSwitch = 13; // shooterlane switch
int shooterlaneCoil = 6; // shooterlane kicker
int ballShooterButton = 51; // ballShooterSwitch
// Booleans
boolean shootfailchk = 0;
boolean AutoKick = LOW;
boolean kick = LOW;
boolean firstplay = HIGH;
// Timers
unsigned long kicktimer = 0;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for Ufo
// Pins
int ufoCoil = 37; // output ufo
// Booleans
boolean ufosw = 0;
boolean ufoInactivesw = 0;
// Integers
int lottery = 0;
int ufoshoot = 0;
int ufoanalog = 0;
int ufoMinus = 0; // pontlopasnal (lottery 8): a kirabolt jatekos sorszama
// Timers
unsigned long ufoInactiveTimer = 0;
unsigned long ufoshoottimer = 0;
unsigned long ufoshoottimer2 = 0;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for Weed
// Pins
int d8 = 8;
int d9 = 9;
int d10 = 10;
int d11 = 11;
// Integers
int weedswitch1 = 0;
int weedswitch2 = 0;
int weedswitch3 = 0;
int weedswitch4 = 0;
// Booleans
boolean weedoff = LOW;
boolean weedmultibool = LOW;
boolean weedblastbool = LOW;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for Loop
// Pins
int loopSwitchTop = 30; // Felső loop
int loopSwitchSide = 43; // loop alsó
// Booleans
boolean looplightbool = LOW;
boolean loopsw = LOW;
// Timers
unsigned long looptimer = 0;
unsigned long loopswt = 0;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/// Variables for Light Effect
// Shoot effect - ID=1
const uint8_t EffectID1[] PROGMEM = { 0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0};
// Weedmultiball effect - ID=2
const uint8_t EffectID2[] PROGMEM = { 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,
                    1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
// Weedblast effect - ID=3
const uint8_t EffectID3[] PROGMEM = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,6,0,0,0,0,0,0,0,0,0,0,0,6,6,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,7,7,7,7,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,9,0,9,0,9,0,9,0,0,0,0,0,0,0,0,9,
                    2,0,0,2,0,0,2,2,0,2,2,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,2,0,2,2,2,2,2,0,0,0,0,
                    0,0,0,0,8,8,0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
// Bridge effect - ID=5
const uint8_t EffectID5[] PROGMEM = { 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,
                    1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


/////////////////////////////////////////////////////////////////
/// Misc


// int cheechAlleySwitch = "D42"; // cheech alley
int startButton = 47; // start
// int d49 = 49; // unused
int spsound = 0;
int giftsw = 0;
int intmon = 1;
int heysoundcounter = 0;
int tiltcounter =0;
int shootLightCycle;
int colorcode;
int offset = 0;
int effectID = 0;
int weedmulticounter = 0;
int weedblastcounter = 0;
int looplightcounter = 0;
int feny[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int hurryhitcounter = 0;
int fasz = 0;         // Led start at fasz
int randGift = 0;
int resetTimer = 0;

boolean hurryhitbool = 0;
boolean shooteffbool = LOW;
boolean blinky = LOW;
boolean ledState = LOW;
boolean cigarState = LOW;
boolean initlight = HIGH;
boolean Inittable = LOW;
boolean sendDsw = LOW;
boolean sltimesw = LOW;
boolean weedtableindicator = LOW;
boolean effect = LOW;
boolean effectState = LOW;
boolean multiloopsw = LOW;
boolean addPlayersw = LOW;
boolean lfHscSw = 0;
boolean rfHscSw = 0;
boolean shHscSw = 0;
boolean stHscSw = 0;
boolean hurryUp = LOW;
boolean hurryUpState = LOW;
// Player select mod (intmon == 3)
boolean selArmSw = LOW;
boolean selShootSw = LOW;
unsigned long selShootTimer = 0;
unsigned long selTimeoutTimer = 0;
boolean weedhurryswitch1 = 0;
boolean weedhurryswitch2 = 0;
boolean weedhurryswitch3 = 0;
boolean weedhurryswitch4 = 0;
boolean tiltLogicSW = LOW;


unsigned long weedhurrytimer1 = 0;
unsigned long weedhurrytimer2 = 0;
unsigned long weedhurrytimer3 = 0;
unsigned long weedhurrytimer4 = 0;
unsigned long hurryUpTimer = 0;
unsigned long stHscTimer = 0;
unsigned long lfHscTimer = 0;
unsigned long rfHscTimer = 0;
unsigned long shHscTimer = 0;
unsigned long addPlayerTimer = 0;
unsigned long CollectTimer = 0;
unsigned long bonus = 0;
unsigned long score[] = {0, 0, 0, 0, 0}; // index 1..4 = players, [0] unused
unsigned long weedm[] = {0, 0, 0, 0, 0};
unsigned long previousMillis = 0;
unsigned long blinkypreviousMillis = 0;
unsigned long cigarpreviousMillis = 0;
unsigned long effectpreviousMillis = 0;
unsigned long BallpreviousMillis = 0;
unsigned long weedtimer = 0;
unsigned long sltimer = 0;
unsigned long gsltimer = 0;
unsigned long gsrtimer = 0;
unsigned long bonusxtimer1 = 0;
unsigned long bonusxtimer2 = 0;
unsigned long bonusxtimer3 = 0;
unsigned long bonusxtimer4 = 0;
unsigned long sendDtimer = 0;
unsigned long weedswitchtimer = 0;
unsigned long cigartime = 0;
unsigned long weedtableindicatortimer = 0;
unsigned long hurryUppreviousMillis = 0;
unsigned long heysoundtimer = 0;
unsigned long tilttimer = 0;
unsigned long tiltLogicTimer = 0;
const long interval = 100; // Ledstate blinktime intervall


void setup() {

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  currentPalette = RainbowColors_p;
  //  currentPalette = SetupBlackAndWhiteStripedPalette();
  currentBlending = NOBLEND;

  Wire.begin();
  Serial.begin(115200); // a Python GUI-hoz igazitva (a regi 9800 a 9600 elgepelese volt)

  pinMode(PIN_A0, INPUT); // Ball 1
  pinMode(PIN_A1, INPUT); // Ball 2
  pinMode(PIN_A2, INPUT); // Ball 3
  pinMode(PIN_A3, INPUT); // Ball 4
  pinMode(PIN_A4, INPUT); // Ball 5
  pinMode(PIN_A5, INPUT); // UFO
  pinMode(a8, OUTPUT);
  pinMode(a10, INPUT);
  pinMode(PIN_A12, INPUT_PULLUP); // Tilt SW
  pinMode(PIN_A13, OUTPUT); // Reset
  pinMode(rightSlingshotSwitch, INPUT_PULLUP);
  pinMode(ballTroughCoil, OUTPUT);
  pinMode(shooterlaneCoil, OUTPUT);
  pinMode(chongSwitch, INPUT_PULLUP);
  pinMode(d8, INPUT_PULLUP);
  pinMode(d9, INPUT_PULLUP);
  pinMode(d10, INPUT_PULLUP);
  pinMode(d11, INPUT_PULLUP);
  pinMode(cheechSwitch, INPUT_PULLUP);
  pinMode(shooterLaneSwitch, INPUT_PULLUP);
  pinMode(cncLetterC1st, INPUT_PULLUP);
  pinMode(cncLetterN, INPUT_PULLUP);
  pinMode(fishTankSwitch1, INPUT_PULLUP);
  pinMode(fishTankSwitch2, INPUT_PULLUP);
  pinMode(daveLaneSwitchA, INPUT_PULLUP);
  pinMode(daveLaneSwitchD, INPUT_PULLUP);
  // Pin 20 and 21 is reserved for Wire
  pinMode(daveLaneSwitchV, INPUT_PULLUP);
  pinMode(daveLaneSwitchE, INPUT_PULLUP);
  pinMode(rightFlipperBat, OUTPUT);
  pinMode(cncLetterC3rd, INPUT_PULLUP);
  pinMode(rightflipperButton, INPUT_PULLUP);
  pinMode(leftFlipperButton, INPUT_PULLUP);
  pinMode(rightSlingshotCoil, OUTPUT);
  pinMode(leftSlingshotCoil, OUTPUT);
  pinMode(loopSwitchTop, INPUT_PULLUP);
  pinMode(rightSlingshotSwitch, INPUT_PULLUP);
  pinMode(leftSlingshotSwitch, INPUT_PULLUP);
  pinMode(pop3Coil, OUTPUT);
  pinMode(pop1Coil, OUTPUT);
  pinMode(pop1Switch, INPUT_PULLUP);
  pinMode(pop2Switch, INPUT_PULLUP);
  pinMode(pop2Coil, OUTPUT);
  pinMode(ufoCoil, OUTPUT);
  pinMode(spinnerSwitch, INPUT_PULLUP);
  pinMode(pop3Switch, INPUT_PULLUP);
  pinMode(gateSwitch3, INPUT_PULLUP);
  pinMode(leftFlipperBat, OUTPUT);
  //pinMode(cheechAlleySwitch, INPUT_PULLUP);
  pinMode(loopSwitchSide, INPUT_PULLUP);
  pinMode(gateSwitch2, INPUT_PULLUP);
  pinMode(gateSwitch1, INPUT_PULLUP);
  // pin 18 (TX1) es 19 (RX1) a wavTrigger Serial1-e, ne hasznald masra!
  // pin 46 es 48 felszabadult (regen AltSoftSerial volt rajtuk)
  pinMode(startButton, INPUT_PULLUP);
  //pinMode(d49, INPUT_PULLUP);
  pinMode(bridgeLowSwitch, INPUT_PULLUP);
  pinMode(ballShooterButton, INPUT_PULLUP);
  pinMode(bridgeHighSwitch, INPUT_PULLUP);
  //pinMode(d53, INPUT_PULLUP);

  //// Wav Trigger Init
  wTrig.start();
  delay(1000);
  wTrig.stopAllTracks();
  wTrig.masterGain(0);
  wTrig.trackPlayPoly(90);

  
  CoilGuardInit(); // tekercsvedelem (e_coil_guard.ino) - a pinMode-ok UTAN kell!
  SimInit();       // probapadi szimulator (f_sim_mode.ino) - eles buildben ures

  heysoundtimer = millis();
  digitalWrite(PIN_A13, HIGH);
}

void loop() {
  CoilGuardReport(); // jelzi a sorosra, ha a tekercsvedelem kozbelepett
  SimPoll();         // probapadi szimulator lepteto - eles buildben ures

  if (intmon != 0) { // 1 = attract, 2 = hiscore/nevbevitel, 3 = player select
    intmMode();
  }


  if (intmon == LOW) {
    Multiball();
    Loopshoot();
    Ballhandler();
    Weed();
    Pops();
    Weedspinner();
    Gate();
    Initlights();
    Blinktimer();
    Left_Flipper();
    Right_Flipper();
    Right_Slingshot();
    Left_Slingshot();
    CnC();
    Fishtank();
    Chong_switch();
    Cheech_switch();
    UFOO();
    Dave_switch();
    BonusXLed();
    SendData();
    Ballsave();
    Collectives();
    BridgeLow();
    BridgeHigh();
    HurryUp();
    Tilt();
    RunLightEffect(); // fenyeffekt-motor (d_light_effects.ino), effect == HIGH eseten fut
  }

  if (effect == LOW) {

    static int startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    FillLEDsFromPaletteColors( startIndex);
  }
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);



}

///////////////////////////////////
///////////////////////////////////
//// BALLHANDLER
///////////////////////////////////
///////////////////////////////////

void Ballhandler() {
   if(shoot==0){
    MIV(LOW); // Measure how many ball in the stack
   }

    /////////////////
    //// Firstball
    ////////////////

    if (firstplay == HIGH && shoot == 0 && BIS == 5) {
      if (BIS + BIP > 5) {
        BIS = BIS -1;
        shoottimer = millis();
        shoottimer2 = millis();
        shoot = 1;
        wTrig.trackPlayPoly(17);
      }
    }


    /////////////////
    //// Ballsave
    ////////////////

    if (ballsaversw == HIGH && shoot == 0) {
      MIV(HIGH);
      if (BIP != 5) {
        if (BIS + BIP > 5) {
          BIS = BIS - 1;
          shoottimer = millis();
          shoottimer2 = millis();
          shoot = 1;
          if (firstplay == LOW) {
            startmus = LOW;
          }
        }
        if (sidelaneBallsaverSw == HIGH && BIS != 0) {
          sidelaneBallsaverSw = LOW;
          BIS = BIS - 1;
          shoottimer = millis();
          shoottimer2 = millis();
          shoot = 1;
          if (firstplay == LOW) {
            startmus = LOW;
          }
        }
      }
      // SpaceCoke (BIP == 5): az utolso tarolt golyot NEM azonnal lokjuk ki,
      // hanem 1 mp varakozas utan (maxBallSw) - a gepben futott verziobol
      // atveve, a forditott idozites-feltetel javitasaval
      if (BIP == 5) {
        if (BIS + BIP > 5) {
          if (BIS > 1) {
            BIS = BIS - 1;
            shoottimer = millis();
            shoottimer2 = millis();
            shoot = 1;
            if (firstplay == LOW) {
              startmus = LOW;
            }
          }
          if (BIS == 1 && maxBallSw == LOW) {
            maxBallSw = HIGH;
            maxBallSwTimer = millis();
          }
        }
        if (sidelaneBallsaverSw == HIGH && BIS != 0) {
          sidelaneBallsaverSw = LOW;
          if (BIS > 1) {
            BIS = BIS - 1;
            shoottimer = millis();
            shoottimer2 = millis();
            shoot = 1;
            if (firstplay == LOW) {
              startmus = LOW;
            }
          }
          if (BIS == 1 && maxBallSw == LOW) {
            maxBallSw = HIGH;
            maxBallSwTimer = millis();
          }
        }
        if (maxBallSw == HIGH && millis() - maxBallSwTimer > 1000) {
          BIS = BIS - 1;
          shoottimer = millis();
          shoottimer2 = millis();
          shoot = 1;
          if (firstplay == LOW) {
            startmus = LOW;
          }
          maxBallSw = LOW;
        }
      }
    }

    ///////////////
    //// Nextball
    //////////////

    if (ballsaversw == LOW && shoot == 0 && firstplay == LOW) {
      if (BIS == 5) {
      digitalWrite(leftFlipperBat, LOW);
      digitalWrite(rightFlipperBat, LOW);
        wTrig.stopAllTracks();
        hurryUp = LOW;
        maxBallSw = LOW;
        BIP = 1;

        if (numofplayers == player && ball == 3 && extraball == 0) {
          Serial.println("End");
          delay(20);
          wTrig.trackPlayPoly(46);
        }
        else {
          Serial.println("Next");
          delay(20);
          Serial.flush();
          wTrig.trackPlaySolo(11);
          delay(4500);
        }
        shoottimer = millis();
        shoottimer2 = millis();

        firstplay = HIGH;
        AutoKick = LOW;
        initlight = HIGH;
        Initlights();
        Inittable = HIGH;
        inittable();

        if (bonusx == 1) {
          bonus = bonus * 2;
          bonusx = 0;
        }
        if (bonusx == 2) {
          bonus = bonus * 4;
          bonusx = 0;
        }
        if (bonusx == 3) {
          bonus = bonus * 6;
          bonusx = 0;
        }
        if (bonusx == 4) {
          bonus = bonus * 8;
          bonusx = 0;
        }

        score[player] = score[player] + bonus;
        bonus = 0;
        SendData();
        if (extraball == 0)
        {
          player = player + 1;
          startmus = HIGH;
          firstplay = HIGH;
          weedmetersend();
          if (player > numofplayers) {
            player = 1;
            ball = ball + 1;
            if (ball == 4) {
              initlight = HIGH;
              Initlights();
              intmon = 2;
            }
            else {
              shoot = 1;
            }
          }
        }
        else {
          extraball = extraball - 1;
          shoot = 1;
          wTrig.trackPlaySolo(74);
          startmus = HIGH;
          firstplay = HIGH;
        }
      }
    }


  ////////////////////////
  //// Ejectball
  ////////////////////////
  // A tekercs MINDEN mas esetben LOW-t kap - ha a kilokes kozben barmi
  // nullazna a shoot-ot, a tekercs korabban HIGH-ban ragadt (CoilGuard fogta meg)
  if (shoot == 1 && millis() - shoottimer <= 50) {
    digitalWrite(ballTroughCoil, HIGH);
  }
  else {
    digitalWrite(ballTroughCoil, LOW);
    if (shoot == 1) {
      firstplay = LOW;
    }
  }


  /////////////////////////////////////////////////
  //// Ballkick
  /////////////////////////////////////////////////

  if (SimDigitalRead(shooterLaneSwitch) == LOW && kick == 0) {
    if (AutoKick == LOW && SimDigitalRead(ballShooterButton) == LOW && millis() - 500 > shoottimer2) {
      kick = 1;
      kicktimer = millis();
      AutoKick = HIGH;
      ballsavetimer = millis() + 30000;
      ballsaversw = HIGH;
      ballsavetime = 15000;
    }

    // FONTOS: az utolso kick ota is teljen el 700 ms! Enelkul az 50 ms-os
    // impulzus vege utan AZONNAL ujratuzelt (~60 ms-onkent, gepfegyverkent),
    // amig golyot latott a savban - gyenge kilovesnel ez egette a tekercset.
    if (AutoKick == HIGH && millis() - 700 > shoottimer2 && millis() - kicktimer > 700) {
      kick = 1;
      kicktimer = millis();
    }
  }



  if (kick == 1) {
    digitalWrite(shooterlaneCoil, HIGH);
    if (millis() - 50 > kicktimer) {
      digitalWrite(shooterlaneCoil, LOW);

      if (startmus == HIGH) {
        wTrig.trackPlayPoly(1);
        effect = HIGH;
        effectID = 1;
      }
      else {
        startmus = LOW;
      }
      kick = 0;
      shoot = 0;
      shootfail = 0;
    }
  }

  // Rescue: ha a kilokes elindult (shoot == 1), de a golyo nem ert a kilovosavba,
  // a gomb ~3 mp nyomva tartasa ujra kiloki. Jatek kozben (shoot == 0) nem csinal semmit.
  if (SimDigitalRead(ballShooterButton) == LOW && shoot == 1 && kick == 0){
    shootfail ++;
    if (shootfail > 300){
    shoottimer = millis();
    shoottimer2 = millis();
    shoot = 1;
    kick = 0;
    shootfail=0;
    }
    }
  else {
    shootfail = 0;
  }
  }
///////////////////////////////////
//// Mesure ballstack infravalue
///////////////////////////////////
void MIV(boolean m) {
  Blinktimer();
  if (BallReadState == HIGH || m == HIGH) {
    ball1 = SimAnalogRead(PIN_A0);
    ball2 = SimAnalogRead(PIN_A1);
    ball3 = SimAnalogRead(PIN_A2);
    ball4 = SimAnalogRead(PIN_A3);
    ball5 = SimAnalogRead(PIN_A4);

    // FORDITOTT logika (a gepben futott verziobol atveve):
    // alacsony analog ertek (<100) = golyo ott van!
    if (ball1 < 100){
      ballPresent1 = 1;
      }
    if (ball1 > 100){
      ballPresent1 = 0;
      }

    if (ball2 < 100){
      ballPresent2 = 1;
      }
    if (ball2 > 100){
      ballPresent2 = 0;
      }

    if (ball3 < 100){
      ballPresent3 = 1;
      }
    if (ball3 > 100){
      ballPresent3 = 0;
      }

    if (ball4 < 100){
      ballPresent4 = 1;
      }
    if (ball4 > 100){
      ballPresent4 = 0;
      }

    if (ball5 < 100){
      ballPresent5 = 1;
      }
    if (ball5 > 100){
      ballPresent5 = 0;
      }

      if (BIP != 5){
      BIS = ballPresent1 + ballPresent2 + ballPresent3 + ballPresent4 + ballPresent5;
      }
      else
      {
        if (ballPresent1 == 1 && ballPresent2 == 0 && ballPresent3 == 0 && ballPresent4 == 0 && ballPresent5 == 0)
        {
          BIS = 1;
          }
        if (ballPresent1 == 1 && ballPresent2 == 1 && ballPresent3 == 0 && ballPresent4 == 0 && ballPresent5 == 0)
        {
          BIS = 2;
          }
        if (ballPresent1 == 1 && ballPresent2 == 1 && ballPresent3 == 1 && ballPresent4 == 0 && ballPresent5 == 0)
        {
          BIS = 3;
          }
        if (ballPresent1 == 1 && ballPresent2 == 1 && ballPresent3 == 1 && ballPresent4 == 1 && ballPresent5 == 0)
        {
          BIS = 4;
          }
        if (ballPresent1 == 1 && ballPresent2 == 1 && ballPresent3 == 1 && ballPresent4 == 1 && ballPresent5 == 1)
        {
          BIS = 5;
          }
        if (ballPresent1 == 0 && ballPresent2 == 0 && ballPresent3 == 0 && ballPresent4 == 0 && ballPresent5 == 0)
        {
          BIS = 0;
          }
        }
        
  }

}


/////////////////////////////////////////////////
//// Ballsaver
/////////////////////////////////////////////////

void Ballsave() {
  if (ballsaversw == HIGH && millis() + 30000 - ballsavetime < ballsavetimer) {
    Blinktimer();
    if (ledState == LOW) {
      leds[8] = CRGB::Black; // x4
    }
    if (ledState == HIGH) {
      leds[8] = CRGB::Red; // x4
    }
  }
  else {
    ballsaversw = LOW;
    // a sidelane-zaszlot is torolni kell, kulonben ha a mento a felhasznalas
    // elott jar le, a zaszlo beragad es a KOVETKEZO golyonal fantom-golyot lok ki
    sidelaneBallsaverSw = LOW;
  }
  if (ballsaversw == LOW) {
    leds[8] = CRGB::Black; // x4
  }
}

/////////////////////////////////////////////////
//// Multiball
/////////////////////////////////////////////////
void Multiball() {
  // BIS >= 4: ha ket golyo egy mintaveteli ablakban (1s) folyik le, a BIS 3-rol
  // egybol 5-re ugrik es a == 4 sosem teljesulne -> a multiball beragadna!
  if (multiball != 0 && ballsaversw == LOW && BIS >= 4) {
    if (multiball == 1) {
      wTrig.trackPause(89);
      wTrig.trackLoop(89, 0);
      wTrig.trackResume(1);
    }
    if (multiball == 2) {
      wTrig.trackPause(88);
      wTrig.trackLoop(88, 0);
      wTrig.trackResume(1);
    }
    if (multiball == 3) {
      wTrig.trackPause(64);
      wTrig.trackLoop(64, 0);
      wTrig.trackResume(1);
    }
    if (multiball == 4) {
      wTrig.trackPause(65);
      wTrig.trackLoop(65, 0);
      wTrig.trackResume(1);
    }
    if (multiball == 5) {
      wTrig.trackPause(109);
      wTrig.trackLoop(109, 0);
      wTrig.trackResume(1);
    }
    ufosw = 1;
    multiball = 0;
    spinnersw = 1;
    multiloopsw = 0;
    BIP = 1;
    BrdgLowActive = LOW;
    BrdgHighActive = LOW;
  }
}



/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Ballhadler rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Timers
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Blinktimer() {
  unsigned long BallcurrentMillis = millis();
  unsigned long currentMillis = millis();
  unsigned long blinkycurrentMillis = millis();
  unsigned long cigarcurrentMillis = millis();
  unsigned long effectcurrentMillis = millis();
  unsigned long hurryUpcurrentMillis = millis();

  if (BallcurrentMillis - BallpreviousMillis >= 1000) {
    // save the last time you blinked the LED
    BallpreviousMillis = BallcurrentMillis;

    // if the LED is off turn it on and vice-versa:
    if (BallReadState == LOW) {
      BallReadState = HIGH;
    } else {
      BallReadState = LOW;
    }
  }
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
  }
  if (blinkycurrentMillis - blinkypreviousMillis >= 500) {
    // save the last time you blinked the LED
    blinkypreviousMillis = blinkycurrentMillis;

    // if the LED is off turn it on and vice-versa:
    if (blinky == LOW) {
      blinky = HIGH;
    } else {
      blinky = LOW;
    }
  }
  if (cigarcurrentMillis - cigarpreviousMillis >= 50) {
    // save the last time you blinked the LED
    cigarpreviousMillis = cigarcurrentMillis;

    // if the LED is off turn it on and vice-versa:
    if (cigarState == LOW) {
      cigarState = HIGH;
    } else {
      cigarState = LOW;
    }
  }
  if (effectcurrentMillis - effectpreviousMillis >= 30) {
    // save the last time you blinked the LED
    effectpreviousMillis = effectcurrentMillis;

    // if the LED is off turn it on and vice-versa:
    if (effectState == LOW) {
      effectState = HIGH;
    } else {
      effectState = LOW;
    }
  }
  if (hurryUpcurrentMillis - hurryUppreviousMillis >= 1500) {
    // save the last time you blinked the LED
    hurryUppreviousMillis = hurryUpcurrentMillis;

    // if the LED is off turn it on and vice-versa:
    if (hurryUpState == LOW) {
      hurryUpState = HIGH;
    } else {
      hurryUpState = LOW;
    }
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Times rutins
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Light handling
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
    uint8_t brightness = 255;

    for (int i = fasz; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePalettePeriodically() {
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;

    if (lastSecond != secondHand) {
        lastSecond = secondHand;
        if (secondHand == 0) {
            currentPalette = RainbowColors_p;
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 10) {
            currentPalette = RainbowStripeColors_p;
            currentBlending = NOBLEND;
        }
        if (secondHand == 15) {
            currentPalette = RainbowStripeColors_p;
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 20) {
            SetupPurpleAndGreenPalette();
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 25) {
            SetupTotallyRandomPalette();
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 30) {
            SetupBlackAndWhiteStripedPalette();
            currentBlending = NOBLEND;
        }
        if (secondHand == 35) {
            SetupBlackAndWhiteStripedPalette();
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 40) {
            currentPalette = CloudColors_p;
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 45) {
            currentPalette = PartyColors_p;
            currentBlending = LINEARBLEND;
        }
        if (secondHand == 50) {
            currentPalette = myRedWhiteBluePalette_p;
            currentBlending = NOBLEND;
        }
        if (secondHand == 55) {
            currentPalette = myRedWhiteBluePalette_p;
            currentBlending = LINEARBLEND;
        }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette() {
    for (int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV(random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.

void SetupBlackAndGreenStripedPalette() {
    // 'black out' all 16 palette entries...
    fill_solid(currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Red;
    currentPalette[4] = CRGB::Red;
    currentPalette[8] = CRGB::Red;
    currentPalette[12] = CRGB::Red;

}

void SetupBlackAndWhiteStripedPalette() {
    // 'black out' all 16 palette entries...
    fill_solid(currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB(255, 255, 255);
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette() {
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green = CHSV(HUE_GREEN, 255, 255);
    CRGB black = CRGB::Black;

    currentPalette = CRGBPalette16(
        green, green, black, black,
        purple, purple, black, black,
        green, green, black, black,
        purple, purple, black, black);
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Light handling Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// intm mode and player select
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void intmMode() {
  if (intmon == 1) {
    fasz = 0;
    ChangePalettePeriodically();
    incomeMsg = " ";
      
      /*
      miv1 = SimAnalogRead(a1);
      miv2 = SimAnalogRead(a3);
      miv3 = SimAnalogRead(a4);
      miv4 = SimAnalogRead(a5);
      miv5 = SimAnalogRead(a6);
      Serial.println(miv1);
      Serial.println(miv2);
      Serial.println(miv3);
      Serial.println(miv4);
      Serial.println(miv5);
      Serial.println(SimAnalogRead(a7));
      Serial.println(SimAnalogRead(a2));
      
      Serial.println("----");
      delay(700);
      */

    if(millis()>heysoundtimer + 10000){
      heysoundcounter ++;
      heysoundtimer = millis();
      if(heysoundcounter == 36){
        heysoundcounter = 0;
        wTrig.trackPlayPoly(37);
        }
      }

    
    if (SimDigitalRead(startButton) == LOW) {
      // 1. start: jatekosvalaszto mod - a jatek meg NEM indul!
      // (V2-es CharmMode logika visszahozva)
      wTrig.trackPlayPoly(47);
      Serial.println("Start"); // GUI: kilep az attractbol a SCORE kepernyore
      delay(20);
      numofplayers = 1;
      selArmSw = LOW;          // az inditashoz elobb el kell engedni a startot
      selShootSw = HIGH;       // a shoot gombot is elesiteni kell
      selShootTimer = millis();
      selTimeoutTimer = millis();
      intmon = 3;
      SendData();
    }
  }

  //// Player select mod: shoot gomb = +1 jatekos, 2. start = jatek indul
  if (intmon == 3) {
    SendData(); // folyamatos pontszam/jatekosszam kuldes a GUI-nak

    if (selArmSw == LOW && SimDigitalRead(startButton) == HIGH) {
      selArmSw = HIGH; // a belepo startnyomas elengedve -> a kovetkezo mar indit
    }
    if (selShootSw == HIGH && millis() - 250 > selShootTimer && SimDigitalRead(ballShooterButton) == HIGH) {
      selShootSw = LOW;
    }

    // Shoot / player select gomb: +1 jatekos (4 utan korbefordul 1-re)
    if (SimDigitalRead(ballShooterButton) == LOW && selShootSw == LOW) {
      selShootSw = HIGH;
      selShootTimer = millis();
      selTimeoutTimer = millis();
      numofplayers = numofplayers + 1;
      if (numofplayers == 5) {
        numofplayers = 1;
      }
      wTrig.trackPlayPoly(103);
      SendData();
    }

    // 2. start: jatek inditasa a kivalasztott jatekosszammal
    if (selArmSw == HIGH && SimDigitalRead(startButton) == LOW) {
      wTrig.trackPlayPoly(15);
      Serial.println("Zero");
      delay(300);
      intmon = 0;
      fasz = 68;
      ball = 1;
      player = 1;
      firstplay = HIGH;
      startmus = HIGH;
      BIP = 1;
      Inittable = HIGH;
      inittable();
      initlight = HIGH;
      Initlights();
      score[0] = 0;
      weedm[0] = 0;
      weedmeter[0] = 180;
      score[1] = 0;
      weedm[1] = 0;
      weedmeter[1] = 180;
      score[2] = 0;
      weedm[2] = 0;
      weedmeter[2] = 180;
      score[3] = 0;
      weedm[3] = 0;
      weedmeter[3] = 180;
      score[4] = 0;
      weedm[4] = 0;
      weedmeter[4] = 180;
      weedmetersend();
      feny[0] = 960;
      miv1 = SimAnalogRead(a1);
      miv2 = SimAnalogRead(a3);
      miv3 = SimAnalogRead(a4);
      miv4 = SimAnalogRead(a5);
      miv5 = SimAnalogRead(a6);
      cheechCollectives[0] = 0;
      chongCollectives[0] = 0;
      cheechCollectives[1] = 0;
      chongCollectives[1] = 0;
      cheechCollectives[2] = 0;
      chongCollectives[2] = 0;
      cheechCollectives[3] = 0;
      chongCollectives[3] = 0;
      cheechCollectives[4] = 0;
      chongCollectives[4] = 0;
      bonus = 0;
      bonusx = 0;
    }

    // 60 mp tetlenseg: vissza az attract modba
    if (millis() - selTimeoutTimer > 60000) {
      intmon = 1;
      numofplayers = 1;
      heysoundtimer = millis();
      Serial.println("Attract"); // GUI: attract-loop ujraindul
      delay(20);
    }
  }
  if (intmon == 2)
  {

    if (Serial.available() > 0 && intmon == 2) {
      // read the incoming:
      incomeMsg = Serial.readString();
    }

    //// Send buttonstates to Highscore table
    if (SimDigitalRead(leftFlipperButton) == LOW && lfHscSw == 0) {
      lfHscSw = 1;
      lfHscTimer = millis();
      wTrig.trackPlayPoly(105);
      Serial.println("FLIPPER_LEFT"); // Python GUI protokoll (regi Unity-nev: "Right")
      delay(20);
    }
    if (lfHscSw == 1 && millis() - 250 > lfHscTimer && SimDigitalRead(leftFlipperButton) == HIGH) {
      lfHscSw = 0;
    }

    if (SimDigitalRead(rightflipperButton) == LOW && rfHscSw == 0) {
      rfHscSw = 1;
      rfHscTimer = millis();
      wTrig.trackPlayPoly(106);
      Serial.println("FLIPPER_RIGHT"); // Python GUI protokoll (regi Unity-nev: "Left")
      delay(20);
    }
    if (rfHscSw == 1 && millis() - 250 > rfHscTimer && SimDigitalRead(rightflipperButton) == HIGH) {
      rfHscSw = 0;
    }

    if (SimDigitalRead(ballShooterButton) == LOW && shHscSw == 0) {
      shHscSw = 1;
      shHscTimer = millis();
      wTrig.trackPlayPoly(107);
      Serial.println("PLAYER_PRESS"); // Python GUI protokoll: betu kivalasztasa
      delay(20);
    }
    if (shHscSw == 1 && millis() - 250 > shHscTimer && SimDigitalRead(ballShooterButton) == HIGH) {
      shHscSw = 0;
    }

    if (SimDigitalRead(startButton) == LOW && stHscSw == 0) {
      stHscSw = 1;
      stHscTimer = millis();
      Serial.println("Start"); // GUI: nevbevitel kihagyasa (skip)
      delay(20);
    }
    if (stHscSw == 1 && millis() - 250 > stHscTimer && SimDigitalRead(startButton) == HIGH) {
      stHscSw = 0;
    }


    if (incomeMsg == "Exit") {
        digitalWrite(PIN_A13, LOW);
    }
    if (incomeMsg == "Exit1") {
        wTrig.trackPlayPoly(102);
        digitalWrite(PIN_A13, LOW);
    }
    if (incomeMsg == "Exit2") {
      wTrig.trackPlayPoly(104);
      digitalWrite(PIN_A13, LOW);
    }
    if (SimDigitalRead(startButton) == LOW && SimDigitalRead(ballShooterButton) == LOW) {
        resetTimer++;
        if (resetTimer > 2000) {
            digitalWrite(PIN_A13, LOW);
        }
    }
  }

}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End intm mode && player select rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Inittable
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void inittable() {
  if (Inittable == HIGH) {
    tiltcounter = 0;
    cncswitch1 = 0;
    cncswitch2 = 0;
    cncswitch3 = 0;
    weedswitch1 = 0;
    weedswitch2 = 0;
    weedswitch3 = 0;
    weedswitch4 = 0;
    fishTankLightState1 = 0;
    fishTankLightState2 = 0;
    chongLightActiveSw = 0;
    cheechLightActiveSw = 0;
    chongactive = 0;
    ufosw = 0;
    davearr[1] = 0;
    davearr[2] = 0;
    davearr[3] = 0;
    davearr[4] = 0;
    gatearr[1] = 0;
    gatearr[2] = 2;
    gatearr[3] = 0;
    BrdgLowActive = LOW;
    BrdgHighActive = LOW;
    bonusx1sw = 0;
    bonusx2sw = 0;
    bonusx3sw = 0;
    bonusx4sw = 0;
    spinnersw = 0;
    Inittable = LOW;
    weedmetersend();
    beerCollect = 0;
    randGift = random(1, 10);
    tiltcounter =0;
    switch (randGift) {
      case 1:
        cncswitch1 = 2;
        break;
      case 2:
        cncswitch2 = 2;
        break;
      case 3:
        cncswitch3 = 2;
        break;
      case 4:
        weedswitch1 = 2;
        break;
      case 5:
        weedswitch2 = 2;
        break;
      case 6:
        weedswitch3 = 2;
        break;
      case 7:
        weedswitch4 = 2;
        break;
      case 8:
        fishTankLightState1 = 2;
        break;
      case 9:
        fishTankLightState2 = 2;
        break;
    }
    giftsw = 1;
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Inittable Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Data send
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void SendData() {
  if (sendDsw == LOW) {
    sendDsw = HIGH;
    sendDtimer = millis();
  }
  if (millis() - 350 > sendDtimer && sendDsw == HIGH) {
    char scoremsg[64];
    snprintf(scoremsg, sizeof(scoremsg), "score,%lu,%d,%d,%d,%lu,%d",
             score[player], numofplayers, player, ball, bonus, bonusx);
    Serial.println(scoremsg);
    sendDsw = LOW;
  }


}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Data send rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Score
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void Score(unsigned long scr, unsigned long bns) {
  bonus = bonus + bns;
  score[player] = score[player] + scr;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Score Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Initlights
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Initlights() {
  if (initlight == 1) {
    leds[0] = CRGB::White;
    leds[1] = CRGB::White;
    //leds[2] = CRGB::Black; // V
    //leds[3] = CRGB::Black; // E
    leds[4] = CRGB::White;
    leds[5] = CRGB::White;
    //leds[6] = CRGB::Black; // x8
    //leds[7] = CRGB::Black; // x6
    //leds[8] = CRGB::Black; // Ballsave
    //leds[9] = CRGB::Black; // x4
    //leds[10] = CRGB::Black; // x2
    leds[11] = CRGB::White;
    leds[12] = CRGB::White;
    leds[13] = CRGB::White;
    leds[14] = CRGB::White;
    //leds[15] = CRGB::Black; // A
    //leds[16] = CRGB::Black; // D
    leds[17] = CRGB::White; // CnC ambient
    //leds[18] = CRGB::Black; // C
    //leds[19] = CRGB::Black; // &
    //leds[20] = CRGB::Black; // C
    leds[21] = CRGB::Black; // Left loop
    leds[22] = CRGB::Black; // Left loop
    leds[23] = CRGB::White; // Left ramp ambient
    leds[24] = CRGB::Black; // Left ramp
    leds[25] = CRGB::Black; // Left ramp
    leds[26] = CRGB::Black; // Chong indicator
    leds[27] = CRGB::Black; // Get High
    leds[28] = CRGB::Black; // Get high
    //leds[29] = CRGB::Black; // W
    //leds[30] = CRGB::Black; // E
    //leds[31] = CRGB::Black; // E
    //leds[32] = CRGB::Black; // D
    leds[33] = CRGB::Black; // Cheech indicator
    leds[34] = CRGB::Black; // Right ramp arrow 2/2
    leds[35] = CRGB::Black; // Right ramp arrow 2/1
    leds[36] = CRGB::Black; // Right ramp arrow 1/1
    leds[37] = CRGB::Black; // Right ramp arrow 1/1
    //leds[38] = CRGB::Orange; // Cheech wheel
    //leds[39] = CRGB::Orange; // Chong Pipe
    //leds[40] = CRGB::Orange; // Chong Cockroach
    //leds[41] = CRGB::Orange; // Cheech cigar
    leds[42] = CRGB::Black; // UFO arrow 1
    leds[43] = CRGB::Black; // UFO arrow 2
    leds[44] = CRGB::Black; // Right loop 2
    leds[45] = CRGB::Black; // Right loop 1
    //leds[46] = CRGB::Orange; // MUF dvr
    //leds[47] = CRGB::Orange; // Chong plunger key
    leds[48] = CRGB::Black; // Fish
    leds[49] = CRGB::Black; // Tank
    leds[50] = CRGB::White; // Fishtank ambient
    leds[51] = CRGB::White; // Right ramp ambient
    leds[52] = CRGB::Green; // Gate 3 ambient
    leds[53] = CRGB::Black; // Gate 3
    leds[54] = CRGB::Green; // Gate 32 ambient
    leds[55] = CRGB::White; // Gate 2
    leds[56] = CRGB::Green; // Gate 21 ambient
    leds[57] = CRGB::Black; // Gate 1
    leds[58] = CRGB::Green; // Gate 1 ambient
    leds[59] = CRGB::Gray; // Michoakan
    leds[60] = CRGB::Gray; // Acapulco Gold
    leds[61] = CRGB::Gray; // Hightable flasher
    leds[62] = CRGB::Gray; // Thai stick
    leds[63] = CRGB::Gray; // Labrador
    leds[64] = CRGB::Blue; // Pop1
    leds[65] = CRGB::Blue; // Pop2
    leds[66] = CRGB::Blue; // Pop3
    leds[67] = CRGB::Yellow; // Car ambient
    fasz = 68;
    currentPalette = RainbowColors_p;
    initlight = 0;
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Initlight rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Flippers
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void Left_Flipper() {
  if (SimDigitalRead(leftFlipperButton) == LOW && flipl == 0) {
    flipl = 1;
    flipltimer = millis();
  }
  if (flipl == 1) {
    digitalWrite(leftFlipperBat, HIGH);
  }
  if (SimDigitalRead(leftFlipperButton) == HIGH && millis() - 100 > flipltimer) {
    digitalWrite(leftFlipperBat, LOW);
    flipl = 0;
  }
}


void Right_Flipper() {
  if (SimDigitalRead(rightflipperButton) == LOW && flipr == 0) {
    flipr = 1;
    fliprtimer = millis();
  }
  if (flipr == 1) {
    digitalWrite(rightFlipperBat, HIGH);
  }
  if (SimDigitalRead(rightflipperButton) == HIGH && millis() - 100 > fliprtimer) {
    digitalWrite(rightFlipperBat, LOW);
    flipr = 0;
  }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Flippers Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Slingshots
/////////////////////////////////////////////////
/////////////////////////////////////////////////


void Right_Slingshot() {
  if (SimDigitalRead(rightSlingshotSwitch) == LOW && slingr == 0) {
    slingr = 1;
    slingrtimer = millis();
    wTrig.trackPlayPoly(29);
    ufoInactivesw = 1;
    ufoInactiveTimer = millis();
    ballHandlerSkip = 1;
    ballHandlerSkipTimer = millis();

    if (hurryUp == HIGH) {
      effect = HIGH;
      effectID = 6;
      Score(2500, 100);
      Serial.println("Point1");
      delay(20);
    }
    else {
      Score(0, 50);
    }

  }
  if (slingr == 1 && millis() - 12 < slingrtimer) {
    digitalWrite(rightSlingshotCoil, HIGH);
  }
  else {
    digitalWrite(rightSlingshotCoil, LOW);
  }

  if (SimDigitalRead(rightSlingshotSwitch) == HIGH && millis() - 500 > slingrtimer) {
    slingr = 0;
  }
}







void Left_Slingshot() {
  if (SimDigitalRead(leftSlingshotSwitch) == LOW && slingl == 0) {
    slingl = 1;
    slingltimer = millis();
    wTrig.trackPlayPoly(29);
    ufoInactivesw = 1;
    ufoInactiveTimer = millis();
    ballHandlerSkip = 1;
    ballHandlerSkipTimer = millis();

    if (hurryUp == HIGH) {
      effect = HIGH;
      effectID = 6;
      Score(2500, 100);
      Serial.println("Point1");
      delay(20);
    }
    else {
      Score(0, 50);
    }

  }
  if (slingl == 1 && millis() - 12 < slingltimer) {
    digitalWrite(leftSlingshotCoil, HIGH);
  }
  else {
    digitalWrite(leftSlingshotCoil, LOW);
  }
  if (SimDigitalRead(leftSlingshotSwitch) == HIGH && millis() - 500 > slingltimer) {
    slingl = 0;
  }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Slingshots Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// CnC
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void CnC() {
  if (SimDigitalRead(cncLetterC3rd) == HIGH) {
    Blinktimer();
    if (effect == LOW) {
      leds[18] = CRGB::Yellow;
    }
    if (ledState == LOW) {
      leds[18] = CRGB::Black;
    }
  }



  if (hurryUp == LOW) {
    /// Normal mode
    if (SimDigitalRead(cncLetterC3rd) == LOW && cncswitch3 == 0) {
      cncswitch3 = 1;
      wTrig.trackPlayPoly(12);
      Score(1500, 50);
      if (giftsw == 1) {
        giftsw = 0;
        Gift();
      }
    }
    if (SimDigitalRead(cncLetterN) == LOW && cncswitch2 == 0) {
      cncswitch2 = 1;
      wTrig.trackPlayPoly(13);
      Score(1500, 50);
      if (giftsw == 1) {
        giftsw = 0;
        Gift();
      }
    }
    if (SimDigitalRead(cncLetterC1st) == LOW && cncswitch1 == 0) {
      cncswitch1 = 1;
      wTrig.trackPlayPoly(14);
      Score(1500, 50);
      if (giftsw == 1) {
        giftsw = 0;
        Gift();
      }
    }
  }

  if (hurryUp == HIGH) {
    /// HurryUp mode
    if (SimDigitalRead(cncLetterC3rd) == LOW && cnchurryswitch3 == 0) {
      cnchurryswitch3 = 1;
      cnchurryTimer3 = millis();
      wTrig.trackPlayPoly(12);
      effect = HIGH;
      effectID = 6;
      Score(2500, 500);
      Serial.println("Point1");
    }
    if (SimDigitalRead(cncLetterN) == LOW && cnchurryswitch2 == 0) {
      cnchurryswitch2 = 1;
      cnchurryTimer2 = millis();
      wTrig.trackPlayPoly(13);
      effect = HIGH;
      effectID = 6;
      Score(2500, 500);
      Serial.println("Point1");
    }
    if (SimDigitalRead(cncLetterC1st) == LOW && cnchurryswitch1 == 0) {
      cnchurryswitch1 = 1;
      cnchurryTimer1 = millis();
      wTrig.trackPlayPoly(14);
      effect = HIGH;
      effectID = 6;
      Score(2500, 500);
      Serial.println("Point1");
      Score(1500, 50);
    }

    if (millis() - 350 > cnchurryTimer1) {
      cnchurryswitch1 = 0;
    }
    if (millis() - 350 > cnchurryTimer2) {
      cnchurryswitch2 = 0;
    }
    if (millis() - 350 > cnchurryTimer3) {
      cnchurryswitch3 = 0;
    }

  }



  /// Random gift
  if (giftsw == 1 ) {
    if (SimDigitalRead(cncLetterC3rd) == LOW && cncswitch3 == 2) {
      cncswitch3 = 1;
      wTrig.trackPlayPoly(12);
      wTrig.trackPlayPoly(36);
      wTrig.trackPlayPoly(42);
      Score(5000, 500);
      delay(10);
      Serial.println("Point2");
      giftsw = 3;
    }
    if (SimDigitalRead(cncLetterN) == LOW && cncswitch2 == 2) {
      cncswitch2 = 1;
      wTrig.trackPlayPoly(13);
      wTrig.trackPlayPoly(36);
      wTrig.trackPlayPoly(42);
      Score(5000, 500);
      delay(10);
      Serial.println("Point2");
      giftsw = 3;
    }
    if (SimDigitalRead(cncLetterC1st) == LOW && cncswitch1 == 2) {
      cncswitch1 = 1;
      wTrig.trackPlayPoly(14);
      wTrig.trackPlayPoly(36);
      wTrig.trackPlayPoly(42);
      Score(5000, 500);
      delay(10);
      Serial.println("Point2");
      giftsw = 3;
    }
  }
  /// End random gift

  if (effect == LOW) {
    if (cncswitch3 == 1) {
      leds[18] = CRGB::White; // C
    }
    if (cncswitch2 == 1) {
      leds[19] = CRGB::White; // C
    }
    if (cncswitch1 == 1) {
      leds[20] = CRGB::White; // C
    }
    if (cncswitch3 == 0) {
      leds[18] = CRGB::Black; // C
    }
    if (cncswitch2 == 0) {
      leds[19] = CRGB::Black; // C
    }
    if (cncswitch1 == 0) {
      leds[20] = CRGB::Black; // C
    }
    /// Random gift
    if (giftsw == 1 ) {
      if (cncswitch3 == 2) {
        if (ledState == HIGH) {
          leds[18] = CRGB::Green; // C
        }
        if (ledState == LOW) {
          leds[18] = CRGB::Yellow; // C
        }
      }
      if (cncswitch2 == 2) {
        if (ledState == HIGH) {
          leds[19] = CRGB::Green; // C
        }
        if (ledState == LOW) {
          leds[19] = CRGB::Yellow; // C
        }
      }
      if (cncswitch1 == 2) {
        if (ledState == HIGH) {
          leds[20] = CRGB::Green; // C
        }
        if (ledState == LOW) {
          leds[20] = CRGB::Yellow; // C
        }
      }
    }
  }


  if (cncswitch1 == 1 && cncswitch2 == 1 && cncswitch3 == 1 && cncoff == 0) {
    cnctimer = millis();
    cncoff = 1;
    bonus = bonus + 750;
  }

  if (cncoff == 1) {
    chongLightActiveSw = HIGH;
    cheechLightActiveSw = HIGH;
    if (millis() - 1000 < cnctimer) {
      Blinktimer();
      if (ledState == HIGH) {
        leds[18] = CRGB::Green; // C
        leds[19] = CRGB::Green; // C
        leds[20] = CRGB::Green; // C
      }
      if (ledState == LOW) {
        leds[18] = CRGB::Yellow; // C
        leds[19] = CRGB::Yellow; // C
        leds[20] = CRGB::Yellow; // C
      }


    }

    if (millis() - 1100 > cnctimer) {
      cncswitch1 = 0;
      cncswitch2 = 0;
      cncswitch3 = 0;
      cncoff = 0;
    }
  }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End CnC Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Loop
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void Loopshoot() {
  if (SimDigitalRead(loopSwitchTop) == LOW && loopsw == LOW) {
    looptimer = millis();
    loopswt = millis();
    loopsw = HIGH;
    wTrig.trackPlayPoly(19);
  }
  if (SimDigitalRead(loopSwitchSide) == LOW && millis() - 1000 < looptimer && loopsw == LOW) {
    if (multiloopsw == 1) {
      Serial.println("Jackpot6");
      wTrig.trackPlayPoly(73);
    }
    wTrig.trackPlayPoly(6);
    if (multiloopsw == 1) {
      Score(30000, 2000);
    }
    else {
      Score(2000, 500);
    }
    effect = HIGH;
    effectID = 4;
    loopswt = millis();
    loopsw = HIGH;
    multiloopsw = 0;
  }
  if (loopsw == HIGH && millis() - 100 > loopswt) {
    loopsw = LOW;
  }
  if (multiloopsw == 1) {
    if (ledState == HIGH) {
      leds[21] = CRGB::Red; // Left loop
      leds[22] = CRGB::Yellow; // Left loop
    }
    if (ledState == LOW) {
      leds[21] = CRGB::Yellow; // Left loop
      leds[22] = CRGB::Red; // Left loop
    }
  }
  if (multiloopsw == 0) {
    leds[21] = CRGB::Black; // Left loop
    leds[22] = CRGB::Black; // Left loop
  }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Loop Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Weed
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void Weed() {


  if (hurryUp == LOW) {
    /// Normal mode
    if (SimDigitalRead(d11) == LOW && weedswitch1 == 0) {
      weedswitch1 = 1;
      Score(1000, 50);
      wTrig.trackPlayPoly(5);
      if (giftsw == 1) {
        giftsw = 0;
        Gift();
      }
    }
    if (SimDigitalRead(d10) == LOW && weedswitch2 == 0) {
      weedswitch2 = 1;
      Score(1000, 50);
      wTrig.trackPlayPoly(39);
      if (giftsw == 1) {
        giftsw = 0;
        Gift();
      }
    }
    if (SimDigitalRead(d9) == LOW && weedswitch3 == 0) {
      weedswitch3 = 1;
      Score(1000, 50);
      wTrig.trackPlayPoly(40);
      if (giftsw == 1) {
        giftsw = 0;
        Gift();
      }
    }
    if (SimDigitalRead(d8) == LOW && weedswitch4 == 0) {
      weedswitch4 = 1;
      Score(1000, 50);
      wTrig.trackPlayPoly(41);
      if (giftsw == 1) {
        giftsw = 0;
        Gift();
      }
    }
  }

  if (hurryUp == HIGH) {
    /// HurryUp mode
    if (SimDigitalRead(d11) == LOW && weedhurryswitch1 == 0) {
      weedhurryswitch1 = 1;
      weedhurrytimer1 = millis();
      effect = HIGH;
      effectID = 6;
      Score(2500, 100);
      Serial.println("Point1");
      delay(20);
      wTrig.trackPlayPoly(5);
      if (giftsw == 1) {
        giftsw = 0;
        Gift();
      }
    }
    if (SimDigitalRead(d10) == LOW && weedhurryswitch2 == 0) {
      weedhurryswitch2 = 1;
      weedhurrytimer2 = millis();
      effect = HIGH;
      effectID = 6;
      Score(2500, 100);
      Serial.println("Point1");
      delay(20);
      wTrig.trackPlayPoly(39);
      if (giftsw == 1) {
        giftsw = 0;
        Gift();
      }
    }
    if (SimDigitalRead(d9) == LOW && weedhurryswitch3 == 0) {
      weedhurryswitch3 = 1;
      weedhurrytimer3 = millis();
      effect = HIGH;
      effectID = 6;
      Score(2500, 100);
      Serial.println("Point1");
      delay(20);
      wTrig.trackPlayPoly(40);
      if (giftsw == 1) {
        giftsw = 0;
        Gift();
      }
    }
    if (SimDigitalRead(d8) == LOW && weedhurryswitch4 == 0) {
      weedhurryswitch4 = 1;
      weedhurrytimer4 = millis();
      effect = HIGH;
      effectID = 6;
      Score(2500, 100);
      Serial.println("Point1");
      delay(20);
      wTrig.trackPlayPoly(41);
      if (giftsw == 1) {
        giftsw = 0;
        Gift();
      }
    }
    if (millis() - 350 > weedhurrytimer1) {
      weedhurryswitch1 = 0;
    }
    if (millis() - 350 > weedhurrytimer2) {
      weedhurryswitch2 = 0;
    }
    if (millis() - 350 > weedhurrytimer3) {
      weedhurryswitch3 = 0;
    }
    if (millis() - 350 > weedhurrytimer4) {
      weedhurryswitch4 = 0;
    }
  }

  /// Random gift
  if (giftsw == 1 ) {
    if (SimDigitalRead(d11) == LOW && weedswitch1 == 2) {
      weedswitch1 = 1;
      Score(5000, 100);
      wTrig.trackPlayPoly(5);
      wTrig.trackPlayPoly(36);
      wTrig.trackPlayPoly(42);
      Serial.println("Point2");
      giftsw = 3;
    }
    if (SimDigitalRead(d10) == LOW && weedswitch2 == 2) {
      weedswitch2 = 1;
      Score(5000, 100);
      wTrig.trackPlayPoly(39);
      wTrig.trackPlayPoly(36);
      wTrig.trackPlayPoly(42);
      Serial.println("Point2");
      giftsw = 3;
    }
    if (SimDigitalRead(d9) == LOW && weedswitch3 == 2) {
      weedswitch3 = 1;
      Score(5000, 100);
      wTrig.trackPlayPoly(40);
      wTrig.trackPlayPoly(36);
      wTrig.trackPlayPoly(42);
      Serial.println("Point2");
      giftsw = 3;
    }
    if (SimDigitalRead(d8) == LOW && weedswitch4 == 2) {
      weedswitch4 = 1;
      Score(5000, 100);
      wTrig.trackPlayPoly(41);
      wTrig.trackPlayPoly(36);
      wTrig.trackPlayPoly(42);
      Serial.println("Point2");
      giftsw = 3;
    }
  }

  /// End random gift

  if (effect == LOW) {
    if (weedswitch1 == 1) {
      leds[29] = CRGB::White; // C
    }
    if (weedswitch2 == 1) {
      leds[30] = CRGB::White; // C
    }
    if (weedswitch3 == 1) {
      leds[31] = CRGB::White; // C
    }
    if (weedswitch4 == 1) {
      leds[32] = CRGB::White; // C
    }
    if (weedswitch1 == 0) {
      leds[29] = CRGB::Black; // C
    }
    if (weedswitch2 == 0) {
      leds[30] = CRGB::Black; // C
    }
    if (weedswitch3 == 0) {
      leds[31] = CRGB::Black; // C
    }
    if (weedswitch4 == 0) {
      leds[32] = CRGB::Black; // C
    }


    /// Random gift
    if (giftsw == 1 ) {
      if (weedswitch1 == 2) {
        if (ledState == HIGH) {
          leds[29] = CRGB::Green; // C
        }
        if (ledState == LOW) {
          leds[29] = CRGB::Yellow; // C
        }
      }
      if (weedswitch2 == 2) {
        if (ledState == HIGH) {
          leds[30] = CRGB::Green; // C
        }
        if (ledState == LOW) {
          leds[30] = CRGB::Yellow; // C
        }
      }
      if (weedswitch3 == 2) {
        if (ledState == HIGH) {
          leds[31] = CRGB::Green; // C
        }
        if (ledState == LOW) {
          leds[31] = CRGB::Yellow; // C
        }
      }
      if (weedswitch4 == 2) {
        if (ledState == HIGH) {
          leds[32] = CRGB::Green; // C
        }
        if (ledState == LOW) {
          leds[32] = CRGB::Yellow; // C
        }
      }
    }

  }

  if (weedswitch1 == 1 && weedswitch2 == 1 && weedswitch3 == 1 && weedswitch4 == 1 && weedoff == 0) {
    weedtimer = millis();
    weedoff = 1;
    effect = HIGH;
    effectID = 3;
    wTrig.trackPlayPoly(72);
    spsound = random(1, 5);
    if (spsound == 1) {
      wTrig.trackPlayPoly(67); // I love weed
      Serial.println("Weed");
      delay(10);
    }
    if (spsound == 2) {
      wTrig.trackPlayPoly(66);
      Serial.println("Weed");
      delay(10);
    }
    if (spsound == 3) {
        wTrig.trackPlayPoly(115);
        Serial.println("Weed");
        delay(10);
    }
    if (spsound == 4) {
        wTrig.trackPlayPoly(116);
        Serial.println("Weed");
        delay(10);
    }

  }

  if (weedoff == 1) {
    if (multiball == 0 && hurryUp == LOW) {
      ufosw = 1;
      SetupBlackAndGreenStripedPalette();
      currentBlending = NOBLEND;
      spinnersw = 1;
    }

    if (millis() - 1000 < weedtimer) {
      Blinktimer();
      if (ledState == HIGH) {
        leds[29] = CRGB::Green; // C
        leds[30] = CRGB::Green; // C
        leds[31] = CRGB::Green; // C
        leds[32] = CRGB::Green; // C
      }
      if (ledState == LOW) {
        leds[29] = CRGB::Yellow; // C
        leds[30] = CRGB::Yellow; // C
        leds[31] = CRGB::Yellow; // C
        leds[32] = CRGB::Yellow; // C
      }


    }

    if (millis() - 1100 > weedtimer) {
      weedswitch1 = 0;
      weedswitch2 = 0;
      weedswitch3 = 0;
      weedswitch4 = 0;
      weedoff = 0;
      Score(1000, 500);
    }
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Weed Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Fishtank
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void Fishtank() {
  if (SimDigitalRead(fishTankSwitch1) == LOW && fishTankLightState1 == 0) {
    fishTankLightState1 = 1;
    if (hurryUp == HIGH) {
      effect = HIGH;
      effectID = 6;
      Score(2500, 100);
      Serial.println("Point1");
      delay(20);
    }
    else {
      Score(1500, 10);
    }
    wTrig.trackPlayPoly(10);
    if (giftsw == 1) {
      giftsw = 0;
      Gift();
    }
  }
  if (SimDigitalRead(fishTankSwitch2) == LOW && fishTankLightState2 == 0) {
      fishTankLightState2 = 1;
    if (hurryUp == HIGH) {
      effect = HIGH;
      effectID = 6;
      Score(2500, 100);
      Serial.println("Point1");
      delay(20);
    }
    else {
      Score(1500, 10);
    }
    wTrig.trackPlayPoly(2);
    if (giftsw == 1) {
      giftsw = 0;
      Gift();
    }
  }

  // Random gift
  if (giftsw == 1) {
    if (SimDigitalRead(fishTankSwitch1) == LOW && fishTankLightState1 == 2) {
      fishTankLightState1 = 1;
      Score(5000, 100);
      wTrig.trackPlayPoly(10);
      wTrig.trackPlayPoly(36);
      wTrig.trackPlayPoly(42);
      Serial.println("Point2");
      giftsw = 3;
    }


    if (SimDigitalRead(fishTankSwitch2) == LOW && fishTankLightState2 == 2) {
      fishTankLightState2 = 1;
      Score(5000, 100);
      wTrig.trackPlayPoly(2);
      wTrig.trackPlayPoly(36);
      wTrig.trackPlayPoly(42);
      Serial.println("Point2");
      giftsw = 3;
    }
  }



  // End random gift



  if (effect == LOW) {
    if (fishTankLightState1 == 1) {
      leds[48] = CRGB::White; // C
    }
    if (fishTankLightState2 == 1) {
      leds[49] = CRGB::White; // C
    }

    if (fishTankLightState1 == 0) {
      leds[48] = CRGB::Black; // C
    }
    if (fishTankLightState2 == 0) {
      leds[49] = CRGB::Black; // C
    }
    if (giftsw == 1 ) {
      if (fishTankLightState1 == 2) {
        if (ledState == HIGH) {
          leds[48] = CRGB::Green; // C
        }
        if (ledState == LOW) {
          leds[48] = CRGB::Yellow; // C
        }
      }
      if (fishTankLightState2 == 2) {
        if (ledState == HIGH) {
          leds[49] = CRGB::Green; // C
        }
        if (ledState == LOW) {
          leds[49] = CRGB::Yellow; // C
        }
      }
    }
  }

  if (fishTankLightState1 == 1 && fishTankLightState2 == 1 && fishoff == 0) {
    fishtimer = millis();
    fishoff = 1;
    beerCollect ++;
    if (beerCollect == 1) {
      Serial.println("Beer1");
    }
    if (beerCollect == 2) {
      Serial.println("Beer2");
    }
    if (beerCollect == 3) {
      Serial.println("Beer3");
      BrdgLowActive = HIGH;
      wTrig.trackPlayPoly(98);
      beerCollect = 0;
    }
  }


  if (fishoff == 1) {
    if (millis() - 1000 < fishtimer) {
      Blinktimer();
      if (ledState == HIGH) {
        leds[48] = CRGB::Green; // C
        leds[49] = CRGB::Green; // C
      }
      if (ledState == LOW) {
        leds[48] = CRGB::Yellow; // C
        leds[49] = CRGB::Yellow; // C
      }

    }

    if (millis() - 1100 > fishtimer) {
      fishTankLightState1 = 0;
      fishTankLightState2 = 0;
      fishoff = 0;
    }
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Fishtank Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////



/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// DAVE
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void Dave_switch() {
  if (SimDigitalRead(daveLaneSwitchD) == LOW && sltimesw == LOW) {
    wTrig.trackPlayPoly(23);
    davearr[1] = 1;
    sltimesw = HIGH;
    sltimer = millis();
    if (ballsaversw == HIGH) {
        sidelaneBallsaverSw = HIGH;
    }
  }
  if (SimDigitalRead(daveLaneSwitchA) == LOW && sltimesw == LOW) {
    wTrig.trackPlayPoly(23);
    davearr[2] = 1;
    sltimesw = HIGH;
    sltimer = millis();
  }
  if (SimDigitalRead(daveLaneSwitchV) == LOW && sltimesw == LOW) {
    wTrig.trackPlayPoly(23);
    davearr[3] = 1;
    sltimesw = HIGH;
    sltimer = millis();
  }
  if (SimDigitalRead(daveLaneSwitchE) == LOW && sltimesw == LOW) {
    wTrig.trackPlayPoly(23);
    davearr[4] = 1;
    sltimesw = HIGH;
    sltimer = millis();
    if (ballsaversw == HIGH) {
        sidelaneBallsaverSw = HIGH;
    }
  }

  if (millis() - 1000 > sltimer) {
    sltimesw = LOW;
  }


  if (effect == LOW) {
    if (davearr[1] == 1) {
      leds[16] = CRGB::White; // D
    }
    if (davearr[2] == 1) {
      leds[15] = CRGB::White; // A
    }
    if (davearr[3] == 1) {
      leds[2] = CRGB::White; // D
    }
    if (davearr[4] == 1) {
      leds[3] = CRGB::White; // D
    }
    if (davearr[1] == 0) {
      leds[16] = CRGB::Black; // D
    }
    if (davearr[2] == 0) {
      leds[15] = CRGB::Black; // A
    }
    if (davearr[3] == 0) {
      leds[2] = CRGB::Black; // D
    }
    if (davearr[4] == 0) {
      leds[3] = CRGB::Black; // D
    }
  }

  // Shift left
  if (slltimesw == LOW && SimDigitalRead(leftFlipperButton) == LOW) {
    slltimesw = HIGH;
    slltimer = millis();
    davearr[0] = davearr[1];
    davearr[1] = davearr[2];
    davearr[2] = davearr[3];
    davearr[3] = davearr[4];
    davearr[4] = davearr[0];
  }


  if (millis() - 200 > slltimer && SimDigitalRead(leftFlipperButton) == HIGH) {
    slltimesw = LOW;
  }



  // Shift Right
  if (slrtimesw == LOW && SimDigitalRead(rightflipperButton) == LOW) {
    slrtimesw = HIGH;
    slrtimer = millis();
    davearr[5] = davearr[4];
    davearr[4] = davearr[3];
    davearr[3] = davearr[2];
    davearr[2] = davearr[1];
    davearr[1] = davearr[5];

  }

  if (millis() - 200 > slrtimer && SimDigitalRead(rightflipperButton) == HIGH) {
    slrtimesw = LOW;
  }




  if (daveoff == 0 && davearr[1] == 1 && davearr[2] == 1 && davearr[3] == 1 && davearr[4] == 1) {
    daveoff = 1;
    davetimer = millis();
    ballsavetimer = millis() + 30000;
    if (ballsaversw == LOW) {
      ballsaversw = HIGH;
      ballsavetime = 5000;
    }
  }

  if (daveoff == 1) {
    if (millis() - 1000 < davetimer) {
      Blinktimer();
      if (ledState == HIGH) {
        leds[16] = CRGB::White; // D
        leds[15] = CRGB::White; // A
        leds[2] = CRGB::White; // D
        leds[3] = CRGB::White; // D
      }
      if (ledState == LOW) {
        leds[16] = CRGB::Black; // D
        leds[15] = CRGB::Black; // A
        leds[2] = CRGB::Black; // D
        leds[3] = CRGB::Black; // D
      }

    }

    if (millis() - 1000 > davetimer) {
      davearr[1] = 0;
      davearr[2] = 0;
      davearr[3] = 0;
      davearr[4] = 0;
      daveoff = 0;
      wTrig.trackPlayPoly(51);
    }
  }

}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End DAVE Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Gate
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Gate() {

  if (SimDigitalRead(gateSwitch1) == LOW && gatetimesw == 0) {
    wTrig.trackPlayPoly(28);
    if (gatearr[1] == 2) {
      Score(1500, 50);
      wTrig.trackPlayPoly(17);
      wTrig.trackPlayPoly(72);
      Serial.println("Drift");
      delay(20);
    }
    if (gatearr[2] == 2 || gatearr[3] == 2) {
      gatearr[2] = 0;
      gatearr[3] = 0;
    }
    gatearr[1] = 1;
    gatetimesw = 1;
    gatetimer = millis();
    gateamb1 = HIGH;
    gateambtimer = millis();
  }

  if (SimDigitalRead(gateSwitch2) == LOW && gatetimesw == 0) {
    wTrig.trackPlayPoly(28);
    if (gatearr[2] == 2) {
      Score(1500, 50);
      wTrig.trackPlayPoly(17);
      wTrig.trackPlayPoly(72);
      Serial.println("Drift");
      delay(20);
    }
    if (gatearr[1] == 2 || gatearr[3] == 2) {
      gatearr[1] = 0;
      gatearr[3] = 0;
    }
    gatearr[2] = 1;
    gatetimesw = 1;
    gatetimer = millis();
    gateamb2 = HIGH;
    gateambtimer = millis();
  }

  if (SimDigitalRead(gateSwitch3) == LOW && gatetimesw == 0) {
    wTrig.trackPlayPoly(28);
    if (gatearr[3] == 2) {
      Score(1500, 50);
      wTrig.trackPlayPoly(17);
      wTrig.trackPlayPoly(72);
      Serial.println("Drift");
      delay(20);
    }
    if (gatearr[1] == 2 || gatearr[2] == 2) {
      gatearr[1] = 0;
      gatearr[2] = 0;
    }
    gatearr[3] = 1;
    gatetimesw = 1;
    gatetimer = millis();
    gateamb3 = HIGH;
    gateambtimer = millis();
  }

  if (millis() - 1000 > gatetimer) {
    gatetimesw = 0;
    gateamb1 = LOW;
    gateamb2 = LOW;
    gateamb3 = LOW;

  }

  // Gateamb
  if (effect == LOW) {
    if (gateamb1 == HIGH ) {
      leds[56] = CRGB::Orange; // D
      leds[58] = CRGB::Orange; // D
    }
    else if (millis() - 1000 > gateambtimer) {
      leds[56] = CRGB::Green; // D
      leds[58] = CRGB::Green; // D
    }
    if (gateamb2 == HIGH ) {
      leds[56] = CRGB::Orange; // D
      leds[54] = CRGB::Orange; // D
    }
    else if (millis() - 1000 > gateambtimer) {
      leds[56] = CRGB::Green; // D
      leds[54] = CRGB::Green; // D
    }
    if (gateamb3 == HIGH ) {
      leds[54] = CRGB::Orange; // D
      leds[52] = CRGB::Orange; // D
    }
    else if (millis() - 1000 > gateambtimer) {
      leds[54] = CRGB::Green; // D
      leds[52] = CRGB::Green; // D
    }
  }

  if (effect == LOW) {
    if (gatearr[1] == 1) {
      leds[57] = CRGB::White; // D
    }
    if (gatearr[2] == 1) {
      leds[55] = CRGB::White; // A
    }
    if (gatearr[3] == 1) {
      leds[53] = CRGB::White; // D
    }


    if (gatearr[1] == 2) {
      leds[57] = CRGB::Orange; // D
    }
    if (gatearr[2] == 2) {
      leds[55] = CRGB::Orange; // A
    }
    if (gatearr[3] == 2) {
      leds[53] = CRGB::Orange; // D
    }



    if (gatearr[1] == 0) {
      leds[57] = CRGB::Black; // D
    }
    if (gatearr[2] == 0) {
      leds[55] = CRGB::Black; // A
    }
    if (gatearr[3] == 0) {
      leds[53] = CRGB::Black; // D
    }
  }

  if (gateoffsw == 0 && gatearr[1] == 1 && gatearr[2] == 1 && gatearr[3] == 1) {
    gateofftimer = millis();
    gateoffsw = 1;
    bonusx ++;
  }

  if (gateoffsw == 1 && millis() - 1000 < gateofftimer) {
    Blinktimer();
    if (ledState == LOW) {
      leds[57] = CRGB::Black; // D
      leds[55] = CRGB::Black; // A
      leds[53] = CRGB::Black; // D
    }
    if (ledState == HIGH) {
      leds[57] = CRGB::Red; // D
      leds[55] = CRGB::Red; // A
      leds[53] = CRGB::Red; // D
    }

  }




  if (gateoffsw == 1 && millis() - 1000 > gateofftimer) {

    leds[57] = CRGB::Black; // D
    leds[55] = CRGB::Black; // A
    leds[53] = CRGB::Black; // D
    gatearr[1] = 0;
    gatearr[2] = 0;
    gatearr[3] = 0;
    wTrig.trackPlayPoly(4);
    gateoffsw = 0;

  }

  // SHIFT LEFT
  if (gsltimesw == 0 && SimDigitalRead(leftFlipperButton) == LOW) {
    gsltimesw = 1;
    gsltimer = millis();
    gatearr[0] = gatearr[1];
    gatearr[1] = gatearr[2];
    gatearr[2] = gatearr[3];
    gatearr[3] = gatearr[0];
  }
  if (millis() - 200 > gsltimer && SimDigitalRead(leftFlipperButton) == HIGH) {
    gsltimesw = 0;
  }



  // SHIFT RIGHT
  if (gsrtimesw == 0 && SimDigitalRead(rightflipperButton) == LOW) {
    gsrtimesw = 1;
    gsrtimer = millis();
    gatearr[4] = gatearr[3];
    gatearr[3] = gatearr[2];
    gatearr[2] = gatearr[1];
    gatearr[1] = gatearr[4];
  }
  if (millis() - 200 > gsrtimer && SimDigitalRead(rightflipperButton) == HIGH) {
    gsrtimesw = 0;
  }

}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Gate Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// POPs
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void Pops() {
  // POP 1
  if (SimDigitalRead(pop1Switch) == LOW && pop1LogicBool == LOW) {
    if (hurryUp == HIGH) {
      effect = HIGH;
      effectID = 6;
      Score(1000, 10);
    }
    else {
      Score(200, 10);
    }
    popsw1 = HIGH;

    pop1LogicBool = HIGH;
    poptimer1 = millis();
    ufoInactivesw = 1;
    ufoInactiveTimer = millis();
    ballHandlerSkip = 1;
    ballHandlerSkipTimer = millis();

  }
  if (pop1LogicBool == HIGH && millis() - 50 < poptimer1) {
    digitalWrite(pop1Coil, HIGH);
  }
  else {
    digitalWrite(pop1Coil, LOW);
  }

  if (SimDigitalRead(pop1Switch) == HIGH && millis() - 100 > poptimer1) {
    pop1LogicBool = LOW;
  }

  // POP 2
  if (SimDigitalRead(pop2Switch) == LOW && pop2LogicBool == LOW) {
    if (hurryUp == HIGH) {
      effect = HIGH;
      effectID = 6;
      Score(1000, 10);
    }
    else {
      Score(200, 10);
    }
    popsw2 = HIGH;
    pop2LogicBool = HIGH;
    poptimer2 = millis();
    ufoInactivesw = 1;
    ufoInactiveTimer = millis();
    ballHandlerSkip = 1;
    ballHandlerSkipTimer = millis();

  }

  if (pop2LogicBool == HIGH && millis() - 50 < poptimer2) {
    digitalWrite(pop2Coil, HIGH);
  }
  else {
    digitalWrite(pop2Coil, LOW);
  }

  if (SimDigitalRead(pop2Switch) == HIGH && millis() - 100 > poptimer2) {
    pop2LogicBool = LOW;
  }

  // POP 3

  if (SimDigitalRead(pop3Switch) == LOW && pop3LogicBool == LOW) {
    if (hurryUp == HIGH) {
      effect = HIGH;
      effectID = 6;
      Score(1000, 10);
    }
    else {
      Score(200, 10);
    }
    popsw3 = HIGH;
    pop3LogicBool = HIGH;
    poptimer3 = millis();
    ufoInactivesw = 1;
    ufoInactiveTimer = millis();
    ballHandlerSkip = 1;
    ballHandlerSkipTimer = millis();
  }

  if (pop3LogicBool == HIGH && millis() - 50 < poptimer3) {
    digitalWrite(pop3Coil, HIGH);
  }
  else {
    digitalWrite(pop3Coil, LOW);
  }

  if (SimDigitalRead(pop3Switch) == HIGH && millis() - 100 > poptimer3) {
    pop3LogicBool = LOW;
  }


  // Lights

  if (effect == LOW) {

    // Pop blink 1

    if (popsw1 == HIGH && millis() - 1000 < poptimer1) {
      if (ledState == HIGH) {
        leds[64] = CRGB::Red; // Pop1
      }
      else {
        leds[64] = CRGB::Blue; // Pop1

      }
    }
    else {
      popsw1 = LOW;
      leds[64] = CRGB::Blue; // Pop1
    }

    // Pop blink 2

    if (popsw2 == HIGH && millis() - 1000 < poptimer2) {
      if (ledState == HIGH) {
        leds[65] = CRGB::Red; // Pop1
      }
      else {
        leds[65] = CRGB::Blue; // Pop1

      }
    }
    else {
      popsw2 = LOW;
      leds[65] = CRGB::Blue; // Pop1
    }


    // Pop blink 3

    if (popsw3 == HIGH && millis() - 1000 < poptimer3) {
      if (ledState == HIGH) {
        leds[66] = CRGB::Red; // Pop1
      }
      else {
        leds[66] = CRGB::Blue; // Pop1

      }
    }
    else {
      popsw3 = LOW;
      leds[66] = CRGB::Blue; // Pop1
    }
  }

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End POPs Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// BonusX
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void BonusXLed() {
  if (bonusx == 0) {
    leds[6] = CRGB::Black; // x8
    leds[7] = CRGB::Black; // x6
    leds[9] = CRGB::Black; // x4
    leds[10] = CRGB::Black; // x2
  }
  if (bonusx == 1 && bonusx1sw == 0) {
    bonusx1sw = 1;
    bonusxtimer1 = millis();
    wTrig.trackPlayPoly(27);
    Serial.println("Bonus1");
    delay(20);
  }
  if (bonusx == 2 && bonusx2sw == 0) {
    bonusx2sw = 1;
    bonusxtimer2 = millis();
    wTrig.trackPlayPoly(27);
    Serial.println("Bonus2");
    delay(20);

  }
  if (bonusx == 3 && bonusx3sw == 0) {
    bonusx3sw = 1;
    bonusxtimer3 = millis();
    wTrig.trackPlayPoly(27);
    Serial.println("Bonus3");
    delay(20);

  }
  if (bonusx == 4 && bonusx4sw == 0) {
    bonusx4sw = 1;
    bonusxtimer4 = millis();
    wTrig.trackPlayPoly(27);
    Serial.println("Bonus4");
    delay(20);

  }
  if (bonusx > 4) {
    bonusx = 4;
  }




  if (bonusx1sw == 1 && millis() - 1000 < bonusxtimer1) {
    Blinktimer();
    if (ledState == HIGH) {
      leds[6] = CRGB::Black; // x8
      leds[7] = CRGB::Black; // x6
      leds[9] = CRGB::Black; // x4
      leds[10] = CRGB::White; // x2
    }
    if (ledState == LOW) {
      leds[6] = CRGB::Black; // x8
      leds[7] = CRGB::Black; // x6
      leds[9] = CRGB::Black; // x4
      leds[10] = CRGB::Black; // x2
    }

  }
  if (effect == LOW) {
    if (bonusx1sw == 1 && millis() - 1000 > bonusxtimer1) {
      leds[6] = CRGB::Black; // x8
      leds[7] = CRGB::Black; // x6
      leds[9] = CRGB::Black; // x4
      leds[10] = CRGB::White; // x2
    }
  }


  if (bonusx2sw == 1 && millis() - 1000 < bonusxtimer2) {
    Blinktimer();
    if (ledState == HIGH) {
      leds[6] = CRGB::Black; // x8
      leds[7] = CRGB::Black; // x6
      leds[9] = CRGB::White; // x4
      leds[10] = CRGB::White; // x2
    }
    if (ledState == LOW) {
      leds[6] = CRGB::Black; // x8
      leds[7] = CRGB::Black; // x6
      leds[9] = CRGB::Black; // x4
      leds[10] = CRGB::Black; // x2
    }

  }
  if (effect == LOW) {
    if (bonusx2sw == 1 && millis() - 1000 > bonusxtimer2) {
      leds[6] = CRGB::Black; // x8
      leds[7] = CRGB::Black; // x6
      leds[9] = CRGB::White; // x4
      leds[10] = CRGB::White; // x2
    }
  }

  if (bonusx3sw == 1 && millis() - 1000 < bonusxtimer3) {
    Blinktimer();
    if (ledState == HIGH) {
      leds[6] = CRGB::Black; // x8
      leds[7] = CRGB::White; // x6
      leds[9] = CRGB::White; // x4
      leds[10] = CRGB::White; // x2
    }
    if (ledState == LOW) {
      leds[6] = CRGB::Black; // x8
      leds[7] = CRGB::Black; // x6
      leds[9] = CRGB::Black; // x4
      leds[10] = CRGB::Black; // x2
    }

  }

  if (effect == LOW) {
    if (bonusx3sw == 1 && millis() - 1000 > bonusxtimer3) {
      leds[6] = CRGB::Black; // x8
      leds[7] = CRGB::White; // x6
      leds[9] = CRGB::White; // x4
      leds[10] = CRGB::White; // x2
    }
  }

  if (bonusx4sw == 1 && millis() - 1000 < bonusxtimer4) {
    Blinktimer();
    if (ledState == HIGH) {
      leds[6] = CRGB::White; // x8
      leds[7] = CRGB::White; // x6
      leds[9] = CRGB::White; // x4
      leds[10] = CRGB::White; // x2
    }
    if (ledState == LOW) {
      leds[6] = CRGB::Black; // x8
      leds[7] = CRGB::Black; // x6
      leds[9] = CRGB::Black; // x4
      leds[10] = CRGB::Black; // x2
    }

  }

  if (effect == LOW) {
    if (bonusx4sw == 1 && millis() - 1000 > bonusxtimer4) {
      leds[6] = CRGB::White; // x8
      leds[7] = CRGB::White; // x6
      leds[9] = CRGB::White; // x4
      leds[10] = CRGB::White; // x2
    }
  }

}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End BonusX Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Weedspinner
/////////////////////////////////////////////////
/////////////////////////////////////////////////

void Weedspinner() {
  if (weedspsw == 0 && SimDigitalRead(spinnerSwitch) == LOW) {
    weedspsw = 1;
    if (multiball != 0) {
      Score(1000, 10);
    }
    else if (hurryUp == HIGH)
    {
      Score(2500, 10);
      effect = HIGH;
      effectID = 6;
    }
    else {
      Score(0, 50);
    }
    wTrig.trackPlayPoly(3);
    if (spinnersw == 1) {
      weedtableindicator = HIGH;
      weedtableindicatortimer = millis();

      if (weedm[player] == 0) {
        weedmeter[player] = weedmeter[player] - 25;
        if (weedmeter[player] <= 0) {
          weedmeter[player] = 180;
          weedm[player] = 1;
          BIP = 2;
          Score(10000, 500);
          Serial.println("Multiball1");
          delay(20);
          multiball = 1;
          ballsavetimer = millis() + 30000;
          ballsaversw = HIGH;
          ballsavetime = 30000;
          ufosw = 0;
          effect = HIGH;
          effectID = 2;
          wTrig.trackPause(1);
          wTrig.trackPlayPoly(69);
          wTrig.trackLoop(89, 1);
          wTrig.trackPlayPoly(89);
          spinnersw = 2;
          multiloopsw = 1;
          BrdgLowActive = HIGH;
          BrdgHighActive = HIGH;
        }
      }

      if (weedm[player] == 1) {
        weedmeter[player] = weedmeter[player] - 15;
        if (weedmeter[player] <= 0) {
          weedmeter[player] = 180;
          weedm[player] = 2;
          Score(20000, 1000);
          BIP = 3;
          Serial.println("Multiball2");
          delay(20);
          multiball = 2;
          ballsavetimer = millis() + 30000;
          ballsaversw = HIGH;
          ballsavetime = 30000;
          ufosw = 0;
          effect = HIGH;
          effectID = 2;
          wTrig.trackPause(1);
          wTrig.trackPlayPoly(71);
          wTrig.trackLoop(88, 1);
          wTrig.trackPlayPoly(88);
          spinnersw = 2;
          multiloopsw = 1;
          BrdgLowActive = HIGH;
          BrdgHighActive = HIGH;
        }
      }

      if (weedm[player] == 2) {
        weedmeter[player] = weedmeter[player] - 10;
        if (weedmeter[player] <= 0) {
          weedmeter[player] = 180;
          weedm[player] = 3;
          BIP = 4;
          Score(30000, 1500);
          Serial.println("Multiball3");
          delay(20);
          multiball = 3;
          ballsavetimer = millis() + 30000;
          ballsaversw = HIGH;
          ballsavetime = 30000;
          ufosw = 0;
          effect = HIGH;
          effectID = 2;
          wTrig.trackPause(1);
          wTrig.trackPlayPoly(64);
          wTrig.trackLoop(64, 1);
          wTrig.trackPlayPoly(68);
          spinnersw = 2;
          multiloopsw = 1;
          BrdgLowActive = HIGH;
          BrdgHighActive = HIGH;
        }
      }

      if (weedm[player] == 3) {
        weedmeter[player] = weedmeter[player] - 8;
        if (weedmeter[player] <= 0) {
          weedmeter[player] = 180;
          weedm[player] = 4;
          Serial.println("Multiball4");
          delay(20);
          BIP = 5;
          Score(40000, 2000);
          multiball = 4;
          ballsavetimer = millis() + 30000;
          ballsaversw = HIGH;
          ballsavetime = 30000;
          ufosw = 0;
          effect = HIGH;
          effectID = 2;
          wTrig.trackPause(1);
          wTrig.trackPlayPoly(65);
          wTrig.trackLoop(65, 1);
          wTrig.trackPlayPoly(70);
          spinnersw = 2;
          multiloopsw = 1;
          BrdgLowActive = HIGH;
          BrdgHighActive = HIGH;
        }
      }

      weedmetersend();
      if (cigar < 250) {
        cigar = cigar + 10;
        cigartime = millis();
      }

    }
  }
  if (weedspsw == 1 && SimDigitalRead(spinnerSwitch) == HIGH) {
    weedspsw = 0;
  }
  if (spinnersw == 1) {
    Blinktimer();
    if (ledState ==  HIGH) {
      leds[27] = CRGB::Green; // Get High
      leds[28] = CRGB::Orange; // Get high
    }
    if (ledState ==  LOW) {
      leds[27] = CRGB::Orange; // Get High
      leds[28] = CRGB::Green; // Get high
    }
  }

  if (spinnersw == 2) {
    Blinktimer();
    if (ledState ==  HIGH) {
      leds[27] = CRGB::Red; // Get High
      leds[28] = CRGB::Black; // Get high
    }
    if (ledState ==  LOW) {
      leds[27] = CRGB::Black; // Get High
      leds[28] = CRGB::Pink; // Get high
    }
  }

  if (spinnersw == 0) {
    leds[27] = CRGB::Black; // Get High
    leds[28] = CRGB::Black; // Get high
  }

  if (cigar > 0 && cigarState == HIGH && cigartime + 1000 < millis()) {
    cigar = cigar - 5;
  }
  analogWrite(a8, cigar);

  if (weedtableindicator == HIGH && millis() - 1000 < weedtableindicatortimer) {
    if (ledState == HIGH) {
      leds[61] = CRGB::Red; // Hightable flasher

    }
    else {
      leds[61] = CRGB::Green; // Hightable flasher

    }
  }
  else {
    leds[61] = CRGB::Gray; // Hightable flasher

  }

  if (weedm[player] == 1) {
    leds[59] = CRGB::Gray; // Michoakan
    leds[60] = CRGB::Green; // Acapulco Gold
    leds[62] = CRGB::Gray; // Thai stick
    leds[63] = CRGB::Gray; // Labrador
  }
  if (weedm[player] == 2) {
    leds[59] = CRGB::Green; // Michoakan
    leds[60] = CRGB::Green; // Acapulco Gold
    leds[62] = CRGB::Gray; // Thai stick
    leds[63] = CRGB::Gray; // Labrador
  }
  if (weedm[player] == 3) {
    leds[59] = CRGB::Green; // Michoakan
    leds[60] = CRGB::Green; // Acapulco Gold
    leds[62] = CRGB::Green; // Thai stick
    leds[63] = CRGB::Gray; // Labrador
  }
  if (weedm[player] == 4) {
    leds[59] = CRGB::Green; // Michoakan
    leds[60] = CRGB::Green; // Acapulco Gold
    leds[62] = CRGB::Green; // Thai stick
    leds[63] = CRGB::Green; // Labrador
  }


}
void weedmetersend() {
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write("a");        // sends five bytes
  Wire.write(weedmeter[player]);              // sends one byte
  Wire.endTransmission();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Weedspinner Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// UFO
/////////////////////////////////////////////////
/////////////////////////////////////////////////


void UFOO() {

  if (ufoInactivesw == 0) {
    ufoanalog = SimAnalogRead(PIN_A5);
    //Serial.println(ert);// put your main code here, to run repeatedly:
  }

  if (ufoInactivesw == 1 && millis() - 100 > ufoInactiveTimer) {
    ufoInactivesw = 0;
  }

  if (ufoanalog < 100 && ufoshoot == 0) {
    if (ufosw == 1 && multiball == 0) {
      wTrig.trackPause(1);
      wTrig.trackPlayPoly(45);
      fasz = 0;
      SetupPurpleAndGreenPalette();
      ufoshoot = 4;
      if (numofplayers == 1) {
        lottery = random(1, 8); // 1..7
      }
      else {
        lottery = random(1, 9); // 1..8, a 8-as a pontlopas (csak tobbjatekosnal!)
      }
#ifdef SIM_MODE
      if (simForceLottery > 0) { // probapadi "cinkelt kocka" (f_sim_mode)
        lottery = simForceLottery;
        simForceLottery = 0;
      }
#endif
      if (lottery  == 1) {
        extraball += 1;
        Serial.println("Ufo5");
        delay(20);
      }

      if (lottery  == 7) {
        Score(40000, 2000);
        Serial.println("Ufo9");
        delay(20);
      }

      if (lottery  == 2) {
        Score(5000, 100);
        Serial.println("Ufo7");
        delay(20);
      }

      if (lottery  == 3) {
        Score(15000, 100);
        Serial.println("Ufo1");
        delay(20);
      }

      if (lottery  == 4) {
        Score(20000, 150);
        Serial.println("Ufo2");
        delay(20);
      }

      if (lottery  == 5) {
        Score(30000, 250);
        Serial.println("Ufo4");
        delay(20);
      }

      if (lottery  == 6) {
        Score(25000, 250);
        Serial.println("Ufo3");
        delay(20);
      }

      if (lottery  == 8) {
        // PONTLOPAS: -10000 pont egy veletlen MASIK jatekosnak.
        // Aldozat: a sajat sorszamunkhoz kepest 1..(numofplayers-1)-gyel
        // eltolt jatekos (korbeforgatva) -> soha nem onmagunk.
        ufoMinus = ((player - 1 + random(1, numofplayers)) % numofplayers) + 1;
        if (score[ufoMinus] >= 10000) {
          score[ufoMinus] = score[ufoMinus] - 10000;
        }
        else {
          score[ufoMinus] = 0; // alulcsordulas-vedelem (unsigned!)
        }
        Serial.print("Ufo");
        Serial.println(9 + ufoMinus); // Ufo10..Ufo13 = melyik jatekost raboltuk ki
        delay(20);
      }

    }

    if (ufosw == 0 && multiball == 0 && hurryUp == 0) {
      ufoshoot = random(1, 4);
      if (ufoshoot == 1) {
        wTrig.trackPause(1);
        wTrig.trackPlayPoly(18);
        wTrig.trackPlayPoly(38);
        Serial.println("Ufo6");
        delay(20);
      }
      if (ufoshoot == 2) {
        wTrig.trackPause(1);
        wTrig.trackPlayPoly(43);
      }
      if (ufoshoot == 3) {
        wTrig.trackPause(1);
        wTrig.trackPlayPoly(44);
      }
    }

    if (ufosw == 0 && multiball != 0) {
      ufoshoot = 5;
    }
    if (ufosw == 0 && hurryUp == HIGH) {
      ufoshoot = 5;
    }

    ufoshoottimer = millis();
    ufoshoottimer2 = millis();
  }

  if (ufoshoot == 1 && ufoshoottimer2 < millis() - 4300) {
    digitalWrite(ufoCoil, HIGH);
    if (millis() - 50 > ufoshoottimer + 4300) {
      digitalWrite(ufoCoil, LOW);
      if (millis() - 500 > ufoshoottimer + 4300) {
        ufoshoot = 0;
        wTrig.trackPause(18);
        if (multiball == 0) {
          wTrig.trackResume(1);
        }
        wTrig.trackPlayPoly(42);
        Score(0, 300);
      }
    }
  }

  if (ufoshoot == 2 && ufoshoottimer2 < millis() - 2000) {
    digitalWrite(ufoCoil, HIGH);
    if (millis() - 50 > ufoshoottimer + 2000) {
      digitalWrite(ufoCoil, LOW);
      if (millis() - 500 > ufoshoottimer + 2000) {
        ufoshoot = 0;
        if (multiball == 0) {
          wTrig.trackResume(1);
        }
        wTrig.trackPlayPoly(42);
        Score(0, 300);
      }
    }
  }

  if (ufoshoot == 3 && ufoshoottimer2 < millis() - 1500) {
    digitalWrite(ufoCoil, HIGH);
    if (millis() - 50 > ufoshoottimer + 1500) {
      digitalWrite(ufoCoil, LOW);
      if (millis() - 500 > ufoshoottimer + 1500) {
        ufoshoot = 0;
        if (multiball == 0) {
          wTrig.trackResume(1);
        }
        wTrig.trackPlayPoly(42);
        Score(0, 300);
      }
    }
  }

  //// Itt ad is az ufo valamit

  if (ufoshoot == 4 && ufoshoottimer2 < millis() - 5500) {
    digitalWrite(ufoCoil, HIGH);
    if (millis() - 50 > ufoshoottimer + 5500) {
      digitalWrite(ufoCoil, LOW);
      if (millis() - 700 > ufoshoottimer + 4000) {
        if (lottery  == 1) {    /// ExtraBall
          wTrig.trackPlayPoly(91);
          wTrig.trackPlayPoly(74);
        }
        if (lottery  == 2) {    /// HurryUp
          wTrig.trackPlayPoly(91);
          wTrig.trackPlayPoly(101);
          hurryUp = HIGH;
          hurryUpTimer = millis();
          spinnersw = 2;
          ufosw = 0;
        }
        if (lottery  == 3) {    /// 15000
          wTrig.trackPlayPoly(91);
          wTrig.trackPlayPoly(75);
        }
        if (lottery  == 4) {    /// 20000
          wTrig.trackPlayPoly(91);
          wTrig.trackPlayPoly(76);
        }
        if (lottery  == 6) {    /// 25000
          wTrig.trackPlayPoly(91);
          wTrig.trackPlayPoly(94);
        }
        if (lottery  == 5) {    /// 30000
          wTrig.trackPlayPoly(91);
          wTrig.trackPlayPoly(78);
        }
        if (lottery  == 7) {    /// SpaceCoke Multi
          BIP = 5;
          multiball = 5;
          effect = HIGH;
          effectID = 4;
          wTrig.trackPlayPoly(77);
          wTrig.trackPlayPoly(91);
          wTrig.trackPlayPoly(109);
          wTrig.trackPlayPoly(110);
          ufosw = 0;
          spinnersw = 2;
          BrdgLowActive = HIGH;
          BrdgHighActive = HIGH;
        }
        if (lottery  == 8) {    /// Pontlopas (Ufo10..13)
          wTrig.trackPlayPoly(91); // Firework
          wTrig.trackPlayPoly(123 + ufoMinus); // 124..127 = kirabolt jatekos hangja
          ufoMinus = 0;
        }


        fasz = 68;
        initlight = 1;
        Initlights();
        ballsavetimer = millis() + 30000;
        ballsaversw = HIGH;
        if (lottery == 7) {
          ballsavetime = 30000;
        }
        else {
          ballsavetime = 10000;
        }
        ufoshoot = 0;

        if (lottery == 2) {
          wTrig.trackPlayPoly(108);
          wTrig.trackPlayPoly(42);
        }
        if (lottery == 1 || lottery == 3 || lottery == 4 || lottery == 5 || lottery == 6 || lottery == 8)
        {
          wTrig.trackResume(1);
          wTrig.trackPlayPoly(42);
        }
        ufosw = 0;
      }
    }
  }

  if (ufoshoot == 5 && ufoshoottimer2 < millis() - 1500) {
    digitalWrite(ufoCoil, HIGH);
    if (millis() - 50 > ufoshoottimer + 1500) {
      digitalWrite(ufoCoil, LOW);
      if (millis() - 500 > ufoshoottimer + 1500) {
        ufoshoot = 0;
        Score(0, 300);
      }
    }
  }

  if (ufosw == 1) {
    Blinktimer();
    if (ledState == HIGH) {
      leds[42] = CRGB::Green; // C
      leds[43] = CRGB::Black; // C
    }
    if (ledState == LOW) {
      leds[42] = CRGB::Black; // C
      leds[43] = CRGB::Green; // C
    }

  }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End UFO Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Cheech & Chong
/////////////////////////////////////////////////
/////////////////////////////////////////////////



/// 
///  Inactive state
///
void Chong_switch() {
  if (SimDigitalRead(chongSwitch) == LOW && chongoffsw == LOW) {
    chongoffsw = HIGH;
    chongoffswtimer = millis();
    chongspeech = random(1, 11);
    switch (chongspeech) {
      case 1:
        wTrig.trackPlayPoly(8);
        break;
      case 2:
        wTrig.trackPlayPoly(9);
        break;
      case 3:
        wTrig.trackPlayPoly(52);
        break;
      case 4:
        wTrig.trackPlayPoly(53);
        break;
      case 5:
        wTrig.trackPlayPoly(9);
        break;
      case 6:
        wTrig.trackPlayPoly(79);
        break;
      case 7:
        wTrig.trackPlayPoly(80);
        break;
      case 8:
        wTrig.trackPlayPoly(82);
        break;
      case 9:
        wTrig.trackPlayPoly(83);
        break;
      case 10:
        wTrig.trackPlayPoly(85);
        break;
    }
    
    /// 
    /// Active state
    ///
    if (chongLightActiveSw == HIGH) {
      CollectTimer = millis();
      CollectSw = LOW;
      chongLightActiveSw = LOW;
      cheechLightActiveSw = LOW;
      chongCollectives[player] += 1;
      if (chongCollectives[player] == 1) {
          Serial.println("ChongC1");
          delay(20);
          Score(10000, 500);
          wTrig.trackPlayPoly(123);
          wTrig.trackPlayPoly(120);
      }
      if (chongCollectives[player] == 2) {
          Serial.println("ChongC2");
          delay(20);
          Score(15000, 1000);
          wTrig.trackPlayPoly(123);
          wTrig.trackPlayPoly(121);
      }
      if (chongCollectives[player] == 3) {
          Serial.println("ChongC3");
          delay(20);
          Score(20000, 2000);
          wTrig.trackPlayPoly(123);
          wTrig.trackPlayPoly(122);
          chongCollectives[player] = 0;
      }
    }
    else {
      if (hurryUp == HIGH) {
        effect = HIGH;
        effectID = 6;
        Score(2500, 500);
        Serial.println("Point1");
        delay(20);
      }
      else {
        Score(200, 50);
      }
    }
  }

  if (millis() - 1000 > chongoffswtimer) {
    chongoffsw = LOW;
  }

  if (chongLightActiveSw == HIGH) {
      Blinktimer();
      if (ledState == HIGH) {
      leds[26] = CRGB::Red; // C
    }
    if (ledState == LOW) {
      leds[26] = CRGB::Yellow; // C
    }
  }
  if (chongLightActiveSw == LOW) {
          leds[26] = CRGB::Black; // C
          leds[33] = CRGB::Black; // C
  }

}




void Cheech_switch() {
    /// 
    ///  Inactive state
    ///

    if (SimDigitalRead(cheechSwitch) == LOW && cheechoffsw == LOW) {
    cheechoffsw = HIGH;
    cheechoffswtimer = millis();
    cheechspeech = random(1, 16); // 1..15, a switch mind a 15 hangjat lefedi
    switch (cheechspeech) {
      case 1:
        wTrig.trackPlayPoly(7);
        break;
      case 2:
        wTrig.trackPlayPoly(35);
        break;
      case 3:
        wTrig.trackPlayPoly(36);
        break;
      case 4:
        wTrig.trackPlayPoly(37);
        break;
      case 5:
        wTrig.trackPlayPoly(48);
        break;
      case 6:
        wTrig.trackPlayPoly(49);
        break;
      case 7:
        wTrig.trackPlayPoly(50);
        break;
      case 8:
        wTrig.trackPlayPoly(54);
        break;
      case 9:
        wTrig.trackPlayPoly(55);
        break;
      case 10:
        wTrig.trackPlayPoly(56);
        break;
      case 11:
        wTrig.trackPlayPoly(57);
        break;
      case 12:
        wTrig.trackPlayPoly(58);
        break;
      case 13:
        wTrig.trackPlayPoly(59);
        break;
      case 14:
        wTrig.trackPlayPoly(81);
        break;
      case 15:
        wTrig.trackPlayPoly(87);
        break;
    }

    /// 
    ///  Active state
    ///

    if (cheechLightActiveSw == HIGH) {
      chongLightActiveSw = LOW;
      cheechLightActiveSw = LOW;
      CollectTimer = millis();
      CollectSw = 1;
      cheechCollectives[player] += 1;
      if (cheechCollectives[player] == 1) {
          Serial.println("CheechC1");
          delay(20);
          Score(10000, 500);
          wTrig.trackPlayPoly(123);
          wTrig.trackPlayPoly(117);

      }
      if (cheechCollectives[player] == 2) {
          Serial.println("CheechC2");
          delay(20);
          Score(15000, 1000);
          wTrig.trackPlayPoly(123);
          wTrig.trackPlayPoly(118);
      }
      if (cheechCollectives[player] == 3) {
          Serial.println("CheechC3");
          delay(20);
          Score(20000, 2000);
          wTrig.trackPlayPoly(123);
          wTrig.trackPlayPoly(119);
          cheechCollectives[player] = 0;

      }
    }
    else {
      if (hurryUp == HIGH) {
        effect = HIGH;
        effectID = 6;
        Score(2500, 500);
        Serial.println("Point1");
        delay(20);
      }
      else {
        Score(200, 50);
      }
    }
  }

  if (millis() - 1000 > cheechoffswtimer) {
    cheechoffsw = LOW;
  }

  if (cheechLightActiveSw == HIGH) {
    Blinktimer();
    if (ledState == HIGH) {
      leds[33] = CRGB::Yellow; // C
    }
    if (ledState == LOW) {
      leds[33] = CRGB::Red; // C
    }
  }
  if (cheechLightActiveSw == LOW) {
      leds[26] = CRGB::Black; // C
      leds[33] = CRGB::Black; // C
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Cheech && Chong Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Cheech && Chong Collectives
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Collectives() {
  if (cheechCollectives[player] == 0) {
    leds[38] = CRGB::Orange; // Cheech wheel
    leds[41] = CRGB::Orange; // Cheech cigar
    leds[46] = CRGB::Orange; // MUF dvr
  }
  if (chongCollectives[player] == 0) {
    leds[39] = CRGB::Orange; // Chong Pipe
    leds[40] = CRGB::Orange; // Chong Cockroach
    leds[47] = CRGB::Orange; // Chong plunger key
  }
  if (cheechCollectives[player] == 1) {
    leds[38] = CRGB::White; // Cheech wheel
    leds[41] = CRGB::Orange; // Cheech cigar
    leds[46] = CRGB::Orange; // MUF dvr
  }
  if (chongCollectives[player] == 1) {
    leds[39] = CRGB::White; // Chong Pipe
    leds[40] = CRGB::Orange; // Chong Cockroach
    leds[47] = CRGB::Orange; // Chong plunger key
  }
  if (cheechCollectives[player] == 2) {
    leds[38] = CRGB::White; // Cheech wheel
    leds[41] = CRGB::White; // Cheech cigar
    leds[46] = CRGB::Orange; // MUF dvr
  }
  if (chongCollectives[player] == 2) {
    leds[39] = CRGB::White; // Chong Pipe
    leds[40] = CRGB::White; // Chong Cockroach
    leds[47] = CRGB::Orange; // Chong plunger key
  }
  if (cheechCollectives[player] == 3) {
    leds[38] = CRGB::White; // Cheech wheel
    leds[41] = CRGB::White; // Cheech cigar
    leds[46] = CRGB::White; // MUF dvr
  }
  if (chongCollectives[player] == 3) {
    leds[39] = CRGB::White; // Chong Pipe
    leds[40] = CRGB::White; // Chong Cockroach
    leds[47] = CRGB::White; // Chong plunger key
  }

  if (CollectSw == 1 && millis() - 1000 < CollectTimer) {
    if (ledState == HIGH) {
      leds[38] = CRGB::White; // Cheech wheel
      leds[41] = CRGB::White; // Cheech cigar
      leds[46] = CRGB::White; // MUF dvr
      leds[39] = CRGB::White; // Chong Pipe
      leds[40] = CRGB::White; // Chong Cockroach
      leds[47] = CRGB::White; // Chong plunger key
    }
    if (ledState == LOW) {
      leds[38] = CRGB::Yellow; // Cheech wheel
      leds[41] = CRGB::Yellow; // Cheech cigar
      leds[46] = CRGB::Yellow; // MUF dvr
      leds[39] = CRGB::Yellow; // Chong Pipe
      leds[40] = CRGB::Yellow; // Chong Cockroach
      leds[47] = CRGB::Yellow; // Chong plunger key
    }
  }
  else {
    CollectSw = 0;
  }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Cheech && Chong Collectives rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// GIFT
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Gift() {
  if (giftsw == 0) {
    switch (randGift) {
      case 1:
        cncswitch1 = 0;
        break;
      case 2:
        cncswitch2 = 0;
        break;
      case 3:
        cncswitch3 = 0;
        break;
      case 4:
        weedswitch1 = 0;
        break;
      case 5:
        weedswitch2 = 0;
        break;
      case 6:
        weedswitch3 = 0;
        break;
      case 7:
        weedswitch4 = 0;
        break;
      case 8:
        fishTankLightState1 = 0;
        break;
      case 9:
        fishTankLightState2 = 0;
        break;
    }
  }
  giftsw = 3;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End GIFT Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Bridge LOW
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void BridgeLow() {
  if (BrdgLowActive == LOW) {
    leds[24] = CRGB::Black; // Left ramp
    leds[25] = CRGB::Black; // Left ramp

    if (SimDigitalRead(bridgeLowSwitch) == LOW && BrdgLowSw == 0) {
      BrdgLowSw = 1;
      BrdgLowT = millis();

      if (hurryUp == HIGH) {
        Score(5000, 500);
        Serial.println("Point2");
        delay(20);
      }
      else {

        if (millis() - 4000 > comboTimerH) {
          comboCounter = 0;
          Score(500, 50);
          wTrig.trackPlayPoly(9);
        }

        if (millis() - 4000 < comboTimerH) { /// Ha még nem telt el 4 másodperc a kishíd óta
          comboCounter ++; // kombók száma
          if (comboCounter > 6) {
            comboCounter = 6;
          }
          if (comboCounter == 1) {
            Score(2500, 300);
            Serial.println("Combo1");
            wTrig.trackPlayPoly(95);
          }
          if (comboCounter == 2) {
            Score(5000, 300);
            Serial.println("Combo2");
            wTrig.trackPlayPoly(96);
          }
          if (comboCounter == 3) {
            Score(7500, 300);
            Serial.println("Combo3");
            wTrig.trackPlayPoly(95);
          }
          if (comboCounter == 4) {
            Score(10000, 300);
            Serial.println("Combo4");
            wTrig.trackPlayPoly(95);
          }
          if (comboCounter == 5) {
            Score(15000, 300);
            Serial.println("Combo5");
            wTrig.trackPlayPoly(96);
          }
          if (comboCounter == 6) {
            Score(20000, 300);
            Serial.println("Combo6");
            wTrig.trackPlayPoly(95);
          }
        }
        comboTimerL = millis();
      }

    }
    if (BrdgLowSw == 1 && millis() - 1000 < BrdgLowT) {
      if (ledState == LOW) {
        leds[23] = CRGB::Red; // Left ramp ambient
        leds[17] = CRGB::Yellow; // CnC ambient
      }
      if (ledState == HIGH) {
        leds[23] = CRGB::Yellow; // Left ramp ambient
        leds[17] = CRGB::Red; // CnC ambient
      }
    }
    else {
      BrdgLowSw = 0;
      leds[23] = CRGB::White; // Left ramp ambient
      leds[17] = CRGB::White; // CnC ambient
    }
  }

  if (BrdgLowActive == HIGH || multiball != 0) {
    if (effect == LOW) {
      if (ledState == LOW) {
        leds[24] = CRGB::Yellow; // Left ramp
        leds[25] = CRGB::Green; // Left ramp
      }
      if (ledState == HIGH) {
        leds[24] = CRGB::Green; // Left ramp
        leds[25] = CRGB::Yellow; // Left ramp
      }
    }


    if (SimDigitalRead(bridgeLowSwitch) == LOW && BrdgLowSw == 0 && multiball == 0) {
      BrdgLowSw = 1;
      BrdgLowT = millis();
      Score(5000, 200);
      wTrig.trackPlayPoly(92);
      Serial.println("Point2");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }

    if (SimDigitalRead(bridgeLowSwitch) == LOW && BrdgLowSw == 0 && multiball == 1) {
      BrdgLowSw = 1;
      BrdgLowT = millis();
      Score(10000, 200);
      wTrig.trackPlayPoly(73);
      Serial.println("Jackpot2");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }
    if (SimDigitalRead(bridgeLowSwitch) == LOW && BrdgLowSw == 0 && multiball == 2) {
      BrdgLowSw = 1;
      BrdgLowT = millis();
      Score(15000, 200);
      wTrig.trackPlayPoly(73);
      Serial.println("Jackpot3");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }
    if (SimDigitalRead(bridgeLowSwitch) == LOW && BrdgLowSw == 0 && multiball == 3) {
      BrdgLowSw = 1;
      BrdgLowT = millis();
      Score(20000, 200);
      wTrig.trackPlayPoly(73);
      Serial.println("Jackpot4");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }
    if (SimDigitalRead(bridgeLowSwitch) == LOW && BrdgLowSw == 0 && multiball == 4) {
      BrdgLowSw = 1;
      BrdgLowT = millis();
      Score(25000, 200);
      wTrig.trackPlayPoly(73);
      Serial.println("Jackpot5");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }
    if (SimDigitalRead(bridgeLowSwitch) == LOW && BrdgLowSw == 0 && multiball == 5) {
      BrdgLowSw = 1;
      BrdgLowT = millis();
      Score(30000, 200);
      wTrig.trackPlayPoly(73);
      Serial.println("Jackpot6");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }




    if (BrdgLowSw == 1 && millis() > BrdgLowT + 1000) {
      BrdgLowSw = 0;
      if (multiball == 0) {
        BrdgLowActive = LOW;
      }
      leds[24] = CRGB::Black; // Left ramp
      leds[25] = CRGB::Black; // Left ramp
    }
  }


}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Bridge LOW Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Bridge HIGH
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void BridgeHigh() {
  if (BrdgHighActive == LOW) {
    leds[36] = CRGB::Black; // Left ramp
    leds[37] = CRGB::Black; // Left ramp
    if (SimDigitalRead(bridgeHighSwitch) == LOW && BrdgHighSw == 0) {
      BrdgHighSw = 1;
      BrdgHighT = millis();

      if (hurryUp == HIGH) {
        Score(5000, 500);
        Serial.println("Point2");
        delay(20);
      }
      else {

        if (millis() - 4000 > comboTimerL) {
          comboCounter = 0;
          Score(500, 50);
          wTrig.trackPlayPoly(36);
        }

        if (millis() - 4000 < comboTimerL) { /// Ha még nem telt el 4 másodperc a kishíd óta
          comboCounter ++; // kombók száma
          if (comboCounter > 6) {
            comboCounter = 6;
          }
          if (comboCounter == 1) {
            Score(2500, 300);
            Serial.println("Combo1");
            wTrig.trackPlayPoly(95);
          }
          if (comboCounter == 2) {
            Score(5000, 300);
            Serial.println("Combo2");
            wTrig.trackPlayPoly(96);
          }
          if (comboCounter == 3) {
            Score(7500, 300);
            Serial.println("Combo3");
            wTrig.trackPlayPoly(95);
          }
          if (comboCounter == 4) {
            Score(10000, 300);
            Serial.println("Combo4");
            wTrig.trackPlayPoly(95);
          }
          if (comboCounter == 5) {
            Score(15000, 300);
            Serial.println("Combo5");
            wTrig.trackPlayPoly(96);
          }
          if (comboCounter == 6) {
            Score(20000, 300);
            Serial.println("Combo6");
            wTrig.trackPlayPoly(95);
          }

        }
        comboTimerH = millis();
      }


    }

    if (BrdgHighSw == 1 && millis() - 1000 < BrdgHighT) {
      if (ledState == LOW) {
        leds[50] = CRGB::Red; // Left ramp ambient
        leds[51] = CRGB::Yellow; // CnC ambient
      }
      if (ledState == HIGH) {
        leds[50] = CRGB::Yellow; // Left ramp ambient
        leds[51] = CRGB::Red; // CnC ambient
      }
    }
    else {
      BrdgHighSw = 0;
      leds[50] = CRGB::White; // Left ramp ambient
      leds[51] = CRGB::White; // CnC ambient
    }
  }

  if (BrdgHighActive == HIGH || multiball != 0) {
    if (effect == LOW) {
      if (ledState == LOW) {
        leds[36] = CRGB::Yellow; // Left ramp
        leds[37] = CRGB::Green; // Left ramp
      }
      if (ledState == HIGH) {
        leds[36] = CRGB::Green; // Left ramp
        leds[37] = CRGB::Yellow; // Left ramp
      }
    }
    if (SimDigitalRead(bridgeHighSwitch) == LOW && BrdgHighSw == 0 && multiball == 0) {
      BrdgHighSw = 1;
      BrdgHighT = millis();
      Score(5000, 200);
      wTrig.trackPlayPoly(92);
      Serial.println("Point2");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }

    if (SimDigitalRead(bridgeHighSwitch) == LOW && BrdgHighSw == 0 && multiball == 1) {
      BrdgHighSw = 1;
      BrdgHighT = millis();
      Score(10000, 500);
      wTrig.trackPlayPoly(73);
      Serial.println("Jackpot2");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }
    if (SimDigitalRead(bridgeHighSwitch) == LOW && BrdgHighSw == 0 && multiball == 2) {
      BrdgHighSw = 1;
      BrdgHighT = millis();
      Score(15000, 500);
      wTrig.trackPlayPoly(73);
      Serial.println("Jackpot3");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }
    if (SimDigitalRead(bridgeHighSwitch) == LOW && BrdgHighSw == 0 && multiball == 3) {
      BrdgHighSw = 1;
      BrdgHighT = millis();
      Score(20000, 500);
      wTrig.trackPlayPoly(73);
      Serial.println("Jackpot4");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }
    if (SimDigitalRead(bridgeHighSwitch) == LOW && BrdgHighSw == 0 && multiball == 4) {
      BrdgHighSw = 1;
      BrdgHighT = millis();
      Score(20000, 500);
      wTrig.trackPlayPoly(73);
      Serial.println("Jackpot5");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }
    if (SimDigitalRead(bridgeHighSwitch) == LOW && BrdgHighSw == 0 && multiball == 5) {
      BrdgHighSw = 1;
      BrdgHighT = millis();
      Score(20000, 500);
      wTrig.trackPlayPoly(73);
      Serial.println("Jackpot6");
      delay(20);
      effect = HIGH;
      effectID = 5;
    }





    if (BrdgHighSw == 1 && millis() > BrdgHighT + 1000) {
      BrdgHighSw = 0;
      if (multiball == 0) {
        BrdgHighActive = LOW;
      }
      leds[36] = CRGB::Black; // Left ramp
      leds[37] = CRGB::Black; // Left ramp
    }
  }


}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Bridge HIGH Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Add player
/////////////////////////////////////////////////
/////////////////////////////////////////////////
// Az AddPlayer() (start gombbal jatek kozben) megszunt: a jatekosok
// hozzaadasa a player select modban tortenik (intmMode, intmon == 3).
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Add Player Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// HurryUp
/////////////////////////////////////////////////
/////////////////////////////////////////////////


void HurryUp()
{
  if (hurryUp == HIGH && millis() - 80000 < hurryUpTimer)
  {
    if (ledState == LOW) {
      leds[23] = CRGB::White; // Left ramp ambient
      leds[50] = CRGB::White; // Fishtank ambient
      leds[51] = CRGB::Black; // Right ramp ambient
      leds[17] = CRGB::Black; // CnC ambient
    }
    if (ledState == HIGH) {
      leds[23] = CRGB::Black; // Left ramp ambient
      leds[50] = CRGB::Black; // Fishtank ambient
      leds[51] = CRGB::White; // Right ramp ambient
      leds[17] = CRGB::White; // CnC ambient
    }
    if (hurryUpState == LOW) {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = LINEARBLEND;
      fasz = 0;
      initlight = HIGH;
    }
    else {
      fasz = 68;
      Initlights();
      initlight = LOW;
    }
  }
  if (hurryUp == HIGH && millis() - 80000 > hurryUpTimer)
  {
    initlight = HIGH;
    Initlights();
    fasz = 68;
    wTrig.trackPause(108);
    wTrig.trackResume(1);
    hurryUp = LOW;
    spinnersw = HIGH;
  }



}


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End HurrryUp Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Tilt
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Tilt() {
    if (SimDigitalRead(PIN_A12) == 0 && tiltLogicSW == LOW) {
        wTrig.trackPlayPoly(111);
        wTrig.trackPlayPoly(112);
        tiltcounter = tiltcounter + 10;
        tilttimer = millis();
        tiltLogicSW = HIGH;
        tiltLogicTimer = millis();
        Serial.println("Danger");
    }

    if (tiltcounter != 0 && millis() - tilttimer > 1000) {
        tilttimer = millis();
        tiltcounter = tiltcounter - 1;
    }
    if (tiltLogicSW == HIGH && millis() - tiltLogicTimer > 500){
      tiltLogicSW = LOW;
      }
    if (tiltcounter > 15) {
        wTrig.stopAllTracks();
        Serial.println("Tilt");
        wTrig.trackPlayPoly(112);
        wTrig.trackPlayPoly(113);
        wTrig.trackPlayPoly(114);
        for (int i = 0; i < 68; i++) {
                leds[i] = CRGB::Black;
        }
        FastLED.show();
        ballsaversw = LOW;
        digitalWrite(leftFlipperBat, LOW);
        digitalWrite(rightFlipperBat, LOW);
        digitalWrite(rightFlipperBat, LOW);
        digitalWrite(leftSlingshotCoil, LOW);
        digitalWrite(rightSlingshotCoil, LOW);
        digitalWrite(ufoCoil, LOW);
        digitalWrite(pop1Coil, LOW);
        digitalWrite(pop2Coil, LOW);
        digitalWrite(pop3Coil, LOW);
        digitalWrite(shooterlaneCoil, LOW);
        while (BIS != 5) {
            MIV(HIGH);
        }
        tiltcounter = 0; // tilt lekezelve, kulonben a blokk masodpercekig ujra lefutna
    }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Tilt Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// SIM MODE - probapadi szimulator
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//
// Ha a fo fajlban a #define SIM_MODE aktiv, a firmware bemeneteit
// szimulalni lehet, valodi flipperhardver nelkul:
//
//  - SimDigitalRead/SimAnalogRead: ha van szimulalt ertek, azt adja,
//    kulonben a valodi pint olvassa (eles gepen = sima digitalRead)
//  - mini "golyofizika": figyeli a tarolo- es kilovo-tekercs kimenetet,
//    es ugy mozgatja a szimulalt golyokat (tarolo-infrak, kilovosav
//    kapcsolo), ahogy a valosagban tortenne
//  - inditas utan automatikus DEMO JATEK fut le (start, kiloves,
//    celpontok, ballsave-mentes, 3 golyo, game over, nevbevitel),
//    a Pi-s GUI-n elore kovetheto
//  - soros egykarakteres parancsokkal kezzel is lehet "jatszani"
//    (kuldj '?'-et a parancslistaert)
//
// !!! ELES GEPRE FELTOLTES ELOTT A SIM_MODE DEFINE-T KI KELL VENNI !!!
// (Sim modban a tarolo-infrak SZIMULALT erteket adnak, a valodi
// erzekeloket a firmware nem latna!)

#ifdef SIM_MODE

// ---- szimulalt allapotok ----
int8_t simD[70];            // -1 = valodi pin, 0/1 = szimulalt ertek
int simA[16];               // -1 = valodi, egyebkent szimulalt analog ertek
unsigned long simReleaseAt[70]; // gombnyomas-lejaratok
uint8_t troughBalls = 5;    // golyok a taroloban
unsigned long laneLowAt = 0;  // ekkor "erkezik" a golyo a kilovosavba
unsigned long laneHighAt = 0; // ekkor hagyja el a savot
boolean prevTroughCoil = LOW;
boolean prevKickCoil = LOW;
unsigned long simBannerAt = 0;
unsigned long simT0 = 0;
unsigned int simScriptIx = 0;

// ---- demo forgatokonyv ----
#define SIMCMD_PRESS 0
#define SIMCMD_DRAIN 1
struct SimEvent { uint32_t t; uint8_t cmd; uint8_t pin; uint16_t ms; };
const SimEvent simScript[] = {
  {  2000, SIMCMD_PRESS, 47, 200 }, // Start gomb -> jatek indul
  {  5000, SIMCMD_PRESS, 51, 400 }, // kilovo gomb -> golyo jatekban
  {  7000, SIMCMD_PRESS, 11, 150 }, // weed celpontok 1..4
  {  7600, SIMCMD_PRESS, 10, 150 },
  {  8200, SIMCMD_PRESS,  9, 150 },
  {  8800, SIMCMD_PRESS,  8, 150 }, // -> Weedblast effekt
  { 10000, SIMCMD_PRESS,  4,  80 }, // jobb slingshot
  { 11000, SIMCMD_PRESS, 38, 100 }, // spinner-porgetes
  { 11300, SIMCMD_PRESS, 38, 100 },
  { 11600, SIMCMD_PRESS, 38, 100 },
  { 11900, SIMCMD_PRESS, 38, 100 },
  { 12200, SIMCMD_PRESS, 38, 100 },
  { 14000, SIMCMD_PRESS, 34, 100 }, // pop bumper 1
  { 15000, SIMCMD_PRESS, 12, 150 }, // Cheech
  { 16000, SIMCMD_PRESS,  7, 150 }, // Chong
  { 18000, SIMCMD_PRESS, 45, 120 }, // gate 1..3 -> bonus X
  { 18600, SIMCMD_PRESS, 44, 120 },
  { 19200, SIMCMD_PRESS, 40, 120 },
  { 19800, SIMCMD_DRAIN,  0,   0 }, // lefolyas ballsave alatt -> mentes!
  { 27000, SIMCMD_DRAIN,  0,   0 }, // 1. golyo vege -> "Next"
  { 34000, SIMCMD_PRESS, 51, 400 }, // 2. golyo kilovese
  { 36000, SIMCMD_PRESS, 38, 100 },
  { 36300, SIMCMD_PRESS, 38, 100 },
  { 36600, SIMCMD_PRESS, 38, 100 },
  { 52000, SIMCMD_DRAIN,  0,   0 }, // 2. golyo vege
  { 59000, SIMCMD_PRESS, 51, 400 }, // 3. golyo kilovese
  { 61000, SIMCMD_PRESS, 31,  80 }, // bal slingshot
  { 76000, SIMCMD_DRAIN,  0,   0 }, // 3. golyo vege -> "End", nevbevitel
  { 80000, SIMCMD_PRESS, 27, 200 }, // nevbevitel demo: bal flipper
  { 82000, SIMCMD_PRESS, 26, 200 }, // jobb flipper
  { 84000, SIMCMD_PRESS, 51, 200 }, // kilovo gomb = valasztas
};
#define SIM_SCRIPT_LEN (sizeof(simScript) / sizeof(simScript[0]))

void SimUpdateTrough() {
  for (uint8_t i = 0; i < 5; i++) {
    simA[i] = (i < troughBalls) ? 1000 : 400; // >988 = golyo ott van
  }
}

void SimPress(uint8_t pin, uint16_t ms) {
  simD[pin] = 0; // INPUT_PULLUP logika: LOW = benyomva
  simReleaseAt[pin] = millis() + ms;
  Serial.print("SIM: press pin ");
  Serial.println(pin);
}

void SimDrain() {
  if (troughBalls < 5) {
    troughBalls++;
  }
  SimUpdateTrough();
  Serial.println("SIM: drain");
}

void SimInit() {
  for (uint8_t i = 0; i < 70; i++) {
    simD[i] = -1;
    simReleaseAt[i] = 0;
  }
  for (uint8_t i = 0; i < 16; i++) {
    simA[i] = -1;
  }
  troughBalls = 5;
  SimUpdateTrough();
  simA[5] = 500;  // UFO infra: nincs golyo benne (<100 lenne a golyo)
  simD[13] = 1;   // kilovosav: ures
  simT0 = millis();
  simScriptIx = 0;
  Serial.println("SIM MODE AKTIV - demo jatek indul (parancsok: '?')");
}

int SimDigitalRead(uint8_t pin) {
  if (pin < 70 && simD[pin] >= 0) {
    return simD[pin];
  }
  return digitalRead(pin);
}

int SimAnalogRead(uint8_t pin) {
  uint8_t ch = (pin >= 54) ? pin - 54 : pin; // PIN_A0..A15 vagy csatorna
  if (ch < 16 && simA[ch] >= 0) {
    return simA[ch];
  }
  return analogRead(pin);
}

void SimHelp() {
  Serial.println("SIM parancsok: S=start K=kiloves D=lefolyas R=demo ujra");
  Serial.println(" 1-4=weed c=cheech h=chong o=pop1 n=spinner l/L=loop");
  Serial.println(" a/b/e=gate1-3 s=jobb sling f/j=bal/jobb flipper x/y=hidak");
}

void SimPoll() {
  unsigned long now = millis();

  // -- gombnyomasok lejarata --
  for (uint8_t i = 0; i < 70; i++) {
    if (simD[i] == 0 && simReleaseAt[i] != 0 && now > simReleaseAt[i]) {
      simD[i] = -1;
      simReleaseAt[i] = 0;
    }
  }

  // -- mini golyofizika: tekercs-kimenetek figyelese --
  boolean troughCoil = digitalRead(ballTroughCoil);
  if (troughCoil == HIGH && prevTroughCoil == LOW && troughBalls > 0) {
    troughBalls--;                 // egy golyo elhagyja a tarolot
    SimUpdateTrough();
    laneLowAt = now + 800;         // ~0.8 mp mulva er a kilovosavba
    Serial.println("SIM: golyo kilokve a tarolobol");
  }
  prevTroughCoil = troughCoil;

  boolean kickCoil = digitalRead(shooterlaneCoil);
  if (kickCoil == HIGH && prevKickCoil == LOW) {
    laneHighAt = now + 150;        // a kicker kilovi a savbol
    laneLowAt = 0;
  }
  prevKickCoil = kickCoil;

  if (laneLowAt != 0 && now > laneLowAt) {
    simD[13] = 0;                  // golyo a kilovosavban
    laneLowAt = 0;
    Serial.println("SIM: golyo a kilovosavban");
  }
  if (laneHighAt != 0 && now > laneHighAt) {
    simD[13] = 1;                  // golyo jatekban
    laneHighAt = 0;
    Serial.println("SIM: golyo jatekban");
  }

  // -- demo forgatokonyv --
  if (simScriptIx < SIM_SCRIPT_LEN && now - simT0 >= simScript[simScriptIx].t) {
    const SimEvent &e = simScript[simScriptIx];
    if (e.cmd == SIMCMD_PRESS) {
      SimPress(e.pin, e.ms);
    }
    if (e.cmd == SIMCMD_DRAIN) {
      SimDrain();
    }
    simScriptIx++;
  }

  // -- kezi parancsok sorosrol (nevbevitel alatt nem nyulunk a Serial-hoz!) --
  if (intmon != 2) {
    while (Serial.available() > 0) {
      char c = Serial.read();
      switch (c) {
        case 'S': SimPress(47, 200); break;      // start
        case 'K': SimPress(51, 400); break;      // kilovo gomb
        case 'D': SimDrain(); break;             // lefolyas
        case '1': SimPress(11, 150); break;      // weed 1..4
        case '2': SimPress(10, 150); break;
        case '3': SimPress(9, 150); break;
        case '4': SimPress(8, 150); break;
        case 'c': SimPress(12, 150); break;      // cheech
        case 'h': SimPress(7, 150); break;       // chong
        case 'o': SimPress(34, 100); break;      // pop1
        case 'n': SimPress(38, 100); break;      // spinner
        case 'l': SimPress(30, 100); break;      // loop felso
        case 'L': SimPress(43, 100); break;      // loop also
        case 'a': SimPress(45, 120); break;      // gate 1..3
        case 'b': SimPress(44, 120); break;
        case 'e': SimPress(40, 120); break;
        case 's': SimPress(4, 80); break;        // jobb sling
        case 'f': SimPress(27, 200); break;      // bal flipper
        case 'j': SimPress(26, 200); break;      // jobb flipper
        case 'x': SimPress(52, 100); break;      // nagy hid
        case 'y': SimPress(53, 100); break;      // kis hid
        case 'R':                                // demo ujrainditasa
          simT0 = millis();
          simScriptIx = 0;
          troughBalls = 5;
          SimUpdateTrough();
          Serial.println("SIM: demo ujraindul");
          break;
        case '?': SimHelp(); break;
        default: break;                          // \r \n stb. csendben
      }
    }
  }

  // -- emlekezteto, hogy sim modban vagyunk (15 mp-enkent) --
  if (now - simBannerAt > 15000) {
    simBannerAt = now;
    Serial.println("SimMode");
  }
}

#else // ---- eles build: atlatszo csonkok, nulla tobbletkoltseg ----

int SimDigitalRead(uint8_t pin) {
  return digitalRead(pin);
}

int SimAnalogRead(uint8_t pin) {
  return analogRead(pin);
}

void SimInit() {
}

void SimPoll() {
}

#endif // SIM_MODE
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End SIM MODE
/////////////////////////////////////////////////
/////////////////////////////////////////////////

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
//    kulonben a valodi pint olvassa (eles buildben = sima digitalRead)
//  - mini "golyofizika": figyeli a tarolo- es kilovo-tekercs kimenetet,
//    es ugy mozgatja a szimulalt golyot, ahogy a valosagban tortenne
//  - bekapcsolas utan automatikus DEMO JATEK fut le (lasd lent),
//    a Pi-s GUI-n elore kovetheto
//  - soros egykarakteres parancsokkal kezzel is lehet jatszani
//    (kuldj '?'-et a listaert)
//
// A demo forgatokonyv a valodi jatekmenetet koveti:
//  1. start -> PLAYER SELECT mod (GUI kilep az attractbol)
//  2. shoot gomb -> +1 jatekos (2 jatekos lesz)
//  3. start -> jatek indul, golyo a kilovosavba
//  P1/B1: kiloves -> gate -> popok -> slingek -> DAVE lane ->
//         E-outlane + ballsave-mentes -> lefolyas
//  P2/B1: gyors kor
//  P1/B2: WEED kigyujtes + spinner-porgetes -> MULTIBALL 1 + jackpot,
//         majd dupla gyors lefolyas (BIS>=4 fix tesztje!)
//  P2/B2: gyors kor
//  P1/B3: WEED -> UFO cinkelt kockaval -> SPACECOKE (5 golyos multiball),
//         lefolyasok ballsave alatt es utan (visszaloves-teszt)
//  P2/B3: gyors kor -> GAME OVER -> nevbevitel demo
//
// !!! ELES GEPRE FELTOLTES ELOTT A SIM_MODE DEFINE-T KI KELL VENNI !!!

#ifdef SIM_MODE

// ---- szimulalt allapotok ----
int8_t simD[70];                // -1 = valodi pin, 0/1 = szimulalt
int simA[16];                   // -1 = valodi, egyebkent szimulalt analog
unsigned long simReleaseAt[70]; // gombnyomas-lejaratok
uint8_t troughBalls = 5;        // golyok a taroloban
unsigned long laneLowAt = 0;    // ekkor "erkezik" a golyo a kilovosavba
unsigned long laneHighAt = 0;   // ekkor hagyja el a savot
unsigned long laneOccupiedSince = 0; // miota "foglalt" a sav (onjavitashoz)
unsigned long ufoRestoreAt = 0; // UFO-infra visszaallitasa
boolean prevTroughCoil = LOW;
boolean prevKickCoil = LOW;
unsigned long simBannerAt = 0;
unsigned int simScriptIx = 0;
unsigned long simPrevFire = 0;  // az elozo esemeny idopontja (relativ utemezes)
unsigned long simLaneAnchor = 0;

// ---- forgatokonyv ----
// trig: TIME = az elozo esemeny utan t ms-mal
//       LANE = amikor golyo er a kilovosavba, utana t ms-mal
#define TRIG_TIME 0
#define TRIG_LANE 1
#define SIMCMD_PRESS   0  // pin lenyomasa ms ideig
#define SIMCMD_DRAIN   1  // egy golyo lefolyik (vissza a taroloba)
#define SIMCMD_LOTTERY 2  // kovetkezo UFO-lotto kenyszeritese (pin = ertek)
#define SIMCMD_UFOHIT  3  // golyo az UFO-ba (A5 infra ms ideig)

struct SimEvent { uint8_t trig; uint32_t t; uint8_t cmd; uint8_t pin; uint16_t ms; };
const SimEvent simScript[] = {
  // --- inditas: player select 2 jatekossal ---
  { TRIG_TIME,  3000, SIMCMD_PRESS, 47, 200 },  // 1. start -> player select
  { TRIG_TIME,  2500, SIMCMD_PRESS, 51, 300 },  // shoot -> 2 jatekos
  { TRIG_TIME,  2500, SIMCMD_PRESS, 47, 200 },  // 2. start -> JATEK INDUL
  // --- P1 / B1: normal golyout + ballsave ---
  { TRIG_LANE,  1200, SIMCMD_PRESS, 51, 400 },  // kiloves
  { TRIG_TIME,  1500, SIMCMD_PRESS, 44, 100 },  // gate 2 (felso bonus lane)
  { TRIG_TIME,  1000, SIMCMD_PRESS, 34,  80 },  // popok pofozzak
  { TRIG_TIME,   400, SIMCMD_PRESS, 35,  80 },
  { TRIG_TIME,   400, SIMCMD_PRESS, 39,  80 },
  { TRIG_TIME,   500, SIMCMD_PRESS, 34,  80 },
  { TRIG_TIME,  1500, SIMCMD_PRESS,  4,  80 },  // sling oda-vissza
  { TRIG_TIME,   600, SIMCMD_PRESS, 31,  80 },
  { TRIG_TIME,   500, SIMCMD_PRESS,  4,  80 },
  { TRIG_TIME,  1500, SIMCMD_PRESS, 42, 150 },  // DAVE "A" lane
  { TRIG_TIME,  3000, SIMCMD_PRESS, 23, 150 },  // E-outlane! (sidelane ballsave)
  { TRIG_TIME,   800, SIMCMD_DRAIN,  0,   0 },  // golyo kiment -> azonnali mentes
  { TRIG_TIME, 20000, SIMCMD_DRAIN,  0,   0 },  // ballsave lejart -> P1/B1 vege
  // --- P2 / B1: gyors kor ---
  { TRIG_LANE,  1500, SIMCMD_PRESS, 51, 400 },
  { TRIG_TIME,  2000, SIMCMD_PRESS, 34,  80 },
  { TRIG_TIME, 16000, SIMCMD_DRAIN,  0,   0 },
  // --- P1 / B2: MULTIBALL teszt ---
  { TRIG_LANE,  1500, SIMCMD_PRESS, 51, 400 },
  { TRIG_TIME,  2000, SIMCMD_PRESS, 11, 150 },  // WEED kigyujtese
  { TRIG_TIME,   600, SIMCMD_PRESS, 10, 150 },
  { TRIG_TIME,   600, SIMCMD_PRESS,  9, 150 },
  { TRIG_TIME,   600, SIMCMD_PRESS,  8, 150 },
  { TRIG_TIME,  2500, SIMCMD_PRESS, 38, 100 },  // spinner x8 (-25/porges)
  { TRIG_TIME,   400, SIMCMD_PRESS, 38, 100 },
  { TRIG_TIME,   400, SIMCMD_PRESS, 38, 100 },
  { TRIG_TIME,   400, SIMCMD_PRESS, 38, 100 },
  { TRIG_TIME,   400, SIMCMD_PRESS, 38, 100 },
  { TRIG_TIME,   400, SIMCMD_PRESS, 38, 100 },
  { TRIG_TIME,   400, SIMCMD_PRESS, 38, 100 },
  { TRIG_TIME,   400, SIMCMD_PRESS, 38, 100 },  // -> MULTIBALL 1 (2 golyo)
  { TRIG_TIME,  6000, SIMCMD_PRESS, 53, 120 },  // kis hid = JACKPOT
  { TRIG_TIME, 32000, SIMCMD_DRAIN,  0,   0 },  // multiball-ballsave lejart
  { TRIG_TIME,   700, SIMCMD_DRAIN,  0,   0 },  // 2. golyo gyorsan: BIS>=4 teszt!
  // --- P2 / B2: gyors kor ---
  { TRIG_LANE,  1500, SIMCMD_PRESS, 51, 400 },
  { TRIG_TIME, 16000, SIMCMD_DRAIN,  0,   0 },
  // --- P1 / B3: SPACECOKE teszt ---
  { TRIG_LANE,  1500, SIMCMD_PRESS, 51, 400 },
  { TRIG_TIME,  2000, SIMCMD_PRESS, 11, 150 },  // WEED ujra (UFO elesitese)
  { TRIG_TIME,   600, SIMCMD_PRESS, 10, 150 },
  { TRIG_TIME,   600, SIMCMD_PRESS,  9, 150 },
  { TRIG_TIME,   600, SIMCMD_PRESS,  8, 150 },
  { TRIG_TIME,  2000, SIMCMD_LOTTERY, 7,  0 },  // cinkelt kocka: SpaceCoke
  { TRIG_TIME,   500, SIMCMD_UFOHIT, 0, 300 },  // golyo az UFO-ba
  { TRIG_TIME, 14000, SIMCMD_DRAIN,  0,   0 },  // 3 gyors lefolyas ballsave alatt
  { TRIG_TIME,  1200, SIMCMD_DRAIN,  0,   0 },  //   -> mindet vissza kell lonie!
  { TRIG_TIME,  1200, SIMCMD_DRAIN,  0,   0 },
  { TRIG_TIME, 26000, SIMCMD_DRAIN,  0,   0 },  // ballsave utan: golyok kiengedese
  { TRIG_TIME,  1200, SIMCMD_DRAIN,  0,   0 },
  { TRIG_TIME,  1200, SIMCMD_DRAIN,  0,   0 },
  { TRIG_TIME,  1200, SIMCMD_DRAIN,  0,   0 },  // BIS>=4 -> multiball vege
  { TRIG_TIME,  2500, SIMCMD_DRAIN,  0,   0 },  // utolso golyo -> P1/B3 vege
  // --- P2 / B3: gyors kor -> GAME OVER ---
  { TRIG_LANE,  1500, SIMCMD_PRESS, 51, 400 },
  { TRIG_TIME, 16000, SIMCMD_DRAIN,  0,   0 },  // -> "End" -> nevbevitel!
  // --- nevbevitel demo ---
  // A GUI a game over utan meg ~8 mp SUMMARY + ~8 mp FINAL_SCORES
  // kepernyot mutat (tobbjatekos mod!), a NAME_ENTRY csak ~18 mp-nel
  // jon - a gombnyomasokat CSAK UTANA szabad kuldeni!
  { TRIG_TIME, 22000, SIMCMD_PRESS, 27, 200 },  // betuvaltas balra
  { TRIG_TIME,  2000, SIMCMD_PRESS, 26, 200 },  // betuvaltas jobbra
  { TRIG_TIME,  2000, SIMCMD_PRESS, 51, 200 },  // betu kivalasztasa
  { TRIG_TIME,  2000, SIMCMD_PRESS, 51, 200 },
  { TRIG_TIME,  2000, SIMCMD_PRESS, 51, 200 },  // nev kesz
};
#define SIM_SCRIPT_LEN (sizeof(simScript) / sizeof(simScript[0]))

void SimUpdateTrough() {
  for (uint8_t i = 0; i < 5; i++) {
    simA[i] = (i < troughBalls) ? 50 : 500; // FORDITOTT: <100 = golyo ott van
  }
}

// Figyelem: a soros kiirasok VESSZOVEL tagoltak, mert a GUI protokollja
// minden ismeretlen, vesszo nelkuli sort video-triggernek ertelmez!
void SimPress(uint8_t pin, uint16_t ms) {
  simD[pin] = 0; // INPUT_PULLUP logika: LOW = benyomva
  simReleaseAt[pin] = millis() + ms;
  Serial.print("SIM,press,");
  Serial.println(pin);
}

void SimDrain() {
  if (troughBalls < 5) {
    troughBalls++;
  }
  SimUpdateTrough();
  Serial.println("SIM,drain");
}

void SimUfoHit(uint16_t ms) {
  simA[5] = 50; // <100 = golyo az UFO-ban
  ufoRestoreAt = millis() + ms;
  Serial.println("SIM,ufohit");
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
  simA[5] = 500;  // UFO infra: nincs benne golyo
  simD[13] = 1;   // kilovosav: ures
  simPrevFire = millis();
  simScriptIx = 0;
  simLaneAnchor = 0;
  Serial.println("SIM,aktiv,demo indul");
}

int SimDigitalRead(uint8_t pin) {
  if (pin < 70 && simD[pin] >= 0) {
    return simD[pin];
  }
  return digitalRead(pin);
}

int SimAnalogRead(uint8_t pin) {
  uint8_t ch = (pin >= 54) ? pin - 54 : pin; // PIN_A0..A15 vagy csatornaszam
  if (ch < 16 && simA[ch] >= 0) {
    return simA[ch];
  }
  return analogRead(pin);
}

void SimHelp() {
  Serial.println("SIM,parancsok: S=start K=kiloves D=lefolyas R=demo ujra");
  Serial.println("SIM,1-4=weed c=cheech h=chong o=pop n=spinner l/L=loop");
  Serial.println("SIM,a/b/e=gate s=sling f/j=flipper x/y=hid u=ufo");
  Serial.println("SIM,U=SpaceCoke-cinkeles M=pontlopas-cinkeles (utana u!)");
}

// Indexet kap (nem SimEvent&-t), mert az Arduino auto-prototipusai a
// struct definicioja ELE kerulnenek -> "does not name a type" fordito hiba
void SimRunEvent(uint8_t idx) {
  const SimEvent &e = simScript[idx];
  if (e.cmd == SIMCMD_PRESS) {
    SimPress(e.pin, e.ms);
  }
  if (e.cmd == SIMCMD_DRAIN) {
    SimDrain();
  }
  if (e.cmd == SIMCMD_LOTTERY) {
    simForceLottery = e.pin;
    Serial.println("SIM,cinkelt kocka");
  }
  if (e.cmd == SIMCMD_UFOHIT) {
    SimUfoHit(e.ms);
  }
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

  // -- UFO infra visszaallitasa --
  if (ufoRestoreAt != 0 && now > ufoRestoreAt) {
    simA[5] = 500;
    ufoRestoreAt = 0;
  }

  // -- mini golyofizika: tekercs-kimenetek figyelese --
  boolean troughCoil = digitalRead(ballTroughCoil);
  if (troughCoil == HIGH && prevTroughCoil == LOW && troughBalls > 0) {
    troughBalls--;                 // egy golyo elhagyja a tarolot
    SimUpdateTrough();
    laneLowAt = now + 800;         // ~0.8 mp mulva er a kilovosavba
    Serial.println("SIM,golyo kilokve");
  }
  prevTroughCoil = troughCoil;

  boolean kickCoil = digitalRead(shooterlaneCoil);
  if (kickCoil == HIGH && prevKickCoil == LOW) {
    Serial.print("SIM,kicker,lane=");   // diagnosztika: fantom-kick vadaszat
    Serial.println((int)simD[13]);
    if (laneHighAt == 0) {
      laneHighAt = now + 150;      // a kicker kilovi a savbol
    }                              // (ujratuzeles NEM tolja ki az uritest!)
    laneLowAt = 0;
  }
  prevKickCoil = kickCoil;

  if (laneLowAt != 0 && now > laneLowAt) {
    simD[13] = 0;                  // golyo a kilovosavban
    laneOccupiedSince = now;
    laneLowAt = 0;
    Serial.println("SIM,golyo a savban");
  }
  if (laneHighAt != 0 && now > laneHighAt) {
    simD[13] = 1;                  // golyo jatekban
    laneHighAt = 0;
    Serial.println("SIM,golyo jatekban");
  }

  // ONJAVITAS: ha a sav 8 mp-nel tovabb "foglalt" (beragadt allapot -
  // a valosagban a golyo nem ul orokke a savban), kiuritjuk, kulonben
  // fantom-kickek szuletnek es az egesz demo konyveles szetcsuszik
  if (simD[13] == 0 && now - laneOccupiedSince > 8000) {
    simD[13] = 1;
    Serial.println("SIM,lane-timeout-fix");
  }

  // -- forgatokonyv-lepteto (relativ idozites, LANE = golyora var) --
  if (simScriptIx < SIM_SCRIPT_LEN) {
    const SimEvent &e = simScript[simScriptIx];
    if (e.trig == TRIG_TIME) {
      if (now - simPrevFire >= e.t) {
        SimRunEvent(simScriptIx);
        simPrevFire = now;
        simScriptIx++;
      }
    }
    else { // TRIG_LANE
      if (simLaneAnchor == 0) {
        if (simD[13] == 0) {
          simLaneAnchor = now;     // golyo megerkezett, innen indul a t
        }
      }
      else if (now - simLaneAnchor >= e.t) {
        SimRunEvent(simScriptIx);
        simPrevFire = now;
        simLaneAnchor = 0;
        simScriptIx++;
      }
    }
  }

  // -- kezi parancsok sorosrol (nevbevitel alatt nem nyulunk a Serial-hoz) --
  if (intmon != 2) {
    while (Serial.available() > 0) {
      char c = Serial.read();
      switch (c) {
        case 'S': SimPress(47, 200); break;      // start
        case 'K': SimPress(51, 400); break;      // kilovo / player select gomb
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
        case 'U':                                // cinkelt kocka: SpaceCoke
          simForceLottery = 7;
          Serial.println("SIM,cinkelt kocka");
          break;
        case 'M':                                // cinkelt kocka: pontlopas
          simForceLottery = 8;                   // (csak tobbjatekos jatekban ervenyes!)
          Serial.println("SIM,cinkelt kocka,pontlopas");
          break;
        case 'u': SimUfoHit(300); break;         // golyo az UFO-ba
        case 'R':                                // demo ujrainditasa
          simScriptIx = 0;
          simPrevFire = millis();
          simLaneAnchor = 0;
          troughBalls = 5;
          SimUpdateTrough();
          Serial.println("SIM,demo ujraindul");
          break;
        case '?': SimHelp(); break;
        default: break;                          // \r \n stb. csendben
      }
    }
  }

  // -- emlekezteto, hogy sim modban vagyunk (15 mp-enkent) --
  if (now - simBannerAt > 15000) {
    simBannerAt = now;
    Serial.println("SIM,mode");
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

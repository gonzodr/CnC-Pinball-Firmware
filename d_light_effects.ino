/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// FENYEFFEKT MOTOR (V4) - baked-frame
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//
// Minden effekt "baked-frame": kesz RGB kepkockakbol all, amiket a KULSO
// SZERKESZTO gyart (cellankent kesz szin/fenyero, tetszoleges gorbe) - a
// motor csak kirakja. Adat + leiro: effect_data.h (ott a teljes spec).
//
// KETFELE lejatszas (a leiro overlay flagje donti el):
//  - FULL (overlay=0): az effekt ATVESZI a palyat, a (0,0,0) = fekete.
//    Inditas: effect = HIGH; effectID = ID;   (a jatek-LED-ek el vannak nyomva)
//  - OVERLAY (overlay=1): az effekt csak RARAJZOL a normal jatek-fenyre, a
//    (0,0,0) cella = ATLATSZO (a motor nem erinti). Igy egy kis effekt nem
//    sotetiti el a palya tobbi reszet. Inditas: PlayOverlay(ID);
//
// Az effekt ID-ja EXPLICIT a leiroban (nem a tabla-sorrend) -> ID szerint
// keresunk. INTRO+LOOP+OUTRO: introFrames = az elso N kocka EGYSZER lefut,
// utana [introFrames, loopFrames) a ciklus (loops-szor), a tobbi 1x outro.
// introFrames=0 -> nincs intro (a ciklus rogton a 0. kockatol indul, a regi
// viselkedes). A vegen a motor magatol all vissza.

#define EFFECT_LEDS 68 // a jatekter LED-jei (0..67, lasd LEDMAP.md)
// Overlay effektnel a per-cella ATLATSZO sentinel: magenta (255,0,255).
// Ezt a cellat a motor kihagyja (a jatek latszik alatta). A (0,0,0) NEM
// atlatszo -> az fekete (elsotetit). Igy per-cella/per-kocka donthetsz:
// szin / fekete-elsotetites / atlatszo.
#define FX_TR_R 255
#define FX_TR_G 0
#define FX_TR_B 255

// Az effect_data.h-ban a fx_* tablak XOR-maszkolva vannak TAROLVA. Ok: ezen a
// panelen az USB-soros feltoltes a maszkolatlan bajtmintakon elhal (a flash
// iras 28%-nal timeoutol), maszkolva viszont hibatlanul felmegy. A maszk csak
// a tarolast erinti, olvasaskor a fx_read() visszafejti - a szinek valtozatlanok,
// a magenta sentinel vizsgalat is a dekodolt ertekeken fut.
// FIGYELEM: az effect_data.h GENERALT fajl. Ujrageneralas utan ujra le kell
// futtatni a mask_effect_data.ps1-et, kulonben elszinezodnek az effektek!
#define FX_DATA_MASK 0x5A
// Maszkolatlan effect_data.h-val a firmware LEFORDULNA es FELMENNE, csak rossz
// szinekkel - ezert ez itt fordulaskor elhasal helyette.
#if !defined(FX_DATA_MASK_APPLIED) || (FX_DATA_MASK_APPLIED != FX_DATA_MASK)
#error "effect_data.h is not XOR-masked (or uses a different mask) - run mask_effect_data.ps1"
#endif
static inline uint8_t fx_read(const uint8_t* p) { return pgm_read_byte(p) ^ FX_DATA_MASK; }

// Gamma-korrekcio (2.2, az sRGB monitorhoz igazitva). A szerkeszto a
// monitoron (gamma ~2.2) tervezi a szineket, a WS2812B viszont LINEARIS,
// ezert a koztes ertekek (gradiens/feather/opacity) tul vilagosnak es
// elszinezodve (piros->pink, zold->vilagoskek) jelentek meg. Ez a tabla a
// baked szineket visszahuzza oda, ahogy a monitoron latszanak. A tiszta
// szinek (0 es 255) valtozatlanok -> a normal jatekfenyeket NEM erinti.
const uint8_t gamma8[256] PROGMEM = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
  3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6,
  6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12,
  12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19,
  20, 20, 21, 22, 22, 23, 23, 24, 25, 25, 26, 26, 27, 28, 28, 29,
  30, 30, 31, 32, 33, 33, 34, 35, 35, 36, 37, 38, 39, 39, 40, 41,
  42, 43, 43, 44, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 54, 55,
  56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
  73, 74, 75, 76, 77, 78, 79, 81, 82, 83, 84, 85, 87, 88, 89, 90,
  91, 93, 94, 95, 97, 98, 99, 100, 102, 103, 105, 106, 107, 109, 110, 111,
  113, 114, 116, 117, 119, 120, 121, 123, 124, 126, 127, 129, 130, 132, 133, 135,
  137, 138, 140, 141, 143, 145, 146, 148, 149, 151, 153, 154, 156, 158, 159, 161,
  163, 165, 166, 168, 170, 172, 173, 175, 177, 179, 181, 182, 184, 186, 188, 190,
  192, 194, 196, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221,
  223, 225, 227, 229, 231, 234, 236, 238, 240, 242, 244, 246, 248, 251, 253, 255,
};
static inline uint8_t gam(uint8_t v)        { return pgm_read_byte(&gamma8[v]); }
static inline uint8_t fx_g(const uint8_t* p) { return gam(fx_read(p)); }

int  runningEffect = 0;
unsigned long effectStartT = 0;
uint8_t fullEffectPlaysRemaining = 1;
boolean fullEffectLoopForever = LOW;

int8_t overlayIdx = -1;           // futo overlay effekt tabla-indexe (-1 = nincs)
unsigned long overlayStartT = 0;

// Az aktualis kockaindex (intro + ciklus + outro logika). done = true ha az effekt lejart.
uint16_t bakedCurrentFrame(const EffectDef& e, unsigned long startT, bool& done) {
  uint16_t introLen = (e.introFrames > e.frames) ? e.frames : e.introFrames;
  uint16_t loopEnd  = (e.loopFrames == 0 || e.loopFrames > e.frames) ? e.frames : e.loopFrames;
  if (loopEnd < introLen) loopEnd = introLen; // biztonsagi kapocs: a ciklus nem erhet veget az intro elott
  uint16_t loopLen  = loopEnd - introLen;
  uint8_t  loops    = e.loops ? e.loops : 1;
  uint16_t outroLen = e.frames - loopEnd;
  uint32_t cycleSteps = (uint32_t)loopLen * loops;
  uint32_t totalSteps = (uint32_t)introLen + cycleSteps + outroLen;
  uint32_t step = (e.frameMs > 0) ? ((millis() - startT) / e.frameMs) : 0;
  if (step >= totalSteps) { done = true; return 0; }
  done = false;
  if (step < introLen) return (uint16_t)step;
  uint32_t afterIntro = step - introLen;
  if (afterIntro < cycleSteps) {
    return (uint16_t)(introLen + (loopLen ? (afterIntro % loopLen) : 0));
  }
  return (uint16_t)(loopEnd + (afterIntro - cycleSteps));
}

// Az effekt data-kezdopointere az adott kockahoz (PROGMEM).
static inline const uint8_t* bakedFramePtr(const EffectDef& e, uint16_t frame) {
  return e.data + (uint32_t)frame * EFFECT_LEDS * 3;
}

// FULL effekt: az egesz palyat felulirja (a (0,0,0) is fekete lesz).
void RunBakedEffect(uint8_t idx) {
  const EffectDef& e = bakedEffects[idx];
  bool done;
  uint16_t frame = bakedCurrentFrame(e, effectStartT, done);
  if (done) {
    // A runtime is kerhet teljes-effekt ismetlest: Danger = 3 teljes kor,
    // Tilt = drainig vegtelen. A Hurry Up regi, mod-vegeig tarto loopja is
    // ezen a lezarasi ponton marad.
    if (fullEffectLoopForever == HIGH ||
        (hurryUp == HIGH && runningEffect == 6)) {
      effectStartT = millis();
      frame = bakedCurrentFrame(e, effectStartT, done);
    } else if (fullEffectPlaysRemaining > 1) {
      fullEffectPlaysRemaining--;
      effectStartT = millis();
      frame = bakedCurrentFrame(e, effectStartT, done);
    } else { // vege -> vissza a normal jatek-fenyre
      effect = LOW; effectID = 0; runningEffect = 0;
      fullEffectPlaysRemaining = 1;
      fullEffectLoopForever = LOW;
      initlight = HIGH; Initlights();
      return;
    }
  }
  const uint8_t* p = bakedFramePtr(e, frame);
  for (uint8_t i = 0; i < EFFECT_LEDS; i++) {
    // A baked adat R,G,B sorrendben all, de ezen a szalagon a baked szineknel
    // a G es B csatorna fel van cserelve (piros->pink, zold->vilagoskek volt),
    // ezert G/B cserevel irjuk ki: CRGB(R, B, G). A normal jatekfenyeket ez
    // NEM erinti (azok kulon, a helyukon vannak).
    leds[i] = CRGB(fx_g(p), fx_g(p + 1), fx_g(p + 2)); // gamma-korrekcio
    p += 3;
  }
}

// FULL baked effekt inditasa megadott szamu teljes lejatszassal. A forever
// flag a plays erteket felulirja; StopFullBakedEffect() allitja le explicit.
void StartFullBakedEffect(uint8_t id, uint8_t plays, boolean forever) {
  for (uint8_t i = 0; i < bakedEffectCount; i++) {
    if (bakedEffects[i].id == id && !bakedEffects[i].overlay) {
      effect = HIGH;
      effectID = id;
      runningEffect = id;
      effectStartT = millis();
      fullEffectPlaysRemaining = plays ? plays : 1;
      fullEffectLoopForever = forever;
      return;
    }
  }
}

void StopFullBakedEffect() {
  effect = LOW;
  effectID = 0;
  runningEffect = 0;
  fullEffectPlaysRemaining = 1;
  fullEffectLoopForever = LOW;
}

// OVERLAY effekt inditasa a jateklogikabol (csak overlay-flages effektet fogad).
void PlayOverlay(uint8_t id) {
  for (uint8_t i = 0; i < bakedEffectCount; i++) {
    if (bakedEffects[i].id == id && bakedEffects[i].overlay) {
      overlayIdx = i;
      overlayStartT = millis();
      return;
    }
  }
}

// A bank overlay/full flagje alapjan a megfelelo egyszeri lejatszasi utat
// valasztja. Ezt hasznaljak az uj collect- es combo-effektek.
void PlayBakedEffectOnce(uint8_t id) {
  for (uint8_t i = 0; i < bakedEffectCount; i++) {
    if (bakedEffects[i].id == id) {
      if (bakedEffects[i].overlay) {
        PlayOverlay(id);
      } else {
        StartFullBakedEffect(id, 1, LOW);
      }
      return;
    }
  }
}

// OVERLAY effekt: csak a szinadatot hordozo (nem-(0,0,0)) cellakra rajzol,
// a tobbi LED-et a jatek-kepen hagyja. A loop VEGEN a subsystemek utan hivando!
void RunOverlayEffect() {
  if (overlayIdx < 0 || effect == HIGH) { // full effekt alatt nincs overlay
    return;
  }
  const EffectDef& e = bakedEffects[overlayIdx];
  bool done;
  uint16_t frame = bakedCurrentFrame(e, overlayStartT, done);
  if (done) { overlayIdx = -1; return; } // vege - a jatek-feny megy tovabb

  const uint8_t* p = bakedFramePtr(e, frame);
  for (uint8_t i = 0; i < EFFECT_LEDS; i++) {
    uint8_t r = fx_read(p), g = fx_read(p + 1), b = fx_read(p + 2);
    // magenta (255,0,255) = ATLATSZO -> kihagyjuk (a jatek latszik alatta).
    // minden mas rajzolodik, a (0,0,0) fekete is (elsotetit)!
    if (!(r == FX_TR_R && g == FX_TR_G && b == FX_TR_B)) {
      leds[i] = CRGB(gam(r), gam(g), gam(b)); // gamma-korrekcio
    }
    p += 3;
  }
}

// FULL effektek dispatchere (effect == HIGH). Az overlay kulon ut (RunOverlayEffect).
void RunLightEffect() {
  if (effect != HIGH) {
    runningEffect = 0;
    return;
  }
  if (runningEffect != effectID) { // uj effekt indul
    runningEffect = effectID;
    effectStartT = millis();
    fullEffectPlaysRemaining = 1;
    fullEffectLoopForever = LOW;
  }
  int8_t idx = -1;
  for (uint8_t i = 0; i < bakedEffectCount; i++) {
    if (bakedEffects[i].id == runningEffect) { idx = i; break; }
  }
  if (idx < 0) { // nincs ilyen ID (meg) -> biztonsagos lezaras
    effect = LOW; effectID = 0; runningEffect = 0;
    initlight = HIGH; Initlights();
    return;
  }
  RunBakedEffect(idx);
}

/////////////////////////////////////////////////
//// LIGHT TEST (szerviz menubol, soros parancs)
/////////////////////////////////////////////////
// A GUI szerviz menu "Light test" kepernyoje kuldi soroson:
//   LT,<id>  -> az adott ID-ju effekt VEGTELENITVE (loop) jatszasa
//   LT,S     -> stop (vissza a normal fenyre)
// Csak VIZUALIS: a leds[]-et iratja felul a FastLED.show() elott, tekercset
// / jateklogikat NEM erint. A parancs-olvaso nem-blokkolo (nem readString!),
// es a main loop csak intmon != 2 mellett hivja (nem utkozik a nevbevitellel).

int8_t lightTestIdx = -1;          // -1 = nincs teszt; egyebkent bakedEffects index
unsigned long lightTestStartT = 0;

void StartLightTest(uint8_t id) {
  for (uint8_t i = 0; i < bakedEffectCount; i++) {
    if (bakedEffects[i].id == id) {
      lightTestIdx = i;
      lightTestStartT = millis();
      return;
    }
  }
  // ismeretlen id -> nem valtunk (marad, ami volt)
}

void StopLightTest() {
  lightTestIdx = -1; // a kovetkezo kor magatol visszaall a normal fenyre
}

// A kivalasztott effektet loopolva rajzolja feketere torolt alapra,
// sentinel-tudatosan (magenta = atlatszo -> fekete marad). Igy full ES
// overlay effekt is helyesen nez ki. A FastLED.show() ELE hivando.
void RunLightTest() {
  if (lightTestIdx < 0) return;
  const EffectDef& e = bakedEffects[lightTestIdx];
  bool done;
  uint16_t frame = bakedCurrentFrame(e, lightTestStartT, done);
  if (done) {                    // loop: ujraindul
    lightTestStartT = millis();
    frame = 0;
  }
  for (uint8_t i = 0; i < EFFECT_LEDS; i++) leds[i] = CRGB::Black;
  const uint8_t* p = bakedFramePtr(e, frame);
  for (uint8_t i = 0; i < EFFECT_LEDS; i++) {
    uint8_t r = fx_read(p), g = fx_read(p + 1), b = fx_read(p + 2);
    if (!(r == FX_TR_R && g == FX_TR_G && b == FX_TR_B)) leds[i] = CRGB(gam(r), gam(g), gam(b)); // gamma-korrekcio
    p += 3;
  }
}

// "LT,<id>" vagy "LT,S" feldolgozasa.
void HandleLightTestCmd(const char* s) {
  if (s[0] != 'L' || s[1] != 'T' || s[2] != ',') return;
  const char* arg = s + 3;
  if (arg[0] == 'S' || arg[0] == 's') { StopLightTest(); return; }
  StartLightTest((uint8_t)atoi(arg));
}

// Kozos, nem-blokkolo soros sor-olvaso. Az LT szervizparancsok es az MG_*
// minijatek-protokoll ugyanazon az egyetlen parseren futnak, igy ket rutin
// sosem tudja egymas elol "megenni" a byte-okat.
char controlBuf[64];
uint8_t controlLen = 0;

void HandleControlCmd(const char* s) {
  if (s[0] == 'L' && s[1] == 'T' && s[2] == ',') {
    HandleLightTestCmd(s);
    return;
  }
  if (s[0] == 'A' && s[1] == 'T' && s[2] == ',') {
    HandleAnalogTestCmd(s);   // analog bemenet-teszt (h_analog_test.ino)
    return;
  }
  if (s[0] == 'M' && s[1] == 'G' && s[2] == '_') {
    HandleMunchiesCommand(s);
  }
}

void PollControlSerial() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (controlLen > 0) {
        controlBuf[controlLen] = '\0';
        HandleControlCmd(controlBuf);
        controlLen = 0;
      }
    } else if (controlLen < sizeof(controlBuf) - 1) {
      controlBuf[controlLen++] = c;
    } else {
      controlLen = 0; // tullepes -> eldobjuk a sort
    }
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Fenyeffekt motor
/////////////////////////////////////////////////
/////////////////////////////////////////////////

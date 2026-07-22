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
// keresunk. LOOP+OUTRO: loopFrames = a ciklus (elso N kocka, loops-szor), a
// tobbi 1x outro. A vegen a motor magatol all vissza.

#define EFFECT_LEDS 68 // a jatekter LED-jei (0..67, lasd LEDMAP.md)
// Overlay effektnel a per-cella ATLATSZO sentinel: magenta (255,0,255).
// Ezt a cellat a motor kihagyja (a jatek latszik alatta). A (0,0,0) NEM
// atlatszo -> az fekete (elsotetit). Igy per-cella/per-kocka donthetsz:
// szin / fekete-elsotetites / atlatszo.
#define FX_TR_R 255
#define FX_TR_G 0
#define FX_TR_B 255

int  runningEffect = 0;
unsigned long effectStartT = 0;

int8_t overlayIdx = -1;           // futo overlay effekt tabla-indexe (-1 = nincs)
unsigned long overlayStartT = 0;

// Az aktualis kockaindex (ciklus + outro logika). done = true ha az effekt lejart.
uint16_t bakedCurrentFrame(const EffectDef& e, unsigned long startT, bool& done) {
  uint16_t loopLen  = (e.loopFrames == 0 || e.loopFrames > e.frames) ? e.frames : e.loopFrames;
  uint8_t  loops    = e.loops ? e.loops : 1;
  uint16_t outroLen = e.frames - loopLen;
  uint32_t cycleSteps = (uint32_t)loopLen * loops;
  uint32_t totalSteps = cycleSteps + outroLen;
  uint32_t step = (e.frameMs > 0) ? ((millis() - startT) / e.frameMs) : 0;
  if (step >= totalSteps) { done = true; return 0; }
  done = false;
  return (step < cycleSteps) ? (uint16_t)(step % loopLen)
                             : (uint16_t)(loopLen + (step - cycleSteps));
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
  if (done) { // vege -> vissza a normal jatek-fenyre
    effect = LOW; effectID = 0; runningEffect = 0;
    initlight = HIGH; Initlights();
    return;
  }
  const uint8_t* p = bakedFramePtr(e, frame);
  for (uint8_t i = 0; i < EFFECT_LEDS; i++) {
    leds[i] = CRGB(pgm_read_byte(p), pgm_read_byte(p + 1), pgm_read_byte(p + 2));
    p += 3;
  }
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
    uint8_t r = pgm_read_byte(p), g = pgm_read_byte(p + 1), b = pgm_read_byte(p + 2);
    // magenta (255,0,255) = ATLATSZO -> kihagyjuk (a jatek latszik alatta).
    // minden mas rajzolodik, a (0,0,0) fekete is (elsotetit)!
    if (!(r == FX_TR_R && g == FX_TR_G && b == FX_TR_B)) {
      leds[i] = CRGB(r, g, b);
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
    uint8_t r = pgm_read_byte(p), g = pgm_read_byte(p + 1), b = pgm_read_byte(p + 2);
    if (!(r == FX_TR_R && g == FX_TR_G && b == FX_TR_B)) leds[i] = CRGB(r, g, b);
    p += 3;
  }
}

// "LT,<id>" vagy "LT,S" feldolgozasa (mas sort csendben eldob).
void HandleLightTestCmd(const char* s) {
  if (s[0] != 'L' || s[1] != 'T' || s[2] != ',') return;
  const char* arg = s + 3;
  if (arg[0] == 'S' || arg[0] == 's') { StopLightTest(); return; }
  StartLightTest((uint8_t)atoi(arg));
}

// Nem-blokkolo soros sor-olvaso a light-test parancsokhoz. A main loop
// hivja minden korben (intmon != 2 mellett). Sorvegig gyujt, majd feldolgoz.
char ltBuf[16];
uint8_t ltLen = 0;
void PollLightTestSerial() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (ltLen > 0) { ltBuf[ltLen] = '\0'; HandleLightTestCmd(ltBuf); ltLen = 0; }
    } else if (ltLen < sizeof(ltBuf) - 1) {
      ltBuf[ltLen++] = c;
    } else {
      ltLen = 0; // tullepes -> eldobjuk a sort
    }
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Fenyeffekt motor
/////////////////////////////////////////////////
/////////////////////////////////////////////////

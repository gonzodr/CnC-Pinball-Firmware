/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// FENYEFFEKT MOTOR (V4)
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//
// Minden effekt kozos, ido-alapu motoron fut:
//  - a kepkockak millis() alapjan, fix utemben (EFFECT_FRAME_MS)
//    leptetnek -> egyenletes mozgas, nincs effectState/bool tanc
//  - az aktualis kepkocka MINDEN loop-korben ujrarajzolodik, igy a
//    jateklogika nem tudja felulirni az effekt kepet
//  - FastLED.show() NINCS itt: a loop vegi egyetlen show() tolja ki
//  - fadeToBlackBy() folyamatos utofenyt ad: ami kialszik, az szepen
//    elhalvanyul, a vegen pedig EFFECT_FADE_MS alatt hunyik ki minden
//
// Inditas a jateklogikabol (VALTOZATLAN interfesz):
//    effect = HIGH; effectID = n;
// A motor a vegen magatol all vissza (effect = LOW + Initlights).
//
// Effekt ID-k:
//  1 = Kiloves (EffectID1 maszk, 7 kocka, feher)
//  2 = Weed multiball (EffectID2 maszk, 4 kocka x 4 szin)
//  3 = Weedblast (EffectID3 tobbszinu maszk, 6 kocka)
//  4 = Looplight (teljes palya, 4 szin villanasai)
//  5 = Hid (EffectID5 maszk, 4x feher + 4x piros)
//  6 = HurryUp talalat (teljes palya, sorsolt szin, 4 villanas)

#define EFFECT_LEDS 68        // a jatekter LED-jei (0..67)
#define EFFECT_FRAME_MS 60    // egy lepes idotartama
#define EFFECT_FADE_MS 700    // utofeny a vegen, mielott visszaall az alap
#define EFFECT_FADE_AMOUNT 25 // halvanyitas loop-koronkent (nagyobb = gyorsabb kihunyas)

int runningEffect = 0;
unsigned long effectStartT = 0;
CRGB hurryColor = CRGB::White;

const CRGB hurryPalette[6] = {
  CRGB::Blue, CRGB::Green, CRGB::Yellow, CRGB::Purple, CRGB::Red, CRGB::White
};

// A Weedblast (EffectID3) szinkodjai: a tabla 1..12 ertekeihez tartozo szinek
const CRGB blastPalette[12] = {
  CRGB::White, CRGB::Yellow, CRGB::Red, CRGB::Orange, CRGB::Green,
  CRGB::IndianRed, CRGB::SkyBlue, CRGB::DeepPink, CRGB::DarkViolet,
  CRGB::Tomato, CRGB::LightSeaGreen, CRGB::FairyLight
};

// Egy maszk-sor kirajzolasa a PROGMEM tablabol, egy szinnel.
// A 0 ertek = hatter: NEM feketet irunk, hanem beke hagyjuk, hogy az
// elozo kocka fenyei a fadeToBlackBy-tol szepen elhalvanyuljanak.
void EffectDrawMask(const uint8_t* table, uint8_t row, CRGB color) {
  int base = (int)row * EFFECT_LEDS;
  for (int i = 0; i < EFFECT_LEDS; i++) {
    if (pgm_read_byte(&table[base + i]) != 0) {
      leds[i] = color;
    }
  }
}

// Tobbszinu maszk-sor (Weedblast): a tabla erteke valasztja ki a szint
void EffectDrawMulti(const uint8_t* table, uint8_t row) {
  int base = (int)row * EFFECT_LEDS;
  for (int i = 0; i < EFFECT_LEDS; i++) {
    uint8_t c = pgm_read_byte(&table[base + i]);
    if (c >= 1 && c <= 12) {
      leds[i] = blastPalette[c - 1];
    }
  }
}

// Teljes jatekter egy szinre
void EffectFill(CRGB color) {
  fill_solid(leds, EFFECT_LEDS, color);
}

void RunLightEffect() {
  if (effect != HIGH) {
    runningEffect = 0;
    return;
  }

  // uj effekt indul (vagy masik effekt valtja le a futot)
  if (runningEffect != effectID) {
    runningEffect = effectID;
    effectStartT = millis();
    if (effectID == 6) {
      hurryColor = hurryPalette[random(0, 6)]; // szin EGYSZER sorsolva, nem villodzik
    }
  }

  unsigned long elapsed = millis() - effectStartT;
  unsigned int step = elapsed / EFFECT_FRAME_MS;

  // folyamatos utofeny: amit ebben a korben nem rajzolunk ujra, halvanyul
  fadeToBlackBy(leds, EFFECT_LEDS, EFFECT_FADE_AMOUNT);

  unsigned int totalSteps = 0;

  switch (runningEffect) {

    case 1: // Kiloves -- 7 kockas vegigfutas, feher
      totalSteps = 7;
      if (step < totalSteps) {
        EffectDrawMask(EffectID1, step, CRGB::White);
      }
      break;

    case 2: // Weed multiball -- 4 kocka x 4 szin, lukteto utemben
      totalSteps = 32;
      if (step < totalSteps && (step & 1) == 0) {
        CRGB c;
        if      (step < 8)  { c = CRGB::Red; }
        else if (step < 16) { c = CRGB::Orange; }
        else if (step < 24) { c = CRGB::Yellow; }
        else                { c = CRGB::White; }
        EffectDrawMask(EffectID2, (step >> 1) & 3, c);
      }
      break;

    case 3: // Weedblast -- 6 kockas, tobbszinu "robbanas"
      totalSteps = 6;
      if (step < totalSteps) {
        EffectDrawMulti(EffectID3, step);
      }
      break;

    case 4: // Looplight -- teljes palya, 4 szin lukteto villanasai
      totalSteps = 32;
      if (step < totalSteps && (step & 1) == 0) {
        CRGB c;
        if      (step < 8)  { c = CRGB::DeepPink; }
        else if (step < 16) { c = CRGB::DarkViolet; }
        else if (step < 24) { c = CRGB::SkyBlue; }
        else                { c = CRGB::White; }
        EffectFill(c);
      }
      break;

    case 5: // Hid -- maszkos villogas: 4x feher, majd 4x piros
      totalSteps = 16;
      if (step < totalSteps && (step & 1) == 0) {
        EffectDrawMask(EffectID5, (step >> 1) & 3, (step < 8) ? CRGB::White : CRGB::Red);
      }
      break;

    case 6: // HurryUp talalat -- 4 villanas az indulaskor sorsolt szinnel
      totalSteps = 8;
      if (step < totalSteps && (step & 1) == 0) {
        EffectFill(hurryColor);
      }
      break;

    default: // ismeretlen ID -- vedelem: azonnal lezarjuk
      totalSteps = 0;
      break;
  }

  // az animacio lefutott ES az utofeny is kihunyt -> visszaallas az alapra
  if (elapsed > (unsigned long)totalSteps * EFFECT_FRAME_MS + EFFECT_FADE_MS) {
    effect = LOW;
    effectID = 0;
    runningEffect = 0;
    initlight = HIGH;
    Initlights();
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Fenyeffekt motor
/////////////////////////////////////////////////
/////////////////////////////////////////////////

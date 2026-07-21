/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// FENYEFFEKT MOTOR (V4) - baked-frame
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//
// Minden effekt "baked-frame": kesz RGB kepkockakbol all, amiket a KULSO
// SZERKESZTO gyart. A szerkeszto cellankent kisut MINDENT (szin, fenyero/
// opacity, per-LED kialvas, tetszoleges gorbe) - a motor csak kirakja a
// kepkockat a szalagra. Nincs procedturalis logika, nincs fade a motorban.
//
// Az effektek adata + leiroja az effect_data.h-ban van (ott a reszletes
// formatum-specifikacio). Roviden:
//   - adat: PROGMEM bajt-tomb, kocka-major, kockankent 68 LED x 3 bajt (R,G,B)
//   - leiro: bakedEffects[] tabla, soronkent { nev, data, kockak, kocka_ms, loop }
//   - az effekt ID-ja = a tablabeli sorszam + 1 (elso sor = ID 1)
//
// Inditas a jateklogikabol (VALTOZATLAN interfesz):  effect = HIGH; effectID = N;
// A motor a vegen magatol all vissza (effect = LOW + Initlights).
//
// A jatek jelenlegi trigger-pontjai (ezekhez keszits effektet a szerkesztoben):
//   ID 1 = Kiloves (golyo kiloves)         ID 2 = Weed multiball
//   ID 3 = Weedblast (weed kigyujtve)       ID 4 = Looplight (loop / SpaceCoke)
//   ID 5 = Hid (bridge jackpot)             ID 6 = HurryUp talalat
//   ID 7+ = szabadon uj esemenyekhez (uj triggert is tehetsz a jateklogikaba)
//
// TIPP: az effekt vegere tegyel par halvanyulo kockat, kulonben az utolso
// kocka "odacsattan" a normal jatek-fenyre visszaallaskor.

#define EFFECT_LEDS 68 // a jatekter LED-jei (0..67, lasd LEDMAP.md)

int runningEffect = 0;
unsigned long effectStartT = 0;

// Egy baked effekt lejatszasa (idx = a bakedEffects[] tablabeli sorszam).
void RunBakedEffect(uint8_t idx) {
  if (idx >= bakedEffectCount) { // ismeretlen/ures -> azonnal lezar
    effect = LOW; effectID = 0; runningEffect = 0;
    initlight = HIGH; Initlights();
    return;
  }
  const EffectDef& e = bakedEffects[idx];
  unsigned long elapsed = millis() - effectStartT;
  unsigned int  step    = (e.frameMs > 0) ? (elapsed / e.frameMs) : 0;
  unsigned int  total   = (unsigned int)e.frames * (e.loops ? e.loops : 1);

  if (step < total) {
    uint16_t frame = step % e.frames;
    const uint8_t* p = e.data + (uint32_t)frame * EFFECT_LEDS * 3;
    for (uint8_t i = 0; i < EFFECT_LEDS; i++) {
      leds[i] = CRGB(pgm_read_byte(p), pgm_read_byte(p + 1), pgm_read_byte(p + 2));
      p += 3;
    }
  }
  else { // lejart -> vissza a normal jatek-fenyre
    effect = LOW; effectID = 0; runningEffect = 0;
    initlight = HIGH; Initlights();
  }
}

void RunLightEffect() {
  if (effect != HIGH) {
    runningEffect = 0;
    return;
  }
  if (runningEffect != effectID) { // uj effekt indul
    runningEffect = effectID;
    effectStartT = millis();
  }
  RunBakedEffect(runningEffect - 1); // effectID 1 = bakedEffects[0]
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Fenyeffekt motor
/////////////////////////////////////////////////
/////////////////////////////////////////////////

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
// formatum-specifikacio, az ID->esemeny terkep, az FPS-tabla es a loop/outro
// leirasa is).
//
// Inditas a jateklogikabol (VALTOZATLAN interfesz):  effect = HIGH; effectID = ID;
// Az effekt ID-ja EXPLICIT a leiroban (nem a tabla sorrendje) -> ID szerint
// keresunk. A motor a vegen magatol all vissza (effect = LOW + Initlights).
//
// LOOP + OUTRO: a leiro loopFrames = a ciklus hossza (elso N kocka), ami loops-
// szor ismetlodik; a maradek (frames - loopFrames) EGYSZER lejatszodo outro.

#define EFFECT_LEDS 68 // a jatekter LED-jei (0..67, lasd LEDMAP.md)

int runningEffect = 0;
unsigned long effectStartT = 0;

// Egy baked effekt lejatszasa (idx = a bakedEffects[] tablabeli sorszam).
void RunBakedEffect(uint8_t idx) {
  const EffectDef& e = bakedEffects[idx];
  uint16_t loopLen  = (e.loopFrames == 0 || e.loopFrames > e.frames) ? e.frames : e.loopFrames;
  uint8_t  loops    = e.loops ? e.loops : 1;
  uint16_t outroLen = e.frames - loopLen;
  // uint32_t: AVR-en az int 16 bites, a loopLen*loops (es a step) tulcsordulhat
  uint32_t cycleSteps = (uint32_t)loopLen * loops;
  uint32_t totalSteps = cycleSteps + outroLen;

  unsigned long elapsed = millis() - effectStartT;
  uint32_t      step    = (e.frameMs > 0) ? (elapsed / e.frameMs) : 0;

  if (step < totalSteps) {
    uint16_t frame = (step < cycleSteps) ? (uint16_t)(step % loopLen)          // ciklus
                                         : (uint16_t)(loopLen + (step - cycleSteps)); // outro (1x)
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

  // effekt keresese ID szerint (a leiro id mezoje, nem a tabla-sorrend)
  int8_t idx = -1;
  for (uint8_t i = 0; i < bakedEffectCount; i++) {
    if (bakedEffects[i].id == runningEffect) { idx = i; break; }
  }
  if (idx < 0) { // nincs ilyen ID (meg) -> azonnal, biztonsagosan lezar
    effect = LOW; effectID = 0; runningEffect = 0;
    initlight = HIGH; Initlights();
    return;
  }
  RunBakedEffect(idx);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Fenyeffekt motor
/////////////////////////////////////////////////
/////////////////////////////////////////////////

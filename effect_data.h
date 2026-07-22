#pragma once
// ==========================================================================
//  FENYEFFEKT-ADATOK  (a d_light_effects.ino baked-frame motorja hasznalja)
//  A KULSO SZERKESZTO ide tolti/innen olvassa az effekteket.
// --------------------------------------------------------------------------
//  FORMATUM: minden effekt egy PROGMEM bajt-tomb, KOCKA-MAJOR sorrendben.
//  Kockankent 68 LED (index 0..67, lasd LEDMAP.md), LED-enkent 3 bajt: R,G,B
//  (0..255). Egy kocka = 68*3 = 204 bajt. A tomb hossza = frames * 204.
//  Minden (szin, fenyero/opacity, per-LED fade) ide van "kisutve"; fix idokoz.
//
//  FPS (frameMs egesz ms!):  20 fps = 50 ms (PONTOS, alap) | 25 fps = 40 ms
//   (Pontos csak az 1000 osztoi: 20/25/40/50/100. 24 es 30 nem az.)
//
//  LOOP + OUTRO: loopFrames = az elso N kocka = CIKLUS ("loops"-szor), a tobbi
//  (frames - loopFrames) EGYSZER lejatszodo OUTRO. 0 vagy == frames -> nincs outro.
//
//  OVERLAY / CANVAS MOD (overlay flag):
//   - overlay = 0 (FULL): az effekt atveszi a PALYAT (minden cella rajzolodik,
//     a (0,0,0) = fekete). Inditas:  effect = HIGH; effectID = ID;
//   - overlay = 1 (OVERLAY/CANVAS): az effekt RARAJZOL a normal jatek-fenyre,
//     PER-CELLA / PER-KOCKA donthetsz. Inditas:  PlayOverlay(ID);
//        magenta (255,0,255) = ATLATSZO -> a motor kihagyja, a jatek latszik alatta
//        (0,0,0)             = FEKETE, OPAQUE -> elsotetiti azt a LED-et
//        barmi mas           = szin
//     Igy pl. egy villanas ELEJE csupa (0,0,0) (pillanatnyi elsotetites),
//     a lefutasa meg szinek + magenta (a tobbi LED atlatszo). A szerkeszto
//     "canvas mod"-ban az URES/radirozott cellakat magentakent (255,0,255)
//     exportalja. (Sebesseg: cellankent 1 osszehasonlitas, elhanyagolhato.)
//
//  EGY EFFEKT (ezt csinalja a szerkeszto): egy fx_NEV[] tomb + egy sor a tablaba:
//    { ID, "nev", fx_NEV, KOCKAK, KOCKA_MS, LOOPS, LOOPFRAMES, OVERLAY }
//  Az ID EXPLICIT (nem a sorrend) -> szabadon rendezheto, az ID stabil.
//
//  Trigger-pontok (FULL effektek): ID 1=Kiloves 2=Weed-multiball 3=Weedblast
//   4=Looplight 5=Hid 6=HurryUp. ID 7+ szabad. Az OVERLAY effektekhez a firmware
//   megfelelo pontjaira PlayOverlay(ID) kerul (szolj, melyik esemenyhez).
//
//  MEMORIA: kockak * 68 * 3 bajt / effekt. Szabad flash kb. 200 KB.
// ==========================================================================

struct EffectDef {
  uint8_t     id;         // firmware-ID (a jateklogika ezt hivja)
  const char* name;       // szerkeszto-nev (futasidoben nem kell)
  const uint8_t* data;    // PROGMEM: frames * 68 * 3 bajt (R,G,B, kocka-major)
  uint16_t    frames;     // osszes kocka
  uint16_t    frameMs;    // kocka-idotartam ms (50=20fps, 40=25fps)
  uint8_t     loops;      // a ciklus hanyszor ismetlodjon (1 = egyszer)
  uint16_t    loopFrames; // ciklus hossza (elso N kocka); a tobbi = 1x outro; 0/frames = nincs outro
  uint8_t     overlay;    // 0 = full (atveszi a palyat); 1 = overlay (0,0,0 = atlatszo)
};

// PELDA/sablon (ID 1): 6 kockas szivarvany-villanas, FULL. A szerkeszto lecsereli.
const uint8_t fx_rainbow[] PROGMEM = {
  // 0. kocka
  255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0, 255,0,0,
  // 1. kocka
  255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0, 255,80,0,
  // 2. kocka
  255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0, 255,200,0,
  // 3. kocka
  0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60, 0,220,60,
  // 4. kocka
  0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255, 0,80,255,
  // 5. kocka
  160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220, 160,0,220,
};

const EffectDef bakedEffects[] = {
  // ID  nev         data        kockak  ms  loops loopFrames overlay
  {  1, "rainbow",  fx_rainbow,     6,   100,   1,     6,       0 },  // sablon - csereld le
  // ide told a szerkeszto uj effektjeit
};
const uint8_t bakedEffectCount = sizeof(bakedEffects) / sizeof(bakedEffects[0]);

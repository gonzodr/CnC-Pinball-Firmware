/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Shooteffect | ID: 1
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Shooteffect() {

  if (effectState == HIGH & shooteffbool == LOW & effect == HIGH & effectID == 1) {
      if (shootLightCycle < 7){
      shootLightCycle ++;
      for (int i = 0; i < 68; i++) {
      colorcode = EffectID1[offset];
      offset = offset + 1;

      if (colorcode == 0){
      leds[i] = CRGB::Black;
        }
      if (colorcode == 1){
      leds[i] = CRGB::White;
        }
      }
        FastLED.show();
      }
      if (shootLightCycle == 7){
      offset = 0;
      initlight = HIGH;
      Initlights();
      effect = LOW;
      effectID = 0;
      shooteffbool = LOW;
      shootLightCycle = 0;
      }

    shooteffbool = HIGH;
  }
  else {
    shooteffbool = LOW;
  }
  }
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Shooteffect Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////




/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Weedmultiball | ID: 2
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Weedmultiball() {

  if (effectState == HIGH and weedmultibool == LOW and effect == HIGH and effectID == 2) {
    weedmulticounter = weedmulticounter + 1;
    weedmultibool = HIGH;
  
  
  if (weedmulticounter == 1 or weedmulticounter == 3 or weedmulticounter == 5 or weedmulticounter == 7) {
      if (weedmulticounter == 1){
      }
      for (int i = 0; i < 68; i++) {
          colorcode = EffectID2[offset];
      offset = offset + 1;

      if (colorcode == 0){
      leds[i] = CRGB::Black;
        }
      if (colorcode == 1){
      leds[i] = CRGB::Red;
        }
      }
        FastLED.show();
      }

  if (weedmulticounter == 9 or weedmulticounter == 11 or weedmulticounter == 13 or weedmulticounter == 15) {
      if (weedmulticounter == 9){
      }
      for (int i = 0; i < 68; i++) {
          colorcode = EffectID2[offset];
      offset = offset + 1;

      if (colorcode == 0){
      leds[i] = CRGB::Black;
        }
      if (colorcode == 1){
      leds[i] = CRGB::Orange;
        }
      }
        FastLED.show();
      }

  if (weedmulticounter == 17 or weedmulticounter == 19 or weedmulticounter == 21 or weedmulticounter == 23) {
      if (weedmulticounter == 17){
      }
      for (int i = 0; i < 68; i++) {
          colorcode = EffectID2[offset];
          offset = offset + 1;

      if (colorcode == 0){
      leds[i] = CRGB::Black;
        }
      if (colorcode == 1){
      leds[i] = CRGB::Yellow;
        }
      }
        FastLED.show();
      }

  if (weedmulticounter == 25 or weedmulticounter == 27 or weedmulticounter == 29 or weedmulticounter == 31) {
      if (weedmulticounter == 25){
      }
      for (int i = 0; i < 68; i++) {
          colorcode = EffectID2[offset];
          offset = offset + 1;

      if (colorcode == 0){
      leds[i] = CRGB::Black;
        }
      if (colorcode == 1){
      leds[i] = CRGB::White;
        }
      }
        FastLED.show();
      }
  if (weedmulticounter == 2 || weedmulticounter == 4 || weedmulticounter == 6 || weedmulticounter == 6 || weedmulticounter == 10 || weedmulticounter == 12 || weedmulticounter == 14 || weedmulticounter == 16 || weedmulticounter == 18 || weedmulticounter == 20 || 
  weedmulticounter == 22 || weedmulticounter == 24 || weedmulticounter == 26 || weedmulticounter == 28 || weedmulticounter == 30) {
      for (int i = 0; i < 68; i++) {
        leds[i] = CRGB::Black;
      }
    FastLED.show();
  }

  
  if (weedmulticounter == 32){
      offset = 0;
      initlight = HIGH;
      Initlights();
      effect = LOW;
      effectID = 0;
      weedmultibool = LOW;
      weedmulticounter = 0;
  }
  }
  
  if (effectState == LOW) {
    weedmultibool = LOW;

}

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Weedmultieffect Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Weedblast | ID: 3
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Weedblast() {

  if (effectState == HIGH and weedblastbool == LOW and effect == HIGH and effectID == 3) {
    weedblastcounter = weedblastcounter + 1;
    if (weedblastcounter == 1){
      }
  if (weedblastcounter < 7) {

      for (int i = 0; i < 68; i++) {
          colorcode = EffectID3[offset];
          offset ++;

      if (colorcode == 0){
      leds[i] = CRGB::Black;
        }
      if (colorcode == 1){
      leds[i] = CRGB::White;
        }
      if (colorcode == 2){
      leds[i] = CRGB::Yellow;
        }
      if (colorcode == 3){
      leds[i] = CRGB::Red;
        }
      if (colorcode == 4){
      leds[i] = CRGB::Orange;
        }
      if (colorcode == 5){
      leds[i] = CRGB::Green;
        }
      if (colorcode == 6){
      leds[i] = CRGB::IndianRed;
        }
      if (colorcode == 7){
      leds[i] = CRGB::SkyBlue;
        }
      if (colorcode == 8){
      leds[i] = CRGB::DeepPink;
        }
      if (colorcode == 9){
      leds[i] = CRGB::DarkViolet;
        }
      if (colorcode == 10){
      leds[i] = CRGB::Tomato;
        }
      if (colorcode == 11){
      leds[i] = CRGB::LightSeaGreen;
        }
      if (colorcode == 12){
      leds[i] = CRGB::FairyLight;
        }

      }
      FastLED.show();
    }
  if (weedblastcounter == 7){
      weedblastcounter =0;
      offset = 0;
      initlight = HIGH;
      Initlights();
      effect = LOW;
      effectID = 0;
      weedmultibool = LOW;
      weedmulticounter = 0;
  }
    weedblastbool = HIGH;
  }

  if (effectState == LOW) {
    weedblastbool = LOW;
  }

  }

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Weedmultieffect Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// Looplight | ID: 4
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void Looplight() {

  if (effectState == HIGH and looplightbool == LOW and effect == HIGH and effectID == 4) {
    looplightcounter = looplightcounter + 1;
    looplightbool = HIGH;
  if (looplightcounter == 1 or looplightcounter == 3 or looplightcounter == 5 or looplightcounter == 7){
          for (int i = 0; i < 68; i++) {
      leds[i] = CRGB::DeepPink;
        }
      }
  
  if (looplightcounter == 9 or looplightcounter == 11 or looplightcounter == 13 or looplightcounter == 15){
          for (int i = 0; i < 68; i++) {
      leds[i] = CRGB::DarkViolet;
        }
      }
  
  if (looplightcounter == 17 or looplightcounter == 19 or looplightcounter == 21 or looplightcounter == 23){
          for (int i = 0; i < 68; i++) {
      leds[i] = CRGB::SkyBlue;
        }
      }
  
  if (looplightcounter == 25 or looplightcounter == 27 or looplightcounter == 29 or looplightcounter == 31){
          for (int i = 0; i < 68; i++) {
      leds[i] = CRGB::White;
        }
      }
  
  else if (looplightcounter == 2 || looplightcounter == 4 || looplightcounter == 6 || looplightcounter == 8 || looplightcounter == 10 || looplightcounter == 12 || looplightcounter == 14 || looplightcounter == 16 || looplightcounter == 18 || looplightcounter == 20 || 
  looplightcounter == 22 || looplightcounter == 24 || looplightcounter == 26 || looplightcounter == 28 || looplightcounter == 30){
          for (int i = 0; i < 68; i++) {
      leds[i] = CRGB::Black;
        }
    }
  else if (looplightcounter == 32){
      effect = LOW;
      effectID = 0;
      looplightbool = LOW;
      looplightcounter = 0;
      initlight = HIGH;
      Initlights();
    }
  }


  
  if (effectState == LOW) {
    looplightbool = LOW;
  }

}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Looplight Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// BridgeLowEff | ID: 5
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void BridgeLowEff() {

  if (effectState == HIGH and BridgeLowEffbool == LOW and effect == HIGH and effectID == 5) {
    BridgeLowEffcounter = BridgeLowEffcounter + 1;
    weedmultibool = HIGH;
  if (BridgeLowEffcounter == 1 or BridgeLowEffcounter == 3 or BridgeLowEffcounter == 5 or BridgeLowEffcounter == 7){
      for (int i = 0; i < 68; i++) {
      
      colorcode = EffectID5[offset];
      offset = offset + 1;

      if (colorcode == 0){
      leds[i] = CRGB::Black;
        }
      if (colorcode == 1){
      leds[i] = CRGB::White;
        }
        }
      }
  
  if (BridgeLowEffcounter == 9 or BridgeLowEffcounter == 11 or BridgeLowEffcounter == 13 or BridgeLowEffcounter == 15){
          for (int i = 0; i < 68; i++) {
      
              colorcode = EffectID5[offset];
              offset = offset + 1;

      if (colorcode == 0){
      leds[i] = CRGB::Black;
        }
      if (colorcode == 1){
      leds[i] = CRGB::Red;
        }
        }
      }
  
  else if (BridgeLowEffcounter == 2 || BridgeLowEffcounter == 4 || BridgeLowEffcounter == 6 || BridgeLowEffcounter == 8 || BridgeLowEffcounter == 10 || BridgeLowEffcounter == 12 || BridgeLowEffcounter == 14 || BridgeLowEffcounter == 16){
          for (int i = 0; i < 68; i++) {
      leds[i] = CRGB::Black;
        }
    }
  else if (BridgeLowEffcounter == 17){
      effect = LOW;
      effectID = 0;
      BridgeLowEffbool = LOW;
      BridgeLowEffcounter = 0;
      initlight = HIGH;
      Initlights();
    }    
  }

  
  
  
  
  if (effectState == LOW) {
    BridgeLowEffbool = LOW;
  }
  }

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End Weedmultieffect Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// HurryHit | ID: 6
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HurryHit() {
  int r = random(1, 7);
  if (effectState == HIGH and hurryhitbool == LOW and effect == HIGH and effectID == 6) {
    hurryhitcounter = hurryhitcounter + 1;
    hurryhitbool = HIGH;
  }
  else {
    hurryhitbool = LOW;
  }
  switch (hurryhitcounter) {
    case 1:
      if (r == 1){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Blue; // Slingshot1
        }
      }
      if (r == 2){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Green; // Slingshot1
        }
      }
      if (r == 3){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Yellow; // Slingshot1
        }
      }
      if (r == 4){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Purple; // Slingshot1
        }
      }
      if (r == 5){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Red; // Slingshot1
        }
      }
      if (r == 6){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::White; // Slingshot1
        }
      }
      break;
    case 2:
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Black; // Slingshot1
        }
      break;
    case 3:
      if (r == 1){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Blue; // Slingshot1
        }
      }
      if (r == 2){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Green; // Slingshot1
        }
      }
      if (r == 3){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Yellow; // Slingshot1
        }
      }
      if (r == 4){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Purple; // Slingshot1
        }
      }
      if (r == 5){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Red; // Slingshot1
        }
      }
      if (r == 6){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::White; // Slingshot1
        }
      }
      break;
    case 4:
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Black; // Slingshot1
        }
      break;
    case 5:
      if (r == 1){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Blue; // Slingshot1
        }
      }
      if (r == 2){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Green; // Slingshot1
        }
      }
      if (r == 3){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Yellow; // Slingshot1
        }
      }
      if (r == 4){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Purple; // Slingshot1
        }
      }
      if (r == 5){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Red; // Slingshot1
        }
      }
      if (r == 6){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::White; // Slingshot1
        }
      }
      break;
    case 6:
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Black; // Slingshot1
        }
      break;
    case 7:
      if (r == 1){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Blue; // Slingshot1
        }
      }
      if (r == 2){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Green; // Slingshot1
        }
      }
      if (r == 3){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Yellow; // Slingshot1
        }
      }
      if (r == 4){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Purple; // Slingshot1
        }
      }
      if (r == 5){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::Red; // Slingshot1
        }
      }
      if (r == 6){
      for (int i = 0; i < 68; i++) {
          leds[i] = CRGB::White; // Slingshot1
        }
      }
      break;
    case 8:
      initlight = HIGH;
      Initlights();
      effect = LOW;
      effectID = 0;
      hurryhitbool = LOW;
      hurryhitcounter = 0;
      break;
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//// End HurryHit Rutin
/////////////////////////////////////////////////
/////////////////////////////////////////////////

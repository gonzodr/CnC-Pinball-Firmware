# CnC Pinball - LED terkep (WS2812B szalag, 0..114)

A jatekter LED-jei: **0..67** (ezeket vezerli a jatek + az effektek).
A 68..114 a `FillLEDsFromPaletteColors()` altalanos (attract/hatter) fenye.
A nevek a firmware `LED_*` #define-jai (Initlights az autoritativ forras).

## Bal oldal - CnC (fent -> le)

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 17 | LED_CNC_AMBIENT | CnC ambient |
| 18 | LED_CNC_C1 | C betu (1.) |
| 19 | LED_CNC_AMP | kozepso ("&" / N) |
| 20 | LED_CNC_C2 | C betu (2.) |

## Kozep - Weed (W-E-E-D)

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 29 | LED_WEED_W | W |
| 30 | LED_WEED_E1 | E (1.) |
| 31 | LED_WEED_E2 | E (2.) |
| 32 | LED_WEED_D | D |

## Jobb oldal - Fishtank (fent -> le)

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 48 | LED_FISH | Fish |
| 49 | LED_TANK | Tank |
| 50 | LED_FISHTANK_AMBIENT | Fishtank ambient |

## DAVE savok

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 2  | LED_DAVE_V | V |
| 3  | LED_DAVE_E | E |
| 15 | LED_DAVE_A | A |
| 16 | LED_DAVE_D | D |

## Bonusszorzo + Ballsave

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 6  | LED_BONUS_X8 | x8 |
| 7  | LED_BONUS_X6 | x6 |
| 9  | LED_BONUS_X4 | x4 |
| 10 | LED_BONUS_X2 | x2 |
| 8  | LED_BALLSAVE | Ballsave |

## Loopok

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 21 | LED_LEFT_LOOP_1 | Bal loop |
| 22 | LED_LEFT_LOOP_2 | Bal loop |
| 44 | LED_RIGHT_LOOP_2 | Jobb loop 2 |
| 45 | LED_RIGHT_LOOP_1 | Jobb loop 1 |

## Rampak

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 23 | LED_LEFT_RAMP_AMBIENT | Bal rampa ambient |
| 24 | LED_LEFT_RAMP_1 | Bal rampa (kis hid) |
| 25 | LED_LEFT_RAMP_2 | Bal rampa (kis hid) |
| 34 | LED_RRAMP_ARROW_22 | Jobb rampa nyil 2/2 |
| 35 | LED_RRAMP_ARROW_21 | Jobb rampa nyil 2/1 |
| 36 | LED_RRAMP_ARROW_12 | Jobb rampa nyil 1/2 (nagy hid) |
| 37 | LED_RRAMP_ARROW_11 | Jobb rampa nyil 1/1 (nagy hid) |
| 51 | LED_RIGHT_RAMP_AMBIENT | Jobb rampa ambient |

## Spinner / Get High

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 27 | LED_GET_HIGH_1 | Get High |
| 28 | LED_GET_HIGH_2 | Get High |

## Chong / Cheech (indikator + gyujtogeto figurak)

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 26 | LED_CHONG_IND | Chong indikator |
| 33 | LED_CHEECH_IND | Cheech indikator |
| 38 | LED_CHEECH_WHEEL | Cheech kerek |
| 39 | LED_CHONG_PIPE | Chong pipa |
| 40 | LED_CHONG_ROACH | Chong csotany |
| 41 | LED_CHEECH_CIGAR | Cheech szivar |
| 46 | LED_MUF_DVR | MUF dvr |
| 47 | LED_CHONG_KEY | Chong plunger-kulcs |

## UFO

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 42 | LED_UFO_ARROW_1 | UFO nyil 1 |
| 43 | LED_UFO_ARROW_2 | UFO nyil 2 |

## Gate-ek (kapuk)

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 52 | LED_GATE3_AMBIENT | Gate 3 ambient |
| 53 | LED_GATE3 | Gate 3 |
| 54 | LED_GATE32_AMBIENT | Gate 3-2 ambient |
| 55 | LED_GATE2 | Gate 2 |
| 56 | LED_GATE21_AMBIENT | Gate 2-1 ambient |
| 57 | LED_GATE1 | Gate 1 |
| 58 | LED_GATE1_AMBIENT | Gate 1 ambient |

## Weed-mero (magas asztal)

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 59 | LED_MICHOAKAN | Michoakan |
| 60 | LED_ACAPULCO | Acapulco Gold |
| 61 | LED_HIGHTABLE_FLASHER | Hightable flasher |
| 62 | LED_THAI | Thai stick |
| 63 | LED_LABRADOR | Labrador |

## Pop bumperek + auto

| # | LED_ nev | Jelentes |
|---|----------|----------|
| 64 | LED_POP1 | Pop 1 |
| 65 | LED_POP2 | Pop 2 |
| 66 | LED_POP3 | Pop 3 |
| 67 | LED_CAR_AMBIENT | Auto ambient |

## Nevesitetlen (skill lane / rollover - Initlightsban feher, nincs kodban hasznalva)

| # | Megjegyzes |
|---|-----------|
| 0, 1, 4, 5 | felso savok / rollover (feher) |
| 11, 12, 13, 14 | savok (feher) |

---
*A gift-fazis futofeny (`GiftRunlight`) ket csovaja:*
*bal = 30,29,18,19,20 (E-W-C-N-C) - jobb = 31,32,48,49 (E-D-Fish-Tank)*

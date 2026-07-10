# Serial parancs -> video terkep (a Unity-s CnC GUI V2-bol kinyerve)

Forras: CamTimer.cs (F:\Projects\CnC GUI V2\Assets\Scripts\, masolat a
Python GUI projekt gyokereben) + a Unity Inspector kifotozott bekotesei
(2026-07-10). Ez az ETALON: a gepben futo (mini PC-s!) firmware-nek
ehhez kell(ett) igazodnia.

## FONTOS FIGYELMEZTETES

A mini PC-n (a flipperben) valo'szinuleg UJABB firmware van, mint az
itteni CnC_firmware3 (a V4 alapja)! Bizonyitekok:
 - a Unity GUI Ufo1..Ufo13 parancsokat kezel, a fw3 csak Ufo1-7+9-et kuld
 - a Unity Point1..Point8-at kezel, a fw3 csak Point1-2-t kuld
 - a Unity ismeri a Jackpot1-et (PsyJackpot) es az ExtraB-t, a fw3 nem kuldi
=> AMIG A MINI PC-S FORRAST NEM VETETTUK OSSZE A V4-GYEL, AZ ELES GEPRE
   NE MENJEN FEL A V4! (A mai javitasok mind git-ben vannak, atemelhetok.)

## A Unity CamTimer altal kezelt parancsok (62 db)

score (7 mezos pontszamuzenet), Next, End, Start, Left, Right, Shoot,
Point1-8, Jackpot1-6, Multiball1-4, Bonus1-4, Beer1-3, Combo1-6,
ChongC1-3, CheechC1-3, Ufo1-13, ExtraB, Weed, Drift, Danger, Tilt

Megjegyzes: a "Zero" NINCS a Unity listaban - az a Python GUI-s koszak
uzenete lehet.

## Parancs -> video hozzarendelesek (Unity Inspector alapjan)

| Parancs | Video fajl | Megjegyzes |
|---------|-----------|------------|
| Point1..8 | 2500, 5000, 7500, 10000, 15000, 20000, 25000, 30000 | pontertek-videok |
| Jackpot1 | PsyJackpot | a fw3/V4 SOSEM kuldi! |
| Jackpot2..6 | Jackpot2..6 | |
| Multiball1 | MichokanM | |
| Multiball2 | Acapulco Gold | |
| Multiball3 | Thai Stick | |
| Multiball4 | Thai Stick | gyanus dupla - Labrador kellett volna? |
| Bonus1..4 | Bonus1..4 | |
| ExtraB | ExtraB | a fw3/V4 sosem kuldi (extra ball nema) |
| Ufo1..5 | ufo1..ufo5 | |
| Ufo6 | **Ufofuck** | ELCSUSZAS! a 6-os trigger a "nyeretlen" video |
| Ufo7 | **ufo6** | ezert nincs Ufo7.mp4 fajl! |
| Ufo8..13 | ufo8..ufo13 | a fw3/V4 csak a 9-est kuldi ezekbol |
| Beer1..3 | Beer1..3 | |
| Combo1..6 | Combo1..6 | |
| ChongC1..3 / CheechC1..3 | ChongC1..3 / CheechC1..3 | |
| Weed / Drift / Danger / Tilt | azonos nevu videok | |
| (Start Scr) | PressStarttoPlay | attract kepernyo |
| (Intro Scr) | IntroScr | |
| (Thx Scr) | Thanksto | |

## Kovetkezmeny a Python GUI-ra (V3)

A Python GUI a trigger NEVEVEL azonos nevu fajlt keresi. Emiatt ket
UFO-video biztosan rosszul megy:
 - "Ufo6" triggerre Ufo6.mp4-et jatszik, pedig a Ufofuck.mp4 kellene
   (a "nem nyertel semmit" eset)
 - "Ufo7" triggerre Ufo7.mp4-et keresne, ami NEM LETEZIK (ufo6.mp4 kellene)
=> a Python GUI-ba egy nev-atkepezo tabla kell (Ufo6->Ufofuck, Ufo7->ufo6),
   VAGY a mini PC-s firmware-ben mar at van szamozva - ellenorizni!

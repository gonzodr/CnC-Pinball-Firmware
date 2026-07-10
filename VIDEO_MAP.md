# Serial parancs -> video terkep (a Unity-s CnC GUI V2-bol kinyerve)

Forras: CamTimer.cs (F:\Projects\CnC GUI V2\Assets\Scripts\, masolat a
Python GUI projekt gyokereben) + a Unity Inspector kifotozott bekotesei
(2026-07-10). Ez az ETALON: a gepben futo (mini PC-s!) firmware-nek
ehhez kell(ett) igazodnia.

## MEGOLDVA (2026-07-10): a mini PC-s forras elokerult!

A gepben futott forras a repo "Régi fullos firmvare/CnC_firmware3"
mappajaban van megorizve. A V4-be atemelve belole:
 - FORDITOTT golyoerzekeles (<100 = golyo ott van) - a gep valodi bekotese!
 - UFO pontlopas-nyeremeny (lottery 8, csak tobbjatekosnal): -10000 pont
   egy veletlen masik jatekosnak; uzenet: Ufo10..13 (= kirabolt jatekos),
   hang: 124..127 + 91 (firework)
 - maxBallSw: SpaceCoke-ballsave alatt az utolso tarolt golyo 1 mp
   kesleltetessel megy ki
NEM emeltuk at (a V4/fw3 verzioja maradt): resetFunc() helyett A13-as
reset-vonal; lottery 6 = 25000/Ufo3 (a gepiben ures "nyeretlen" volt);
reset-kombo start+shoot 20 mp.
A gepi verzio bugjai, amiket a portolas javitott: playerSucks==1 duplikatum
(3 jatekosnal), score alulcsordulas 10000 pont alatti aldozatnal.

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
| Ufo8 | ufo8 | egyik firmware sem kuldi (arva video) |
| Ufo9 | ufo9 | SpaceCoke multiball |
| Ufo10..13 | ufo10..ufo13 | pontlopas: a kirabolt jatekos (1..4) videoja |
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

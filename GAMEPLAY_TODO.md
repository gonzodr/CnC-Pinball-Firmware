# CnC Pinball - gameplay TODO

A reszletes indoklas es az esemenyenkenti leltar a `GAMEPLAY_AUDIT.md` fajlban
van. Ez a lista a jelenlegi jatek rendbetetelehez es az utana kovetkezo
WEED-stack fejleszteshez tartozik. A tavoli collectibles-kapu/VUK bovites nincs
ebben a merfoldkoben.

## P0 - audit es forrasigazsag

- [ ] A balance pass utan a teljes SIM-demot ujra futtatni es az uj,
      szandekosan megvaltozott pontozasi golden baseline-t rogzitani.

- [ ] WAV Trigger SD-kartya teljes track-manifestje: ID, fajlnev, tartalom,
      hossz, hangerő, hasznalo esemenyek.
- [ ] A tenyleges videoasset-keszlet manifestje frame-szammal es hosszal.
- [ ] Minden video elso/kozepso/utolso kockajanak vizualis tartalomauditja.
- [ ] Dontes az arva triggerekről: `Point1`, `Point3..8`, `Jackpot1`.
- [ ] Az UFO regi, elcsuszott neveinek kesobbi migracios terve
      (`Ufo6`/`Ufo7` aliasok megorzese a kompatibilitashoz).

## P0 - hianyzo visszajelzesek

- [x] Tilt firmware: ket warning utan a harmadik kulon kontaktus Tilt.
- [x] Tilt firmware: uj warning csak 750 ms stabilan nyitott kontaktus utan.
- [x] Tilt firmware: warningok golyonkent nullazodnak.
- [x] Tilt firmware: Tilt eseten nincs golyovegi bonusz.
- [ ] Multiball1 video: `Michokan`.
- [ ] Multiball2 video: `Acapulco Gold`.
- [ ] Multiball3 video: `Thai Stick`.
- [ ] Multiball4 video: `Labrador`.
- [ ] Jackpot2..Jackpot6 video.
- [x] Cheech es Chong hid-combo videok: karakterenkent 6 sequence,
      2500/5000/7500/10000/15000/20000 ponttal (2026-08-24).
- [x] Oldalspecifikus combo trigger: kishid = Chong, nagyhid = Cheech.
- [x] `Ufo8` bekerult a normal UFO-lotterybe mint `EXTRA BALL LIT`.
- [x] A nagy hid/high ramp begyujti a lit extra ballt es `ExtraB` triggert
      kuld; az azonnali `Ufo5` extra ball tovabbra is megmaradt.
- [x] Az `Ufo5` es `Ufo8` lottery-kimenet tiltott, ha mar van eltett vagy
      kigyujtesre varo extra ball.
- [ ] Dedikalt `EXTRA BALL LIT` fenyjelzes a high rampon.
- [ ] `ExtraB` collect videoasset elkeszitese.
- [ ] Extra ball collect light show es dedikalt callout.
- [ ] A WEED-stack bevezetesekor az azonnali extra ball es az extra-ball-lit
      lottery-sulyanak/retegenek meghatarozasa.
- [x] Uj `Danger` video jelen van: 102 frame (2026-08-24).
- [x] Az elso warning a `Danger` videot kapja.
- [x] Uj `Danger2` video jelen van: 180 frame; a masodik warning ezt kapja
      (2026-08-25).
- [x] Mindket Danger warning az ID11 Tilt-fenyeffektet harom teljes korig
      jatsza.
- [x] Tilt video: 180 frame, 30 fps; a 0-59 intro egyszer fut le, majd a
      60-179 szakaszt a GUI a drain `Next`/`End` uzeneteig loopolja.
- [x] Tilt alatt az ID11 Tilt-fenyeffekt a drain erzekeleseig loopol.
- [ ] C&C complete: feny + hang + `SHOOT CHEECH OR CHONG` GUI-uzenet.
- [ ] WEED complete: `SHOOT UFO OR SPINNER` GUI-uzenet.
- [ ] DAVE complete: `BALL SAVE LIT` feny/hang/GUI-uzenet.
- [ ] Combo alatt az ellenkezo híd nyilai villogjanak az 5,5 masodperces ablakban.
- [x] Dedikalt Chong combo-fenyeffekt (ID9) a kishid sikeres kombotalalatara.
- [x] Dedikalt Cheech combo-fenyeffekt (ID10) a nagyhid sikeres kombotalalatara.
- [ ] Light Editorban canvasra allitani es ujraexportalni az ID7 ChongCollect,
      ID8 CheechCollect es ID9 ComboLowBridge effekteket, ha ezeknek is a
      jatekfenyre kell rajzolniuk. Jelenleg csak az ID10 ComboHighBridge canvas.
- [ ] Hurry Up folyamatos GUI-statusz es visszaszamlalas.
- [ ] Spinner progressz a GUI-n is latszodjon.
- [ ] Multiball alatt `SHOOT JACKPOTS` statusz.

## P0 - pontozas rendbetetele

- [x] A pont- es bonuszertekek nevesitett `Scoring` profilban vannak.
- [x] Spinner: 250 direkt/+10 bonus.
- [x] Sling: 250/+25.
- [x] Pop bumper: 250/+10.
- [x] DAVE lane es completion: nincs pont; a feature egyetlen jutalma a
      legalabb 10 s ball save.
- [x] Minden UFO/VUK-kidobas a tekercs indulasakor legalabb 5 s vedelmi
      ball save-et kap; SpaceCoke multiballnal marad a 30 s.
- [x] Gate rollover: 500/+25.
- [x] C&C complete: 5000/+500.
- [x] WEED complete: 5000/+500.
- [x] FISH/TANK cel: 1500/+50.
- [x] FISH+TANK par: 2500/+250.
- [x] Normal híd: 1000/+100.
- [x] Normal loop: 2500/+250.
- [x] Combo bonusgorbe: 100/150/200/250/300/500.
- [ ] Kis/nagy híd jackpot-arany fizikai nehezseg alapjan.
- [ ] Bonus maximum vagy elvart golyovegi bonusztartomany meghatarozasa.
- [x] Hurry Up kozponti szabaly: minden direkt pont 2x, a bonusz valtozatlan,
      mikro-switch video- es effekt-ujrainditas nelkul.

## P1 - GUI allapot/HUD

- [ ] Firmware->GUI allapotprotokoll megtervezese.
- [ ] `WEED n/4`.
- [ ] `GET HIGH n/180` vagy szazalek.
- [ ] `UFO LIT`.
- [ ] `BEER n/3`.
- [ ] `SHOOT CHEECH OR CHONG`.
- [ ] `COMBO: SHOOT LEFT/RIGHT - n.n s`.
- [ ] `BALL SAVE n s`.
- [ ] `HURRY UP n s` es az aktiv pontszabaly.
- [ ] `MULTIBALL n - SHOOT JACKPOTS`.
- [ ] A HUD prioritasi szabalyai: veszely > jackpot > idozitett mod > fo cel >
      gyujtesi progressz.

## P1 - fenyrendszer

- [ ] Egységes palyafeny-szotar dokumentalasa es alkalmazasa.
- [ ] Multiball-start feny.
- [ ] Kulon híd-jackpot overlay; ne a Weedblast legyen altalanos jackpot-effekt.
- [ ] SpaceCoke start feny.
- [ ] Cheech/Chong harmadik collectible completion feny.
- [ ] Effect ID1 overlay/full inditas ellenorzese es javitasa, ha szukseges.
- [x] Hurry Up talalat nem inditja ujra minden mikro-switchnel a teljes palyas
      effektet.

## P1 - szabalykommunikacio

- [ ] Gift: sajat `GIFT - SHOOT FLASHING TARGET` callout/HUD.
- [ ] Gift ne tunjon el csendben egy rossz celponttol; legyen tartos vagy
      idozitett, es a kudarc legyen egyertelmu.
- [ ] Cheech/Chong harmadik collectible utan ne legyen nema azonnali reset;
      legalabb completion celebration es allapotjelzes kell.
- [ ] Multiball end rovid HUD-uzenet.
- [ ] Golyovegi bonus-count latvany/hang tervezese (nem blokkoló).

## P1 - teszt es telemetria

- [ ] SIM-demo bovítese C&C, FISH/TANK, DAVE, gate/Bonus X, combo, Hurry Up,
      mind az ot multiball es minden UFO-jutalom lefedesere.
- [ ] LED-esemenyek tesztelheto trace-e SIM_MODE-ban, hogy a regresszio a
      fenyinditasokat is lassa.
- [ ] Pontforras-telemetria: esemenyazonosito, jatekos, direkt pont, bonus.
- [ ] 20-30 teljes tesztparti rogzítese.
- [ ] Meres: atlagpont/golyo, median, feature-gyakorisag, extra ball gyakorisag,
      pontforrasok szazalekos megoszlasa.
- [ ] A meres alapjan masodik balanszkor.
- [ ] Tilt-harang gepi teszt: ket kulon kontaktus Danger, harmadik Tilt.
- [ ] Egyetlen hosszan kilengo harang ne adjon tobb warningot.
- [ ] Tilt utan a golyovegi bonusz ne adodjon hozza a score-hoz.

## P2 - WEED-stack merfoldko

- [ ] A bankolo híd kivalasztasa.
- [ ] Jatekosonkenti `weedStack` (0..3) es `weedQualified` allapot.
- [ ] Egy WEED-kor utan az elso UFO/spinner/híd loves fogyassza a kvalifikaciot.
- [ ] Stack 1: alacsony szintu lottery.
- [ ] Stack 2: Extra Ball / Hurry Up / Munchies sorsolas.
- [ ] Stack 3: garantalt SpaceCoke multiball.
- [ ] Extra Ball jatekosonkenti limit es helyettesito jutalom.
- [ ] Stack megmaradasi szabaly golyok kozott.
- [ ] Palyafenyek es GUI-HUD mindharom valasztashoz.
- [ ] SIM-tesztek minden valasztasi sorrendre es drainre.

# Arduino Mega flash hibakeresés — MEGOLDVA (2026-07-24)

## A lényeg egy bekezdésben

A `CnC_firmware4` nem volt felflashelhető: az avrdude a flash írás **28%-ánál** elhalt
timeouttal, **minden gépen ugyanott**. Nem a méret, nem az idő, nem a bootloader és nem
a hex elrendezése volt az ok, hanem az, hogy a panel USB-soros útja **érzékeny konkrét
bájtmintákra**. Egyetlen 256 bájtos lap effekt-adat (`0x4800`) megbízhatóan megölte az
átvitelt — ugyanaz a lap, ugyanarra a címre, XOR-maszkkal viszont hibátlanul felment.

**A javítás:** az `effect_data.h` `fx_*` tábláit XOR 0x5A-val tároljuk, és olvasáskor
visszafejtjük. A firmware azóta fent van a Megán, bitre pontosan ellenőrizve.

---

## A gyökérok

A gyenge láncszem **a panelen (vagy a kábelben) van**, nem a hoszton — ezért volt teljesen
gépfüggetlen a hiba. Eredeti Arduino Mega 2560 (`VID_2341` / `PID_0042`), tehát ATmega16U2
az USB-soros híd, és annak a 2560-hoz menő UART-ja minden gépnél ugyanaz.

A 28% pontosan ott van, ahol a fordított kód véget ér és elkezdődik az `effect_data.h`
LED-adata.

### Tipikus hibaüzenetek (referenciának)
```
Error: timeout / timeout communicating with programmer
Error: failed to execute command 0x13   (CMD_PROGRAM_FLASH_ISP - flash írás)
Error: failed to execute command 0x06   (CMD_LOAD_ADDRESS)
Error: failed to execute command 0x11   (CMD_LEAVE_PROGMODE_ISP)
Error: unable to set load address / unable to leave programming mode
```
(A korábbi verzió ezt félrecímkézte: a **0x06** a load address, a 0x11 a progmode elhagyása.)

---

## A bizonyítékok

Minden mérés a laptopon, ugyanazon a kábelen és porton (COM3):

| teszt | méret | eredmény |
|---|---|---|
| 64 KB-os dummy (magas entrópiájú adat) | 64 180 B | ✅ 10,32 mp |
| **valódi firmware, teljes** | 64 242 B | ❌ **28% / 8,02 mp** |
| csak a „farok", 0x4700-tól | 46 066 B | ❌ **0%-nál** |
| 60–61. lap (fordított kód) | 512 B | ✅ 0,8 mp |
| 71. lap | 256 B | ✅ |
| **72. lap (0x4800)** | 256 B | ❌ **113,9 mp timeoutban** |
| 73. lap (178 × 0xFF sorozat) | 256 B | ✅ |
| szintetikus: csupa 0x00 / 128×0x00 / 64×0xFF / véletlen | 256 B | ✅ mind |
| 72. lap, minden 0x00 → 0x01 | 256 B | ❌ |
| 72. lap, 0xFF → 0xFE, ill. 1..254-re szorítva | 256 B | ❌ |
| **72. lap XOR 0xFF, ill. XOR 0x5A** | 256 B | ✅ **mindkettő** |
| **teljes farok (180 lap) XOR 0x5A** | 46 066 B | ✅ **8,1 mp** |
| **javított firmware, teljes** | 64 282 B | ✅ **10,35 mp + verify OK** |

A Pi-n ugyanez 28% / 8,33 mp-nél halt el, kétszer — vagyis **ugyanaz a lap**.

### Amit ezek kizárnak
- ❌ Nem a méret — 64 180 B dummy simán felmegy.
- ❌ Nem az idő és nem halmozódás — a farok önmagában, 0%-nál bukik.
- ❌ Nem a hex elrendezése — mindkét bináris 1 összefüggő szekció 0x0-tól, nincs lyuk,
  nincs csupa-0xFF lap, nincs címugrás.
- ❌ Nem a nullák — a csupa-nulla lap és a 128 bájtos 0x00-sorozat is felmegy.
- ❌ Nem a hosszú azonos-bájt sorozat általában — a 178 × 0xFF-es lap felmegy.
- ❌ Nem avrdude-verzió-függő — a Pi más verzióval ugyanígy bukott.
- ⚠️ **A korábbi „nem a Mega" állítás TÉVES volt.** A link igenis marginális, csak
  adatfüggően bukik, ezért nem látszott a méret-tesztekben.

---

## Amit a javítás csinál

Ág: **`fix/effect-data-xor-mask`**, két commit:

```
50b3dfa  Fail the build when effect_data.h is not XOR-masked
67c6c45  Store baked effect data XOR-masked to survive the flash upload
```

- **`effect_data.h`** — a 3 `fx_*` tömb mind a 21 624 bájtja XOR 0x5A-val **tárolva**,
  plusz egy `#define FX_DATA_MASK_APPLIED 0x5A` marker a fejlécben.
- **`d_light_effects.ino`** — `FX_DATA_MASK` + `fx_read()`, és a 3 olvasási pont
  (`RunBakedEffect`, `RunOverlayEffect`, `RunLightTest`) ezt használja. A magenta
  átlátszóság-sentinel (255,0,255) vizsgálata a **dekódolt** értékeken fut, tehát
  változatlanul működik.
- **`mask_effect_data.ps1`** — új szkript, ami újra rá tudja tenni (vagy le tudja venni,
  `-Unmask`) a maszkot. Kell, mert az `effect_data.h` **generált fájl**!
- **`#error` őr** — ha valaki újragenerálja az `effect_data.h`-t és elfelejti a maszkolást,
  a build megáll. Enélkül néma hiba lenne: lefordulna, felmenne, csak rossz színekkel.

Költség: **+40 bájt flash**, RAM változatlan, LED-enként 3 `eor` utasítás (kockánként ~13 µs).

### Ha újragenerálod az effekteket a CnC Light Editorral
```powershell
.\mask_effect_data.ps1            # maszkolás
.\mask_effect_data.ps1 -Unmask    # vissza sima adatra
```
Vagy — jobb megoldás — **maga az editor írja ki maszkolva**, és mellé a
`#define FX_DATA_MASK_APPLIED 0x5A` sort is. A define nélkül a build `#error`-ral megáll.

**Fenntartás a 0x5A-ról:** ez nem elvi megoldás, hanem egy összekeverés, ami történetesen
elkerüli a rossz mintát. Egy konstans XOR nem töri meg a sorozatokat, csak áttolja őket
másik értékre. Ha egy jövőbeli effekt-készlet megint elakad, **elég a konstanst átírni**
(`MASK` a szkriptben + `FX_DATA_MASK` a `d_light_effects.ino`-ban, ugyanarra). Ha az sem
elég, a robusztus lépés egy pozíciófüggő maszk (a bájt indexéből származtatva) — az tényleg
feldarabolja a sorozatokat, cserébe olvasáskor kell egy `p - e.data` kivonás bájtonként.

---

## Aktuális állapot

- ✅ **A valódi firmware fent van a Megán**, 64 282 bájt, `flash:v` verify-jal ellenőrizve.
- ⚠️ **A két commit még csak a `fix/effect-data-xor-mask` ágon van.** A Pi a `main`-ről húz
  (`firmware_update.py`: git pull → compile → upload), tehát amíg nincs mergelve és
  felpusholva, a Pi a régi, maszkolatlan adatot fordítja — és ugyanúgy elhal 28%-nál.
- ⚠️ A `CnC_firmware4.ino`-ban van egy **commitolatlan, duplikált** `#include <wavTrigger.h>`
  az 1. sorban (a 39. sorban már benne van). Hatástalan, szándékosan nem lett commitolva.
- ⚠️ **A Pi-n a `cnc-pinball.service` LE VAN ÁLLÍTVA**, vissza kell indítani.
- ⚠️ A Mega az utolsó méréskor **le volt csatlakoztatva** (eltűnt a COM3). Visszadugás után
  érdemes egy záró verify-t futtatni.

## Ami hátravan

```bash
git -C "C:/Users/user/Desktop/Firmware/CnC_firmware4" checkout main
```
```bash
git -C "C:/Users/user/Desktop/Firmware/CnC_firmware4" merge --ff-only fix/effect-data-xor-mask
```
```bash
git -C "C:/Users/user/Desktop/Firmware/CnC_firmware4" push
```
- `sudo systemctl start cnc-pinball.service` a Pi-n
- **Kábelcsere továbbra is megéri.** A XOR elfedi a hibát, nem javítja — és ugyanezen a
  linken megy futás közben a Pi↔Mega kommunikáció is (a GUI `LT,<id>` parancsai, a
  pontszám-üzenetek). Egy marginális link játék közben is akadozhat.

---

## Hasznos elérési utak és parancsok

**Laptop:**
```
arduino-cli : C:\Users\user\AppData\Local\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe
avrdude     : C:\Users\user\AppData\Local\Arduino15\packages\arduino\tools\avrdude\8.0.0-arduino1\bin\avrdude.exe
firmware    : C:\Users\user\Desktop\Firmware\CnC_firmware4
FQBN        : arduino:avr:mega          port: COM3
```

```
arduino-cli board list
arduino-cli compile --fqbn arduino:avr:mega <sketch-mappa>
arduino-cli upload -p COM<N> --fqbn arduino:avr:mega <sketch-mappa> -v
```
A `-v` fontos: abból látszik, hány százaléknál és milyen hibával hal el.

Flash ellenőrzése újraírás nélkül (PowerShellben a `${hex}` kapcsos zárójel kell, különben
a `$hex:` scope-minősítőnek számít):
```powershell
& $avr -C $conf -p atmega2560 -c wiring -P COM3 -b 115200 -U "flash:v:${hex}:i"
```

**Könyvtárak:** FastLED + wavTrigger. A wavTrigger **módosított példány** kell legyen,
amiben `__WT_USE_SERIAL1__` aktív (ellenőrizve: az). Friss library-manager példány ROSSZ
binárist adna.

**Raspberry Pi:** `gonzodr@192.168.1.188`, jelszót a user adja meg chatben. Firmware:
`~/CnC_firmware4`, az F7-es flash a GUI-ból indul.
```
plink -batch -ssh -pw <jelszo> -hostkey "SHA256:ydiGCz518imgu1CCHmDCS9nZOgDWy1hLtak57imV8JU" gonzodr@192.168.1.188 "<parancs>"
```

---

## Amit NE csinálj (bizonyított zsákutcák)

- **Ne öld a futó avrdude-ot** flashelés közben. A panel beragad tőle, és onnantól
  minden mérés hamis lesz (a csupa-nulla kontroll-lap is „hibázik"). Hagyd természetesen
  befejeződni — egy bukás ~113 mp. Egy teljes, sikeres feltöltés kimozdítja belőle.
- Ne tedd be a `dwc_otg.speed=1`-et a Pi `cmdline.txt`-jébe — megöli az összes low-speed
  USB eszközt (billentyűzet/egér nem enumerálódik).
- Ne táplálj Arduinót második USB-kábelről (back-feed / földhurok, elrontja a Pi USB-jét).
  Ha külön táp kell: barrel jack (7–12 V) vagy VIN.
- Ne kapcsold ki a Pi-t áramtalanítással config-szerkesztés után — a FAT boot partícióra
  nem íródik ki, tiszta `sudo reboot` kell.
- A Pi tartósan under-voltage (`vcgencmd get_throttled` = `0x50005`). Valós, de **nem ez**
  a flash-hiba oka — a laptopon is ugyanúgy bukott.

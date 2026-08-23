# Munchies Abduction VUK-protokoll

A Mega és a Raspberry Pi GUI 115200 baudon, újsorral lezárt ASCII sorokkal
kommunikál. A teljes megvalósítás a `g_munchies_mode.ino` fájlban van; a közös,
nem blokkoló sorolvasó a `d_light_effects.ino` `PollControlSerial()` rutinja.

## Session menete

1. Kigyújtott WEED és normál, egygolyós mód mellett az UFO-lottery 9-es
   kimenete indítja a minijátékot (egy játékosnál 1/8, több játékosnál 1/9
   eséllyel). A Mega a VUK-ban tartja a golyót, letiltja a két
   flippertekercset és elküldi az `MG_START,<session>` sort.
2. A GUI aktiválja az előtöltött minijátékot és `MG_READY,<session>` választ ad.
3. A Mega gombváltozáskor azonnal, egyébként 50 ms-onként küldi az
   `MG_INPUT,<session>,<seq>,<mask>` snapshotot. A mask bitjei: bal=1, jobb=2,
   kilövő/vonósugár=4.
4. A GUI 500 ms-onként `MG_ALIVE` heartbeatet küld. Jó/rossz pickupnál
   `MG_PICKUP`, teljes collectionnél `MG_COLLECTION` indítja a fizikai
   fényréteget.
5. Az eredmény után `MG_DONE,<session>,<bonus>` érkezik. A Mega pontosan
   egyszer írja jóvá a pontot, `MG_ACK`-ot küld és 50 ms-os VUK-impulzussal
   visszaadja a golyót. A duplikált DONE csak új ACK-ot kap, újabb pontot nem.

## Watchdogok és mechanikai védelem

- READY timeout: 3 s
- Pi/GUI heartbeat timeout: 5 s (egy mpv → DRM kijelző-visszavételt is elbír)
- teljes session hard limit: 230 s
- sikertelen első VUK-lökésnél egy második 50 ms-os impulzus
- az UFO kapcsolónak fel kell engednie, mielőtt új session indulhat
- a Timer5-alapú `CoilGuard` a minijáték alatt is végig fut

Timeoutkor a Mega `MG_ABORT,<session>,<reason>` sort küld, visszaállítja a
normál fényt és kidobja a golyót. Multiball és Hurry Up alatt nem indul
minijáték; ezekben a helyzetekben megmarad a firmware gyors VUK-kidobó ága.

## Fények

A minijáték végső FastLED-rétege minden frame-ben a normál játéktér fölé kerül:

- idle: majdnem teljesen sötét pálya, néhány lélegző ambient és UFO-nyíl;
- GOOD: egy sima zöld teljes-pályás villanás;
- BAD: két piros teljes-pályás villanás;
- COLLECTION: kb. 1 s-os, szinuszos zöld hullám.

Az effektek kizárólag `millis()`-al futnak, nincs blokkoló `delay()`.

## Telepítési sorrend

Először a GUI-t kell frissíteni, utána a firmware-t. Az új GUI elfogadja a
régi `MUNCHIES`/`VUK_GAME` triggereket is; a régi GUI viszont még nem tud
`MG_READY` választ küldeni az új firmware-nek.

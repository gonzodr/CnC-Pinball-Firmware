# CnC Pinball - Arduino Mega pin terkep (V4)

Aktualis allapot a 2026-07-09-es Serial1-atallas UTAN.
(A 18/19 -> 42/49 atkotes es a WAV Trigger 46 -> 18 atkotese szukseges hozza,
lasd todo.txt!)

## Digitalis pinek

| Pin | Irany | Funkcio |
|-----|-------|---------|
| 0, 1 | - | Serial0 - USB, kommunikacio a Pi-vel (115200?? / firmware: 9800) |
| 2 | KI (PWM) | Szivar-izzas LED (cigar, a spinner tolti) |
| 3 | KI | WS2812B LED-szalag adat (115 LED) |
| 4 | BE (pullup) | Jobb slingshot kapcsolo |
| 5 | KI | GOLYOTAROLO TEKERCS (kilokes) |
| 6 | KI | KILOVOSAV TEKERCS (kicker) |
| 7 | BE (pullup) | Chong kapcsolo |
| 8 | BE (pullup) | Weed celpont 4 |
| 9 | BE (pullup) | Weed celpont 3 |
| 10 | BE (pullup) | Weed celpont 2 |
| 11 | BE (pullup) | Weed celpont 1 |
| 12 | BE (pullup) | Cheech kapcsolo |
| 13 | BE (pullup) | Kilovosav kapcsolo (golyo a savban) |
| 14 | BE (pullup) | CnC "C" betu (1.) |
| 15 | BE (pullup) | CnC "n" betu |
| 16 | BE (pullup) | Fish tank kapcsolo 1 |
| 17 | BE (pullup) | Fish tank kapcsolo 2 |
| 18 | KI | Serial1 TX -> WAV Trigger RX (2026-07-09-tol!) |
| 19 | BE | Serial1 RX <- WAV Trigger TX (ha be van kotve) |
| 20, 21 | - | I2C (SDA/SCL) -> szervos UNO (cim: 8, weed-mero) |
| 22 | BE (pullup) | DAVE "V" sav |
| 23 | BE (pullup) | DAVE "E" sav |
| 24 | KI | JOBB FLIPPER TEKERCS |
| 25 | BE (pullup) | CnC "C" betu (3.) |
| 26 | BE (pullup) | Jobb flippergomb |
| 27 | BE (pullup) | Bal flippergomb |
| 28 | KI | BAL SLINGSHOT TEKERCS |
| 29 | KI | JOBB SLINGSHOT TEKERCS |
| 30 | BE (pullup) | Loop kapcsolo (felso) |
| 31 | BE (pullup) | Bal slingshot kapcsolo |
| 32 | KI | POP BUMPER 3 TEKERCS |
| 33 | KI | POP BUMPER 1 TEKERCS |
| 34 | BE (pullup) | Pop bumper 1 kapcsolo |
| 35 | BE (pullup) | Pop bumper 2 kapcsolo |
| 36 | KI | POP BUMPER 2 TEKERCS |
| 37 | KI | UFO TEKERCS |
| 38 | BE (pullup) | Weed spinner ("Get High") |
| 39 | BE (pullup) | Pop bumper 3 kapcsolo |
| 40 | BE (pullup) | Gate kapcsolo 3 |
| 41 | KI | BAL FLIPPER TEKERCS |
| 42 | BE (pullup) | DAVE "A" sav (2026-07-09: atkotve a 18-rol!) |
| 43 | BE (pullup) | Loop kapcsolo (also/oldalso) |
| 44 | BE (pullup) | Gate kapcsolo 2 |
| 45 | BE (pullup) | Gate kapcsolo 1 |
| 46 | - | SZABAD (regen AltSoftSerial TX volt) |
| 47 | BE (pullup) | Start gomb |
| 48 | - | SZABAD (regen AltSoftSerial RX volt) |
| 49 | BE (pullup) | DAVE "D" sav (2026-07-09: atkotve a 19-rol!) |
| 50 | - | SZABAD |
| 51 | BE (pullup) | Golyokilovo gomb |
| 52 | BE (pullup) | Nagy hid (Bridge High) kapcsolo |
| 53 | BE (pullup) | Kis hid (Bridge Low) kapcsolo |

## Analog pinek

| Pin | Irany | Funkcio |
|-----|-------|---------|
| A0 | BE (analog) | Golyotarolo infra 1 (>988 = golyo ott, <974 = nincs) |
| A1 | BE (analog) | Golyotarolo infra 2 |
| A2 | BE (analog) | Golyotarolo infra 3 |
| A3 | BE (analog) | Golyotarolo infra 4 |
| A4 | BE (analog) | Golyotarolo infra 5 |
| A5 | BE (analog) | UFO infra (<100 = golyo az UFO-ban) |
| A12 | BE (pullup) | Tilt-inga kapcsolo |
| A13 | KI | Reset-vonal (bootkor HIGH, "Exit" uzenetnel LOW) |

## Megjegyzesek

- A tekercsek NAGYBETUVEL vannak jelolve - mindegyiket vedi az
  e_coil_guard.ino watchdog-ja (Timer5, 1 kHz).
- Pin 14-17: ezek a Serial2/Serial3 UART-labak, kapcsolo van rajtuk ->
  ha valaha meg egy soros eszkoz kell, ezeket kell atkotni.
- A 10-es pin a setupban ketszer konfiguralt (a10 INPUT, majd d10
  INPUT_PULLUP) - a pullup ervenyesul, az a10 sor felesleges.
- Az intmMode-ban halott kod olvassa az A1, A4-A7 csatornakat
  (miv1..miv5) - az ertekeket semmi nem hasznalja.
- Timer-foglaltsag: Timer0 = millis, Timer3 = PWM pin 2 (szivar),
  Timer5 = coil guard. A tobbi szabad.

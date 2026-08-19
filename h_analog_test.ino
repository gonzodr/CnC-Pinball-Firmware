////////////////////////////////////////////////////////////////////////////
//// ANALOG BEMENET-TESZT + EEPROM-BAN TAROLT KUSZOBERTEKEK
////////////////////////////////////////////////////////////////////////////
//
// Miert: a golyoerzekelo infra-szenzorok kuszobe korabban be volt egetve a
// kodba (< 100). Minden hangolashoz ujra kellett flashelni a firmware-t.
// Mostantol szenzoronkent kulon kuszob van, az EEPROM-ban tarolva: a Pi
// szerviz menuje irja, a firmware bootkor olvassa. Uj kuszob = nincs flash.
//
// A Pi szerviz menuje ezt a protokollt hasznalja (mind \n-nel zarva):
//   AT,START            -> AT_INFO,<db>,<nev1>,...,<nevN>  majd folyamatos
//                          AT_VAL,<e1>,...,<eN> (kb. 5 Hz), amig STOP nem jon
//   AT,STOP             -> AT_STOPPED
//   AT,GET              -> AT_THR,<k1>,...,<kN>
//   AT,SET,<idx>,<ert>  -> AT_OK,<idx>,<ert>   (EEPROM-ba is beirja)
//                          AT_ERR,RANGE        (rossz index/ertek)
//   Ha nem attract-ban vagyunk: AT_ERR,BUSY (lasd lentebb, miert).
//
// BIZTONSAG: a teszt-mod CSAK attract-bol (intmon == 1) indithato. Jatek
// kozben a tekercsek elnek, es egy szerviz-kepernyo nem szolhat bele -
// ezert inkabb visszautasitjuk. A stream magatol leall, ha a gep kilep az
// attractbol, vagy ha a Pi elnemul (AT_STREAM_TIMEOUT_MS).

#include <EEPROM.h>

// --- Szenzortabla -------------------------------------------------------
// UJ SZENZOR FELVETELE: egyetlen sor ide, semmi mas. A darabszam, az
// EEPROM-blokk merete es a Pi-nek kuldott nevlista mind ebbol szarmazik.
struct AnalogSensor {
  uint8_t pin;
  const char* name;
};

static const AnalogSensor ANALOG_SENSORS[] = {
  { PIN_A0, "Ballstack 1" },
  { PIN_A1, "Ballstack 2" },
  { PIN_A2, "Ballstack 3" },
  { PIN_A3, "Ballstack 4" },
  { PIN_A4, "Ballstack 5" },
  { PIN_A5, "VUK1" },
};

#define ANALOG_SENSOR_COUNT (sizeof(ANALOG_SENSORS) / sizeof(ANALOG_SENSORS[0]))

// Az elso ot szenzor a golyotarolo (ball1..ball5), a hatodik az UFO/VUK.
#define ANALOG_DEFAULT_THRESHOLD 100   // a korabbi, bedrotozott ertek

uint16_t analogThreshold[ANALOG_SENSOR_COUNT];

// --- EEPROM ------------------------------------------------------------
// Elrendezes a 0. cimtol:
//   0: 'C'  1: 'T'      magic
//   2: verzio (1)
//   3: szenzorszam
//   4..: uint16 kuszob (little endian) szenzoronkent
//   utolso: checksum (az elozo byte-ok osszege, 8 biten)
#define AT_EE_ADDR      0
#define AT_EE_MAGIC0    'C'
#define AT_EE_MAGIC1    'T'
#define AT_EE_VERSION   1
#define AT_EE_HEADER    4
#define AT_EE_SIZE      (AT_EE_HEADER + 2 * ANALOG_SENSOR_COUNT + 1)

static uint8_t AnalogEepromChecksum() {
  uint8_t sum = 0;
  for (uint16_t i = 0; i < AT_EE_SIZE - 1; i++) sum += EEPROM.read(AT_EE_ADDR + i);
  return sum;
}

void AnalogThresholdsSave() {
  EEPROM.update(AT_EE_ADDR + 0, AT_EE_MAGIC0);
  EEPROM.update(AT_EE_ADDR + 1, AT_EE_MAGIC1);
  EEPROM.update(AT_EE_ADDR + 2, AT_EE_VERSION);
  EEPROM.update(AT_EE_ADDR + 3, (uint8_t)ANALOG_SENSOR_COUNT);
  for (uint8_t i = 0; i < ANALOG_SENSOR_COUNT; i++) {
    EEPROM.update(AT_EE_ADDR + AT_EE_HEADER + 2 * i, (uint8_t)(analogThreshold[i] & 0xFF));
    EEPROM.update(AT_EE_ADDR + AT_EE_HEADER + 2 * i + 1, (uint8_t)(analogThreshold[i] >> 8));
  }
  EEPROM.update(AT_EE_ADDR + AT_EE_SIZE - 1, AnalogEepromChecksum());
}

// Bootkor hivjuk. Ha az EEPROM ures/serult/regi verzioju (elso inditas uj
// panelen, vagy szenzorszam-valtozas), a bedrotozott alapertek lep eletbe -
// vagyis a gep pontosan ugy viselkedik, mint a valtoztatas elott.
void AnalogThresholdsLoad() {
  for (uint8_t i = 0; i < ANALOG_SENSOR_COUNT; i++) analogThreshold[i] = ANALOG_DEFAULT_THRESHOLD;

  if (EEPROM.read(AT_EE_ADDR + 0) != AT_EE_MAGIC0) return;
  if (EEPROM.read(AT_EE_ADDR + 1) != AT_EE_MAGIC1) return;
  if (EEPROM.read(AT_EE_ADDR + 2) != AT_EE_VERSION) return;
  if (EEPROM.read(AT_EE_ADDR + 3) != (uint8_t)ANALOG_SENSOR_COUNT) return;
  if (EEPROM.read(AT_EE_ADDR + AT_EE_SIZE - 1) != AnalogEepromChecksum()) return;

  for (uint8_t i = 0; i < ANALOG_SENSOR_COUNT; i++) {
    uint16_t lo = EEPROM.read(AT_EE_ADDR + AT_EE_HEADER + 2 * i);
    uint16_t hi = EEPROM.read(AT_EE_ADDR + AT_EE_HEADER + 2 * i + 1);
    uint16_t v = lo | (hi << 8);
    if (v <= 1023) analogThreshold[i] = v;   // 10 bites ADC
  }
}

// --- Teszt-mod ---------------------------------------------------------
#define AT_STREAM_INTERVAL_MS  200UL     // ~5 Hz
#define AT_STREAM_TIMEOUT_MS   600000UL  // 10 perc utan magatol leall

bool analogTestActive = false;
static unsigned long analogTestLastSend = 0;
static unsigned long analogTestStarted = 0;

static void AnalogTestSendInfo() {
  Serial.print("AT_INFO,");
  Serial.print((int)ANALOG_SENSOR_COUNT);
  for (uint8_t i = 0; i < ANALOG_SENSOR_COUNT; i++) {
    Serial.print(',');
    Serial.print(ANALOG_SENSORS[i].name);
  }
  Serial.println();
}

static void AnalogTestSendThresholds() {
  Serial.print("AT_THR");
  for (uint8_t i = 0; i < ANALOG_SENSOR_COUNT; i++) {
    Serial.print(',');
    Serial.print(analogThreshold[i]);
  }
  Serial.println();
}

static void AnalogTestSendValues() {
  Serial.print("AT_VAL");
  for (uint8_t i = 0; i < ANALOG_SENSOR_COUNT; i++) {
    Serial.print(',');
    Serial.print(SimAnalogRead(ANALOG_SENSORS[i].pin));
  }
  Serial.println();
}

void StopAnalogTest() {
  if (!analogTestActive) return;
  analogTestActive = false;
  Serial.println("AT_STOPPED");
}

// A loop()-bol hivjuk minden korben.
void AnalogTestPoll() {
  if (!analogTestActive) return;

  // A gep kilepett az attractbol (jatek indult) vagy lejart a biztonsagi
  // ido: a stream nem maradhat vegtelenul bekapcsolva.
  if (intmon != 1 || millis() - analogTestStarted > AT_STREAM_TIMEOUT_MS) {
    StopAnalogTest();
    return;
  }

  if (millis() - analogTestLastSend >= AT_STREAM_INTERVAL_MS) {
    analogTestLastSend = millis();
    AnalogTestSendValues();
  }
}

// "AT,..." parancsok. A kozos, nem-blokkolo sorolvasobol hivja
// a HandleControlCmd (lasd d_light_effects.ino).
void HandleAnalogTestCmd(const char* s) {
  const char* arg = s + 3;   // "AT," utan

  if (!strncmp(arg, "START", 5)) {
    if (intmon != 1) { Serial.println("AT_ERR,BUSY"); return; }
    analogTestActive = true;
    analogTestStarted = millis();
    analogTestLastSend = 0;
    AnalogTestSendInfo();
    AnalogTestSendThresholds();
    return;
  }

  if (!strncmp(arg, "STOP", 4)) {
    analogTestActive = false;      // csendben: a Pi ugyis kilep
    Serial.println("AT_STOPPED");
    return;
  }

  if (!strncmp(arg, "GET", 3)) {
    AnalogTestSendThresholds();
    return;
  }

  if (!strncmp(arg, "SET,", 4)) {
    const char* p = arg + 4;
    int idx = atoi(p);
    const char* comma = strchr(p, ',');
    if (!comma) { Serial.println("AT_ERR,RANGE"); return; }
    int val = atoi(comma + 1);
    if (idx < 0 || idx >= (int)ANALOG_SENSOR_COUNT || val < 0 || val > 1023) {
      Serial.println("AT_ERR,RANGE");
      return;
    }
    analogThreshold[idx] = (uint16_t)val;
    AnalogThresholdsSave();
    Serial.print("AT_OK,"); Serial.print(idx); Serial.print(','); Serial.println(val);
    return;
  }

  Serial.println("AT_ERR,CMD");
}

# Refaktor-tesztpad (regressziós háló)

A firmware alrendszerenkénti refaktorjához: minden változtatás után a
tesztpadi Mega SIM-demója ugyanazt a soros eseménysort kell adja, mint a
`golden_demo.events.txt` referencia. Ha a diff üres, a refaktor
bizonyítottan megőrizte a viselkedést.

## Előfeltétel: determinizmus

A SIM-init fix `randomSeed()`-et állít (`0xC0FFEE`), így a demó minden
lefutása BITRE azonos. Enélkül a 6 `random()`-hívás miatt a diff zajos
lenne. (Éles buildben — SIM_MODE ki — nincs seed, marad a változatosság.)

## Egy iteráció (a Pi-n)

```bash
# 1. refaktor a forrasban (F: gepen), commit+push
# 2. Pi: pull + flash a padra
cd ~/CnC_firmware4 && git pull && bash test/flash_sim.sh
# 3. golden rogzitese egy teljes ciklusra (~300s)
~/CnC-Pinball-GUI-V3/venv/bin/python3 test/watch.py 300 /tmp/fw/uj
# 4. diff a golden ellen (a "Start"-tol)
awk '/^Start$/{p=1} p' /tmp/fw/uj.events.txt > /tmp/fw/uj.trim
diff test/golden_demo.events.txt /tmp/fw/uj.trim && echo "TISZTA - a refaktor OK"
```

## Mit fed le és mit NEM

- **Lefedi:** pontozás (Score choke-point trace), játékfolyam, videó-
  triggerek, hangválasztás ott, ahol soros-látható.
- **NEM fedi:** a LED-rutinokat (a pad nem capture-özi a szalagot) és a
  hardver-közeli időzítést (MIV/debounce - a SIM kiváltja a bemeneteket).
  Ezeket érveléssel + a gépen ellenőrizzük, nem a diffel.

## Fájlok

- `golden_demo.events.txt` - a referencia eseménysor (2 játékos, teljes
  parti multiballal + SpaceCoke-kal + névbeírással)
- `watch.py` - soros-figyelő, kanonikus eseménysort ír
- `flash_sim.sh` - padi SIM-flash (a main gépbiztos marad utána)

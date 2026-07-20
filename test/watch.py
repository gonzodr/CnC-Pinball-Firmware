#!/usr/bin/env python3
"""Tesztpadi soros-figyelo a firmware-refaktorhoz.

A cnc-pinball service-t ELOTTE le kell allitani (az fogja a portot)!
Rogzit egy megadott ideig, es ket fajlt ir:
  - <alap>.raw.txt        : minden sor idobelyeggel (debughoz)
  - <alap>.events.txt     : KANONIKUS eseménysor (idobelyeg nelkul,
                            egymas utani duplikatumok osszevonva) -> EZT
                            diffeljuk a golden ellen

Hasznalat:
  watch.py <masodperc> <kimeneti-alapnev>
  pl.  watch.py 290 /tmp/fw/current
"""
import sys, time, serial

PORT = "/dev/ttyACM0"
BAUD = 115200

# Ezek a stimulus/allapot-zaj sorok NEM kerulnek a kanonikus esemenysorba
# (a SIM,* a bemenet-vezerles, a score,* pedig 350ms-enkent ismetlodo spam;
#  a T,score trace es minden video-trigger BENT marad).
def is_noise(line: str) -> bool:
    if line.startswith("SIM,"):
        return True
    if line.startswith("score,"):
        return True
    return False


def main():
    dur = float(sys.argv[1]) if len(sys.argv) > 1 else 290
    base = sys.argv[2] if len(sys.argv) > 2 else "/tmp/fw/current"

    ser = serial.Serial(PORT, BAUD, timeout=1)
    raw_lines = []
    events = []
    last_event = None
    t0 = time.time()
    while time.time() - t0 < dur:
        raw = ser.readline()
        if not raw:
            continue
        line = raw.decode("utf-8", errors="replace").strip()
        if not line:
            continue
        raw_lines.append("%8.2f  %s" % (time.time() - t0, line))
        if is_noise(line):
            continue
        # egymas utani azonos esemenyek osszevonasa (dedup)
        if line != last_event:
            events.append(line)
            last_event = line

    with open(base + ".raw.txt", "w") as f:
        f.write("\n".join(raw_lines) + "\n")
    with open(base + ".events.txt", "w") as f:
        f.write("\n".join(events) + "\n")
    print("RAW: %d sor, EVENTS: %d sor -> %s.events.txt" %
          (len(raw_lines), len(events), base))


if __name__ == "__main__":
    main()

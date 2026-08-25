#!/bin/bash
# Tesztpadi SIM-firmware flash-elese a Pi-rol.
# A szimulaciot ket explicit compiler flaggel kapcsolja be; a forrasfajlt
# nem modositja, igy a main munkapeldany vegig gepbiztos marad.
# A cnc-pinball service-t elotte leallitja, hogy szabad legyen a port.
set -e
FWDIR="$HOME/CnC_firmware4"
CLI="$HOME/bin/arduino-cli"
FQBN="arduino:avr:mega"

echo "== service stop + port felszabaditas"
echo 123 | sudo -S systemctl stop cnc-pinball 2>/dev/null || true
sudo fuser -k /dev/ttyACM0 2>/dev/null || true
sleep 2

echo "== forditas"
"$CLI" compile --fqbn "$FQBN" \
  --build-property "compiler.cpp.extra_flags=-DSIM_MODE -DSIM_BUILD_CONFIRMED" \
  "$FWDIR" 2>&1 | grep -E 'error|Sketch uses' || true

echo "== feltoltes"
"$CLI" upload -p /dev/ttyACM0 --fqbn "$FQBN" "$FWDIR" 2>&1 | tail -1

echo "== KESZ (a Mega ujraindult, a demo fut)"

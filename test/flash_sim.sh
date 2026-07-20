#!/bin/bash
# Tesztpadi SIM-firmware flash-elese a Pi-rol.
# SIM_MODE-ot IDEIGLENESEN bekapcsolja, forditja+feltolti, majd a
# munkapeldanyt visszatisztitja (a main AG GEPBIZTOS marad!).
# A cnc-pinball service-t elotte leallitja, hogy szabad legyen a port.
set -e
FWDIR="$HOME/CnC_firmware4"
CLI="$HOME/bin/arduino-cli"
FQBN="arduino:avr:mega"

echo "== service stop + port felszabaditas"
echo 123 | sudo -S systemctl stop cnc-pinball 2>/dev/null || true
sudo fuser -k /dev/ttyACM0 2>/dev/null || true
sleep 2

cd "$FWDIR"
echo "== SIM_MODE be"
sed -i 's|^//#define SIM_MODE|#define SIM_MODE|' CnC_firmware4.ino

echo "== forditas"
"$CLI" compile --fqbn "$FQBN" "$FWDIR" 2>&1 | grep -E 'error|Sketch uses' || true

echo "== feltoltes"
"$CLI" upload -p /dev/ttyACM0 --fqbn "$FQBN" "$FWDIR" 2>&1 | tail -1

echo "== munkapeldany visszatisztitasa (main gepbiztos)"
git checkout -- CnC_firmware4.ino
echo "== KESZ (a Mega ujraindult, a demo fut)"

"""Scan for nearby access-points."""

import time
from network import WLAN

wlan = WLAN(WLAN.IF_STA)

# Set firmware paths BEFORE activating, required for filesystem loading
wlan.config(wifi_fw='/flash/wifi_fw.bin', nvram='/flash/nvram.bin')

wlan.active(False)
wlan.active(True)
wlan.config(antenna=0)

print("Start scanning ...")
ap_list = set()
for _ in range(5):
    time.sleep(1)
    for ap in wlan.scan():
        ap_list.add(ap[0].decode())

print(len(ap_list) > 0)

# Test network.WLAN.config() firmware path get/set for CYW43

try:
    import network
except ImportError:
    print("SKIP")
    raise SystemExit

# Check that there is a WLAN object
try:
    wlan = network.WLAN()
except (ValueError, AttributeError):
    print("SKIP")
    raise SystemExit

# Test setting firmware paths to None
print("=== Set paths to None ===")
try:
    wlan.config(wifi_fw=None)
    result = wlan.config("wifi_fw")
    print("wifi_fw:", repr(result))
except (ValueError, AttributeError):
    print("wifi_fw: NOT_SUPPORTED")
try:
    wlan.config(nvram=None)
    result = wlan.config("nvram")
    print("nvram:", repr(result))
except (ValueError, AttributeError):
    print("nvram: NOT_SUPPORTED")
try:
    wlan.config(bt_fw=None)
    result = wlan.config("bt_fw")
    print("bt_fw:", repr(result))
except (ValueError, AttributeError):
    print("bt_fw: NOT_SUPPORTED")

# Test setting firmware paths to strings
print("=== Set paths to strings ===")
try:
    wlan.config(wifi_fw="/path/to/fw.bin")
    result = wlan.config("wifi_fw")
    print("set wifi_fw:", repr(result))
except (ValueError, AttributeError):
    print("set wifi_fw: NOT_SUPPORTED")
try:
    wlan.config(nvram="/path/to/nvram.bin")
    result = wlan.config("nvram")
    print("set nvram:", repr(result))
except (ValueError, AttributeError):
    print("set nvram: NOT_SUPPORTED")
try:
    wlan.config(bt_fw="/path/to/bt.bin")
    result = wlan.config("bt_fw")
    print("set bt_fw:", repr(result))
except (ValueError, AttributeError):
    print("set bt_fw: NOT_SUPPORTED")

# Test updating a firmware path
print("=== Update a path ===")
try:
    wlan.config(wifi_fw="/new/path/fw.bin")
    result = wlan.config("wifi_fw")
    print("re-set wifi_fw:", repr(result))
except (ValueError, AttributeError):
    print("re-set wifi_fw: NOT_SUPPORTED")

# Test setting all three at once
print("=== Multi-set test ===")
try:
    wlan.config(wifi_fw="/fw.bin", nvram="/nvram.bin", bt_fw="/bt.bin")
    result_wifi = wlan.config("wifi_fw")
    result_nvram = wlan.config("nvram")
    result_bt = wlan.config("bt_fw")
    print("wifi_fw:", repr(result_wifi))
    print("nvram:", repr(result_nvram))
    print("bt_fw:", repr(result_bt))
except (ValueError, AttributeError):
    print("multi-set: NOT_SUPPORTED")

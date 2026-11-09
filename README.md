# Mecanum Rover Code

## Upload and Run

Install Arduino IDE 2 and add the following address under **Preferences → Additional Boards Manager URLs**, then install **esp32 by Espressif Systems, version 3.3.11** through Boards Manager. Select **ESP32S3 Dev Module** (the rover's microcontroller board); no additional libraries are required.

```text
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Open one sketch (Arduino program) from `sketches/`:

* `RC_Car_direct_alternating_test`: pulsed straight-line movement.
* `RC_Car_obstacle_stop`: forward movement with a 30 cm stopping threshold.
* `RC_Car_intelligent_avoidance`: reactive obstacle avoidance.

Connect the ESP32-S3 by USB, select its port and click **Upload**, keeping motor power off during upload. Check the pin assignments in the selected sketch, lift the wheels for the first run, then enable the rover's battery supply; the program starts automatically, whilst switching off motor power stops movement. The autonomous sketch treats a missing forward echo as open space, so its runs require supervision.

The `diagnostics/` folder contains the earlier wiring and power checks. Each diagnostic retains its own pin assignments (wiring); sketches that print measurements use **115200 baud** in Serial Monitor.

## Command-Line Upload

With Arduino CLI 1.5.1 installed, run these commands from the repository root, replacing `PORT` with the port listed by `board list`:

```sh
arduino-cli core update-index --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
arduino-cli core install esp32:esp32@3.3.11 --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
arduino-cli board list
arduino-cli compile --fqbn esp32:esp32:esp32s3 sketches/RC_Car_intelligent_avoidance
arduino-cli upload --fqbn esp32:esp32:esp32s3 --port PORT sketches/RC_Car_intelligent_avoidance
```

Run `bash scripts/check.sh` to compile all sketches without uploading.

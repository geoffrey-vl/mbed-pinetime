# mbed-pinetime

![Digital clock](doc/digitalclock.jpg) ![Analog clock demo](doc/analogclock.jpg)

## Getting Started

Install dependencies:

- python3
- c/c++ compiler, prefereable the GNU GCC compiler version 9

Optionally, install `nrfjprog`

- MAC:

    ```bash
    brew cask install nordic-nrf-command-line-tools
    ```

- Linux

    TODO

## Getting the source code

```bash
mbed import git@github.com:geoffrey-vl/mbed-pinetime.git
```

You can off course also clone the repo manually using git, and next issue `mbed deploy`
to pull the submodules.

The mbed toolchain requires Python and some additional python packes which don't come
with the default Python installation. To install the additional packages:

Install all the Python packages required for Mbed. Your options are:

- Install from the Pipfile. `pipenv install`
- Install from Mbed requirements.txt `pip install -r mbed-os/requirements.txt`

## Compiling

```bash
mbed compile -t GCC_ARM
```

Note that the .mbed file has the mbed target fixed.
A more complete command:

```bash
mbed compile -m PINETIME_DEV_KIT -t GCC_ARM
```

If you want to target the NRF52-DK board which is very similar to the PineTime use:

```bash
mbed compile -m NRF52_DK -t GCC_ARM
```

## Development Notes

- [ ] BMA421 Accelerometer
- [ ] HRS3300 Heart Rate Sensor
- [ ] Hynitron CST816S Touch Pad
- [x] Sitronix ST7789V LCD Driver
- [ ] Macronix SPI Flash
- [ ] Vibrator
- [x] LED backlight
- [x] ADC Battery Voltage Sense
- [x] Physical button
- [ ] BLE

## Flashing

### Remove Flash protection

For non-jlink program/debug probes you must first remove the Flash protection.
Here is a good guide: [Advanced Topic: Remove nRF52 Flash Protection With Raspberry Pi](https://medium.com/@ly.lee/coding-nrf52-with-rust-and-apache-mynewt-on-visual-studio-code-9521bcba6004).

### Flashing using OpenOcd and ST-LINK

Here is a good guide on how to flash your Pinetime using an ST-Link probe:
[Flashing your PineTime using an ST-Link and OpenOCD]([https://link](https://dev.to/aaronc81/flashing-your-pinetime-using-an-st-link-and-openocd-54dd))

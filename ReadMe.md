# Flipper Zero Firmware

[![Discord](https://img.shields.io/discord/740930220399525928.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](http://flipperzero.one/discord)

![Show me the code](https://habrastorage.org/webt/eo/m0/e4/eom0e4btudte7nrhnyic-laiog0.png)

---

📝 This is my own custom repository of the *Flipper Zero* firmware. I plan to create small games for the device, and pick up some ideas for its features (like scripts for Bad USB 👿).

[=> See the original repository](https://github.com/flipperdevices/flipperzero-firmware)

---

## Content

### Applications

- [Dice Roller](./applications/dice_roller): Generates a number between 1 and 6.

### Project structure

- `applications`    - Applications and services used in firmware
- `assets`          - Assets used by applications and services
- `furi`            - Furi Core: os level primitives and helpers
- `debug`           - Debug tool: GDB-plugins, SVD-file and etc
- `docker`          - Docker image sources (used for firmware build automation)
- `documentation`   - Documentation generation system configs and input files
- `firmware`        - Firmware source code
- `lib`             - Our and 3rd party libraries, drivers and etc...
- `scripts`         - Supplementary scripts and python libraries home

Also pay attention to `ReadMe.md` files inside of those directories.

## Work on this repository

### Clone the repository

Since the repository contains submodules, be sure to use the `--recursive` option when cloning it.

```shell
git clone --recursive https://github.com/hubertgdev/flipperzero-firmware.git
```

This repository is not a "direct fork", so the [original firmware repository](https://github.com/flipperdevices/flipperzero-firmware) must be added as the `upstream` remote to keep things synced with the latest version.

```shell
git remote add upstream https://github.com/flipperdevices/flipperzero-firmware.git
```

*Note: I usually work with the `release` branch of the original repository, making me sure that the firmware is stable. Feel free to use the `dev` branch if you want to use beta features.*

### Setup dev environment

I use [Visual Studio Community](https://visualstudio.microsoft.com/fr/vs/community/) as my favorite IDE for this project, but you can add basically any IDE.

In order to build the sources, you will need to [install Docker](https://www.docker.com/get-started/).

Once you have installed docker, setup the container by running the following command:

```shell
cd path/to/local/repository
docker-compose up -d
```

Also, in order to upload the sources to your *Flipper Zero* device, you'll need [qFlipper](https://flipperzero.one/update).

### Compile and upload

Compile the sources with docker by running the following command:

```shell
cd path/to/local/repository
docker-compose exec dev ./fbt
```

Once the sources are compiled, you should now see a `.dfu` file in the `dist/` folder. Open [qFlipper](https://flipperzero.one/update), and with your device connected, select *Install from file*. Locate the generated `.dfu` file, and just wait for your own firmware to be installed!

## Links

* Discord: [flipp.dev/discord](https://flipp.dev/discord)
* Website: [flipperzero.one](https://flipperzero.one)
* Kickstarter page: [kickstarter.com](https://www.kickstarter.com/projects/flipper-devices/flipper-zero-tamagochi-for-hackers)
* Forum: [forum.flipperzero.one](https://forum.flipperzero.one/)
* [Original firmware repository](https://github.com/flipperdevices/flipperzero-firmware)

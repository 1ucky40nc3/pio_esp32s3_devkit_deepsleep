# pio_esp32s3_devkit_deepsleep

Deep Sleep and Neo Pixel LED example with at [ESP32-S3-DevKitC-1] implemented using PlatformIO (PIO)

### Install the PlatformIO IDE VSCode Extension

Install the PlatformIO IDE extension from the VSCode extension marketplace (see workspace recommendations). You may be asked to install Python. You may follow the instructions [here](https://docs.platformio.org/en/latest/faq/install-python.html).

### Initialize the Project

In this example a PlatformIO project has already been created. The project configuration can be inspected in the [platformio.ini](./platformio.ini) file.

The `platformio` (or `pio`) command-line interface should be installed along the the extension. For the local development purposes you need to intialize the command-line interface. Execute the following commands in the command-line:

```bash
pio project init
```

Note: The command-line has to be a PlatformIO terminal. You can create a new PlatformIO termincal by pressing the `PlatformIO: New Terminal` buttom in the bottom left of your VSCode window.

Run the `pio check` command to do a static code analysis to check if everything is setup correctly.

### Set up Configuration and Secrets

You can find sections below on how to handle [configuration](#configuration) and [secrets](#secrets) down below. Make to follow the instructions and you should be good to go.

### Build and Upload the Code to the [ESP32-S3-DevKitC-1]

After installation and initialization you may build and upload the code to your device. Connect the Ardunino Nano ESP32 via USB. Then run the following commands (in a PlatformIO terminal):

```bash
# Check if your device can be detected
pio device list
# Build the code
pio run -e esp32-s3-devkitm-1 -t build
# Upload the build
pio run -e esp32-s3-devkitm-1 -t upload
```

As an alternative to the command you can also use the PlatformIO buttons in VSCode. You can find the buttons below. Make sure to select the correct (`esp32-s3-devkitm-1`) environment for the upload.

### Run Tests

```bash
# Run test locally on your desktop
pio test -e desktop
# Run test on the device
pio test -e esp32-s3-devkitm-1
```

#### Debug Tests

If you are using VSCode (which is recommended), use the following steps to debug your test runs:

1. Select the `desktop` environment to select your local machine as the build target
2. Open the testing module [test_desktop_main.cpp](test/test_desktop/test_desktop_main.cpp) in your editor
3. Go to the debug tab in VSCode
4. Launch the code with the `PIO Debug` configuration

This will automatically attach a gdb debugger and enable VSCode debugging.

## Documentation

### Configuration

Set the configuration in the [config.h](lib/Config/config.h) header file. Use the provided constants as they are used in the code.

## Sources

- [PlatformIO]
- [ESP32-S3-DevKitC-1]

[PlatformIO]: https://platformio.org/
[ESP32-S3-DevKitC-1]: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.1.html#hardware-reference

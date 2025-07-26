/**
 * @brief The baud rate for serial communication.
 *
 * This constant sets the data transfer rate for the serial port,
 */
const unsigned long SERIAL_BAUD = 115200;

/**
 * @brief The delay in milliseconds after serial communication is initialized.
 *
 * This delay provides a brief pause after setting up the serial port,
 * allowing the serial connection to stabilize before any data transmission begins.
 * It can be useful to ensure that the connected device is ready to receive commands or data.
 */
const unsigned long DELAY_AFTER_SERIAL_INITIALIZED = 100;

/**
 * @brief The pin where the Neo Pixel LED is connected to.
 */
const unsigned int LED_PIN = 19;
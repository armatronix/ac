/*!
 * @file Adafruit_seesaw.h
 *
 * This is part of Adafruit's seesaw driver for the Arduino platform.  It is
 * designed specifically to work with the Adafruit products that use seesaw technology.
 *
 * These chips use I2C to communicate, 2 pins (SCL+SDA) are required
 * to interface with the board.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Dean Miller for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef LIB_SEESAW_H
#define LIB_SEESAW_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define SEESAW_ADDRESS                (0x49) ///< Default Seesaw I2C address
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/

    /** Module Base Addreses
     *  The module base addresses for different seesaw modules.
     */
    enum
    {
        SEESAW_STATUS_BASE = 0x00,
        SEESAW_GPIO_BASE = 0x01,
        SEESAW_SERCOM0_BASE = 0x02,

        SEESAW_TIMER_BASE = 0x08,
        SEESAW_ADC_BASE = 0x09,
        SEESAW_DAC_BASE = 0x0A,
        SEESAW_INTERRUPT_BASE = 0x0B,
        SEESAW_DAP_BASE = 0x0C,
        SEESAW_EEPROM_BASE = 0x0D,
        SEESAW_NEOPIXEL_BASE = 0x0E,
        SEESAW_TOUCH_BASE = 0x0F,
    };

    /** GPIO module function addres registers
     */
    enum
    {
        SEESAW_GPIO_DIRSET_BULK = 0x02,
        SEESAW_GPIO_DIRCLR_BULK = 0x03,
        SEESAW_GPIO_BULK = 0x04,
        SEESAW_GPIO_BULK_SET = 0x05,
        SEESAW_GPIO_BULK_CLR = 0x06,
        SEESAW_GPIO_BULK_TOGGLE = 0x07,
        SEESAW_GPIO_INTENSET = 0x08,
        SEESAW_GPIO_INTENCLR = 0x09,
        SEESAW_GPIO_INTFLAG = 0x0A,
        SEESAW_GPIO_PULLENSET = 0x0B,
        SEESAW_GPIO_PULLENCLR = 0x0C,
    };

    /** status module function addres registers
     */
    enum
    {
        SEESAW_STATUS_HW_ID = 0x01,
        SEESAW_STATUS_VERSION = 0x02,
        SEESAW_STATUS_OPTIONS = 0x03,
        SEESAW_STATUS_SWRST = 0x7F,
    };

    /** timer module function addres registers
     */
    enum
    {
        SEESAW_TIMER_STATUS = 0x00,
        SEESAW_TIMER_PWM = 0x01,
        SEESAW_TIMER_FREQ = 0x02,
    };
	
    /** ADC module function addres registers
     */
    enum
    {
        SEESAW_ADC_STATUS = 0x00,
        SEESAW_ADC_INTEN = 0x02,
        SEESAW_ADC_INTENCLR = 0x03,
        SEESAW_ADC_WINMODE = 0x04,
        SEESAW_ADC_WINTHRESH = 0x05,
        SEESAW_ADC_CHANNEL_OFFSET = 0x07,
    };

    /** Sercom module function addres registers
     */
    enum
    {
        SEESAW_SERCOM_STATUS = 0x00,
        SEESAW_SERCOM_INTEN = 0x02,
        SEESAW_SERCOM_INTENCLR = 0x03,
        SEESAW_SERCOM_BAUD = 0x04,
        SEESAW_SERCOM_DATA = 0x05,
    };

    /** neopixel module function addres registers
     */
    enum
    {
        SEESAW_NEOPIXEL_STATUS = 0x00,
        SEESAW_NEOPIXEL_PIN = 0x01,
        SEESAW_NEOPIXEL_SPEED = 0x02,
        SEESAW_NEOPIXEL_BUF_LENGTH = 0x03,
        SEESAW_NEOPIXEL_BUF = 0x04,
        SEESAW_NEOPIXEL_SHOW = 0x05,
    };

    /** touch module function addres registers
     */
    enum
    {
        SEESAW_TOUCH_CHANNEL_OFFSET = 0x10,
    };

#define ADC_INPUT_0_PIN 2 ///< default ADC input pin 
#define ADC_INPUT_1_PIN 3 ///< default ADC input pin
#define ADC_INPUT_2_PIN 4 ///< default ADC input pin
#define ADC_INPUT_3_PIN 5 ///< default ADC input pin

#define PWM_0_PIN 4 ///< default PWM output pin
#define PWM_1_PIN 5 ///< default PWM output pin
#define PWM_2_PIN 6 ///< default PWM output pin
#define PWM_3_PIN 7 ///< default PWM output pin

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN 0x03 ///< for compatibility with platforms that do not already define INPUT_PULLDOWN
#endif

/*=========================================================================*/

#define SEESAW_HW_ID_CODE			0x55 ///< seesaw HW ID code
#define SEESAW_EEPROM_I2C_ADDR 0x3F ///< EEPROM address of i2c address to start up with (for devices that support this feature)

/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with seesaw helper IC
*/
/**************************************************************************/
class Adafruit_seesaw : public Print {
	public:
	// constructors
        Adafruit_seesaw(TwoWire *Wi=NULL);
        ~Adafruit_seesaw(void) {};
		
	bool begin(uint8_t addr = SEESAW_ADDRESS, int8_t flow=-1);
        uint32_t getOptions();
        uint32_t getVersion();
	void SWReset();

        void pinMode(uint8_t pin, uint8_t mode);
        void pinModeBulk(uint32_t pins, uint8_t mode);
        void pinModeBulk(uint32_t pinsa, uint32_t pinsb, uint8_t mode);
        virtual void analogWrite(uint8_t pin, uint16_t value, uint8_t width = 8);
        void digitalWrite(uint8_t pin, uint8_t value);
        void digitalWriteBulk(uint32_t pins, uint8_t value);
        void digitalWriteBulk(uint32_t pinsa, uint32_t pinsb, uint8_t value);

        bool digitalRead(uint8_t pin);
        uint32_t digitalReadBulk(uint32_t pins);
        uint32_t digitalReadBulkB(uint32_t pins);

        void setGPIOInterrupts(uint32_t pins, bool enabled);

        virtual uint16_t analogRead(uint8_t pin);

        uint16_t touchRead(uint8_t pin);

        virtual void setPWMFreq(uint8_t pin, uint16_t freq);

        void enableSercomDataRdyInterrupt(uint8_t sercom = 0);
        void disableSercomDataRdyInterrupt(uint8_t sercom = 0);

        char readSercomData(uint8_t sercom = 0);

        void EEPROMWrite8(uint8_t addr, uint8_t val);
        void EEPROMWrite(uint8_t addr, uint8_t *buf, uint8_t size);
        uint8_t EEPROMRead8(uint8_t addr);

        void setI2CAddr(uint8_t addr);
        uint8_t getI2CAddr();

        void UARTSetBaud(uint32_t baud);

        virtual size_t write(uint8_t);
        virtual size_t write(const char *str);

	protected:
		uint8_t _i2caddr; /*!< The I2C address used to communicate with the seesaw */
		TwoWire *_i2cbus; /*!< The I2C Bus used to communicate with the seesaw */
		int8_t _flow; /*!< The flow control pin to use */

		void      write8(byte regHigh, byte regLow, byte value);
		uint8_t   read8(byte regHigh, byte regLow);
		
		void read(uint8_t regHigh, uint8_t regLow, uint8_t *buf, uint8_t num, uint16_t delay = 125);
		void write(uint8_t regHigh, uint8_t regLow, uint8_t *buf, uint8_t num);
		void writeEmpty(uint8_t regHigh, uint8_t regLow);
		void _i2c_init();

/*=========================================================================
	REGISTER BITFIELDS
    -----------------------------------------------------------------------*/

        /** Sercom interrupt enable register
        */
        union sercom_inten {
            struct {
                uint8_t DATA_RDY : 1; ///< this bit is set when data becomes available
            } bit; ///< bitfields
            uint8_t reg; ///< full register
        };
        sercom_inten _sercom_inten; ///< sercom interrupt enable register instance

/*=========================================================================*/
};

#endif

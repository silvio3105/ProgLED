/**
 * @file ProgLED.h
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Header file for ProgLED library.
 * @version v0.1r1
 * @date 2022-10-24
 * 
 * @copyright Copyright (c) 2022, silvio3105
 * 
 */


/*
License

Copyright (c) 2022, silvio3105 (www.github.com/silvio3105)

Access and use of this Project and its contents are granted free of charge to any Person.
The Person is allowed to copy, modify and use The Project and its contents only for non-commercial use.
Commercial use of this Project and its contents is prohibited.
Modifying this License and/or sublicensing is prohibited.

THE PROJECT AND ITS CONTENT ARE PROVIDED "AS IS" WITH ALL FAULTS AND WITHOUT EXPRESSED OR IMPLIED WARRANTY.
THE AUTHOR KEEPS ALL RIGHTS TO CHANGE OR REMOVE THE CONTENTS OF THIS PROJECT WITHOUT PREVIOUS NOTICE.
THE AUTHOR IS NOT RESPONSIBLE FOR DAMAGE OF ANY KIND OR LIABILITY CAUSED BY USING THE CONTENTS OF THIS PROJECT.

This License shall be included in all functional textual files.
*/

#ifndef _PROGLED_H_
#define _PROGLED_H_

// ----- INCLUDE FILES
#include            <stdint.h>

/** \addtogroup ProgLED
 * 
 * Frameworkelss driver for programmable RGB LEDs. 
 * @{
*/

// ----- DEFINES
/**
 * @brief Return code for not OK status.
 * 
 */
#define PROG_LED_NOK			0

/**
 * @brief Return code for OK status.
 * 
 */
#define PROG_LED_OK				1

/**
 * @brief Return code for continue status during clocking.
 * 
 */
#define PROG_LED_CONTINUE		2

/**
 * @brief Use FPU for color format conversions.
 * 
 * \c 0 FPU will not be used for color conversions.
 * \c 1 FPU will be used for color conversions.
 */
#define USE_FPU					0

/**
 * @brief Status bit in LED's config variable.
 * 
 */
#define PROG_LED_STATUS_BIT		7

/**
 * @brief Status mask in LED's config variable.
 * 
 */
#define PROG_LED_STATUS_MASK	0b10000000

/**
 * @brief Number of first bit for LED brightness in config variable. 
 * 
 */
#define PROG_LED_BRGHT_BIT		0

/**
 * @brief Brightness mask in LED's config variable.
 * 
 */
#define PROG_LED_BRGHT_MASK		0b01111111


// ----- ENUMS
/**
 * @brief Enum for 24-bit color format.
 * 
 */
typedef enum: uint8_t {
	PROG_LED_RGB, /**< RGB 24-bit format. */
	PROG_LED_GRB /**< GRB 24-bit format. */
} ProgLED_format_t;

/**
 * @brief Enum for LED line status.
 * 
 */
typedef enum: uint8_t {
	LINE_IDLE,
	LINE_CLOCKING
} ProgLED_status_t;

/**
 * @brief Enum for LED line type.
 * 
 * @note Not used for now.
 */
typedef enum: uint8_t {
	LED_16MC, /**< 24-bit color format, 8-bit for each color. */
	LED_8C, /**< 1-bit color format, 8 colors in total. */
} ProgLED_type_t;


// ----- TYPEDEFS
/**
 * @brief External hanlder typedef for handling data clocking.
 * 
 * @param bit Value of first bit.
 * @return No return value.
 */
typedef void (*extHandler)(uint8_t bit);

// ----- CLASSES
/**
 * @brief Class representing single LED line.
 * 
 */
class ProgLED : public Led {
	// Public stuff
	public:
	// Constructor and deconstructor
	/**
	 * @brief Object constructor for ProgLED.
	 * 
	 * @param start Pointer to external handler used for starting LEDs update.
	 * @param stop Pointer to external handler used for stoping LEDs update.
	 * @param ledsNum Number of LEDs at line.
	 * @param ledFormat LED color format. See \ref ProgLED_format_t
	 * @return No return value.
	 */
	ProgLED(extHandler start, extHandler stop, uint16_t ledsNum, ProgLED_format_t ledFormat = PROG_LED_GRB);

	/**
	 * @brief Object deconstructor.
	 * 
	 * @return No return value.
	 */
	~ProgLED(void);


	// Variables
	/**
	 * @brief Pointer to LEDs data in memory(heap).
	 * 
	 */
	Led* led = nullptr;	


	// Functions
	/**
	 * @brief ProgLED init function.
	 * 
	 * Checks external handlers and memory pointers.
	 * Sets LEDs to default values.
	 * 
	 * @return \ref PROG_LED_OK if everything is OK.
	 * @return \ref PROG_LED_NOK if memory for LEDs is not allocated.
	 */
	uint8_t init(void);

	/**
	 * @brief Reset LEDs to default color and config.
	 * 
	 * @return No return value.
	 */
	void reset(void);

	/**
	 * @brief Toggle LEDs status between on and off for whole LED line.
	 * 
	 * @return No return value.
	 */
	void toggle(void);

	/**
	 * @brief Turn off LED line.
	 * 
	 * @return No return value.
	 */
	void off(void);

	/**
	 * @brief Turn on LED line.
	 * 
	 * @return No return value.
	 */
	void on(void);

	/**
	 * @brief Calls external handler to start clocking out colors from @ref led memory in \ref format
	 * 
	 * @return No return value. 
	 */
	void update(void);

	/**
	 * @brief Calls external hanlder to stop clocking out colors.
	 * 
	 * @return No return value.
	 */
	void stop(void);

	/**
	 * @brief Fetches bit from LED color.
	 * 
	 * @param bit Reference where next bit will be stored. 
	 * @return PROG_LED_CONTINUE if more bits exists.
	 * @return PROG_LED_OK if last bit is fetched.
	 */
	uint8_t fetchBit(uint8_t& bit);

	/**
	 * @brief Sets brightness for LED line.
	 * 
	 * @param value Brightness value between 0 and 100%.
	 * @return No return value.
	 */
	void brightness(uint8_t value);


	// Private stuff
	private:
	// Variables
	/**
	 * @brief Number of LEDs to control.
	 * 
	 */
	uint16_t ledCount = 0;

	/**
	 * @brief LED index during clocking out.
	 * 
	 */
	uint8_t ledIdx = 0;

	/**
	 * @brief LED data byte at @ref ledIdx during clocking out.
	 * 
	 */
	uint8_t ledByte = 0;

	/**
	 * @brief Bit index at \ref ledByte during clocking out.
	 * 
	 */
	uint8_t ledBit = 0;

	/**
	 * @brief Pointer to external handler called to start clocking out.
	 * 
	 */
	extHandler startHandler = nullptr;

	/**
	 * @brief Pointer to external handler called to stop clocking out.
	 * 
	 */
	extHandler stopHandler = nullptr;

	/**
	 * @brief LED line status.
	 * 
	 */
	ProgLED_status_t lineStatus = LINE_IDLE;


	// Functions
	#if USE_FPU == 0
	/**
	 * @brief Converts RGB color format to HSV color format using fixed point math.
	 * 
	 * @param in Reference to input array in RGB format.
	 * @param out Reference to HSV output array.
	 * @return No return value. 
	 * 
	 * @warning Function does not work without FPU.
	 */
	void rgb2HSV(uint8_t (&in)[3], int32_t (&out)[3]);

	/**
	 * @brief Converts HSV color format to RGB color format using fixed point math.
	 * 
	 * @param in Reference to input array in HSV format.
	 * @param out Reference to RGB output array.
	 * @return No return value.
	 * 
	 * @warning Function does not work without FPU.
	 */
	void hsv2RGB(int32_t (&in)[3], uint8_t (&out)[3]);	
	#else
	/**
	 * @brief Converts RGB color format to HSV color format using fixed point math.
	 * 
	 * @param in Reference to input array in RGB format.
	 * @param out Reference to HSV output array.
	 * @return No return value. 
	 * 
	 * @note Function requires FPU for best performance during calculations.
	 */
	void rgb2HSV(uint8_t (&in)[3], float (&out)[3]);	

	/**
	 * @brief Converts HSV color format to RGB color using float point math.
	 * 
	 * @param in Reference to input array in HSV format.
	 * @param out Reference to RGB output array.
	 * @return No return value.
	 * 
	 * @note Function requires FPU for best performance during calculations.
	 * @warning Function does not convert right HSV(0 0 100)/RGB(255 255 255) back to RGB values.
	 */
	void hsv2RGB(float (&in)[3], uint8_t (&out)[3]);
	#endif // USE_FPU


	protected:
	/**
	 * @brief Index for RGB colors in \ref Led::color struct.
	 * 
	 */
	uint8_t rIdx = 0, gIdx = 1, bIdx = 2;		
};

/**
 * @brief Struct representing single LED chip.
 * 
 */
class Led {
	// Public stuff
	public:
	// Functions
	/**
	 * @brief Sets new LED color.
	 * 
	 * @param r New 8-bit red color value.
	 * @param g New 8-bit green color value.
	 * @param b New 8-bit blue color value.
	 * @param color New 24-bit color in RGB format.
	 * @param on New LED status. Set to \c 0 if to turn off LED. This parameter is optional.
	 * @return No return value.
	 * @{
	 */
	void rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t on = 1);
	void rgb(uint32_t color, uint8_t on = 1);
	/**@}*/	

	/**
	 * @brief Sets new LED's brightness value.
	 * 
	 * @param value New brightness value, ranging from \c 0 to \c 100 
	 * @return No return value.
	 * 
	 * @note \c value over 100 will be stored as 100.
	 */
	void brightness(uint8_t value);

	/**
	 * @brief Turns on LED.
	 * 
	 * @return No return value.
	 */
	inline void on(void);

	/**
	 * @brief Turns off LED.
	 * 
	 * @return No return value.
	 */
	inline void off(void);

	/**
	 * @brief Toggles LED.
	 * 
	 * @return No return value.
	 */
	inline void toggle(void);

	/**
	 * @brief Fetch output color.
	 * 
	 * @param idx Color index.
	 * @return 8-bit color value from desired index. 
	 */
	inline uint8_t getColor(uint8_t idx); 

	/**
	 * @brief Get LED config.
	 * 
	 * @return \c 0 if LED is off.
	 * @return \c 1 if LED is on.
	 */
	inline uint8_t getConfig(void);

	/**
	 * @brief Calculates new RGB values using brightness.
	 * 
	 * @return No return value.
	 */
	void adjustColor(void);

	/**
	 * @brief Reset LED to default values.
	 * 
	 * @return No return value.
	 */
	void reset(void);


	// Protected stuff
	protected:
	// Variables
	/**
	 * @brief 24-bit color data in RGB format.
	 * 
	 */
	uint8_t color[3] = { 0x00, 0x00, 0x00 };

	/**
	 * @brief LED config variable.
	 * 
	 * First seven bits are reserved for LED brightness. Last bit(MSB) is reserved for toggling LED.
	 * It is possible to turn off or on LED without changing LED's color.
	 */
	uint8_t config = (1 << PROG_LED_STATUS_BIT) | (100 << PROG_LED_BRGHT_BIT);

	/**
	 * @brief 24-bit color data calculated using brightness value.
	 * 
	 */
	uint8_t outputColor[3] = { 0x00, 0x00, 0x00 };
};

/** @}*/

#endif // _PROGLED_H_

// END WITH NEW LINE

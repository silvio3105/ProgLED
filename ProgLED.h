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
 * @brief Use FPU for RGB2HSV and HSV2RGB conversion.
 * 
 */
#define USE_FPU					1


#define PROG_LED_STATUS_BIT		0
#define PROG_LED_STATUS_MASK	0b00000001


// ----- ENUMS
typedef enum: uint8_t {
	PROG_LED_RGB, /**< RGB 24-bit format*/
	PROG_LED_GRB /**< GRB 24-bit format*/
} ProgLED_format_t;

typedef enum: uint8_t {
	LED_IDLE,
	LED_UPDATING
} ProgLED_status_t;

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
class ProgLED : protected Led {
	// Public stuff
	public:
	/**
	 * @brief Object constructor for ProgLED.
	 * 
	 * @param start Pointer to external handler used for starting LEDs update.
	 * @param stop Pointer to external handler used for stoping LEDs update.
	 * @param format LED color format. See \ref ProgLED_format_t
	 * @return No return value.
	 */
	ProgLED(extHandler start, extHandler stop, ProgLED_format_t config = PROG_LED_GRB);

	/**
	 * @brief Object deconstructor.
	 * 
	 * @return No return value.
	 */
	~ProgLED(void);

	/**
	 * @brief ProgLED init function.
	 * 
	 * @param ledsNum Number of LEDs to control.
	 * @param lineType Line color type. See \ref ProgLED_type_t
	 * @return \ref PROG_LED_OK if everything is OK.
	 * @return \ref PROG_LED_NOK if memory for \c ledsNum LEDs could not be allocated.
	 */
	uint8_t init(uint8_t ledsNum, ProgLED_type_t lineType);

	/**
	 * @brief Resets LED to default color and config.
	 * 
	 * @return No return value.
	 */
	void reset(void);

	/**
	 * @brief Toggles LED status between on and off.
	 * 
	 * @return No return value.
	 */
	inline void toggle(void);

	/**
	 * @brief Turns off LED.
	 * 
	 * @return No return value.
	 */
	inline void off(void);

	/**
	 * @brief Turns on LED.
	 * 
	 * @return No return value.
	 */
	inline void on(void);

	/**
	 * @brief Calls external handler to start clocking out colors from @ref leds memory in \ref format
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
	 * @brief Sets brightness for signle LED.
	 * 
	 * @param ledIdx LED index, starting from 0.
	 * @param value Brightness value between 0 and 100%.
	 * @return No return value.
	 * @{
	 */
	void brightness(uint8_t ledIdx, uint8_t value);
	void brightness(uint8_t value);
	/**@}*/


	// Private stuff
	private:
	/**
	 * @brief Number of LEDs to control.
	 * 
	 */
	uint8_t ledCount = 0;

	/**
	 * @brief Pointer to LEDs data in memory(heap).
	 * 
	 */
	Led* leds = nullptr;

	/**
	 * @brief Index for RGB colors in \ref Led::color struct.
	 * 
	 */
	uint8_t rIdx = 0, gIdx = 1, bIdx = 2;
	
	/**
	 * @brief LED line color format. See \ref ProgLED_format_t
	 * 
	 */
	ProgLED_format_t format = PROG_LED_GRB;

	/**
	 * @brief LED index during clocking out.
	 * 
	 */
	uint8_t ledIdx = 0;

	/**
	 * @brief Bit index at \ref ledIdx during clocking out.
	 * 
	 */
	uint8_t ledBit = 0;

	/**
	 * @brief Updates color in memory.
	 * 
	 * @param value 8-bit color value. 
	 * @param idx Color index. \ref rIdx \ref gIdx \ref bIdx
	 * @return No return value.
	 */
	inline void led(uint8_t value, uint8_t idx);

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
};

/**
 * @brief Struct representing single LED chip.
 * 
 */
class Led {
	// Protected stuff
	protected:
	/**
	 * @brief 24-bit color data in RGB format.
	 * 
	 */
	uint8_t color[3] = { 0x00, 0x00, 0x00 };

	/**
	 * @brief LED status variable.
	 * 
	 * First seven bits are reserved for LED brightness. Last bit(MSB) is reserved for toggling LED.
	 */
	uint8_t status = 0xFF;



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
};

/** @}*/

#endif // _PROGLED_H_

// END WITH NEW LINE

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

This License shall be included in all methodal textual files.
*/

#ifndef _PROGLED_H_
#define _PROGLED_H_

// ----- INCLUDE FILES
#include            <stdint.h>

/** \addtogroup ProgLED
 * 
 * Frameworkelss library for programmable RGB LEDs. 
 * @{
*/

// ----- DEFINES
// RETURN CODES
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

// CONFIGURATIONS
/**
 * @brief Use FPU for color format conversions.
 * 
 * \c 0 FPU will not be used for color conversions.
 * \c 1 FPU will be used for color conversions.
 */
#define USE_FPU					0

// BITFIELDS
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

// COLORS
#define COL_WHITE				0xFF, 0xFF, 0xFF /**< @brief Snippet for white color. */
#define COL_BLACK				0x00, 0x00, 0x00 /**< @brief Snippet for black. */
#define COL_RED					0xFF, 0x00, 0x00 /**< @brief Snippet for red color. */
#define COL_GREEN				0x00, 0xFF, 0x00 /**< @brief Snippet for green color. */
#define COL_BLUE				0x00, 0x00, 0xFF /**< @brief Snippet for blue color. */


// ----- ENUMATORS
/**
 * @brief Enum for 24-bit color format.
 * 
 */
typedef enum ProgLED_format_t: uint8_t {
	PROG_LED_RGB,  // RGB 24-bit format
	PROG_LED_GRB // GRB 24-bit format
};

/**
 * @brief Enum for LED line status.
 * 
 */
typedef enum ProgLED_status_t: uint8_t {
	LINE_IDLE,
	LINE_CLOCKING
};


// ----- TYPEDEFS
/**
 * @brief External hanlder typedef for handling data clocking.
 * 
 * @param bit Value of first bit.
 * @return No return value.
 */
typedef void (*extHandler)(uint8_t bit);

/**
 * @brief Type definition for LED index.
 * 
 */
typedef uint16_t ledIdx_t;


// ----- CLASSES
template <uint16_t ledNum>
/**
 * @brief Class representing single LED line.
 * 
 */
class ProgLED : public Led {
	// PUBLIC STUFF
	public:
	// CONSTUCTORS AND DECONSTRUCTORS DECLARATIONS
	/**
	 * @brief Object constructor for ProgLED.
	 * 
	 * @param start Pointer to external handler used for starting LEDs update.
	 * @param stop Pointer to external handler used for stoping LEDs update.
	 * @param ledFormat LED color format. See \ref ProgLED_format_t
	 * @return No return value.
	 */
	ProgLED(extHandler start, extHandler stop, ProgLED_format_t ledFormat = PROG_LED_GRB);

	/**
	 * @brief Object deconstructor.
	 * 
	 * @return No return value.
	 */
	~ProgLED(void);


	// VARIABLES
	/**
	 * @brief LED array.
	 * 
	 */
	Led led[ledNum];	


	// METHOD DECLARATIONS
	/**
	 * @brief ProgLED init method.
	 * 
	 * Checks external handlers pointers.
	 * Sets LEDs to default values.
	 * 
	 * @return \ref PROG_LED_OK if everything is OK.
	 * @return \ref PROG_LED_NOK if external handlers are \c nullptr.
	 */
	uint8_t init(void);

	/**
	 * @brief Sets color for whole LED line.
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
	/** @}*/

	/**
	 * @brief Sets new brightness for whole LED line.
	 * 
	 * @param value New brightness value, ranging from \c 0 to \c 100 
	 * @return No return value.
	 * 
	 * @note \c value over 100 will be stored as 100.
	 */
	void brightness(uint8_t value);

	/**
	 * @brief Resets LED line to default color and config.
	 * 
	 * @return No return value.
	 */
	void reset(void);

	/**
	 * @brief Toggles LEDs status between on and off for whole LED line.
	 * 
	 * @return No return value.
	 */
	void toggle(void);

	/**
	 * @brief Turns on LED line.
	 * 
	 * @return No return value.
	 */
	void on(void);

	/**
	 * @brief Turns off LED line.
	 * 
	 * @return No return value.
	 */
	void off(void);

	/**
	 * @brief Calls external handler to start clocking out LED data.
	 * 
	 * @return No return value. 
	 */
	void update(void);

	/**
	 * @brief Calls external hanlder to stop clocking out LED data.
	 * 
	 * @return No return value.
	 */
	void stop(void);

	/**
	 * @brief Fetches bit from LED color.
	 * 
	 * @param bit Reference where next bit will be stored. 
	 * @return \ref PROG_LED_CONTINUE if more bits exists.
	 * @return \ref PROG_LED_OK if last bit is fetched.
	 */
	uint8_t fetchBit(uint8_t& bit);

	/**
	 * @brief Method for fetching index for red color.
	 * 
	 * @return Index for red color at \ref Led::color
	 */
	inline uint8_t getRIdx(void) const;

	/**
	 * @brief Method for fetching index for green color.
	 * 
	 * @return Index for green color at \ref Led::color
	 */
	inline uint8_t getGIdx(void) const;

	/**
	 * @brief Method for fetching index for blue color.
	 * 
	 * @return Index for blue color at \ref Led::color
	 */
	inline uint8_t getBIdx(void) const;		


	// PRIVATE STUFF
	private:
	// VARIABLES
	ledIdx_t ledCount = ledNum; /**< @brief Number of LEDs to control. */
	uint8_t ledIdx = 0; /**< @brief LED selector during clocking out. */
	uint8_t ledByte = 0; /**< @brief LED data byte selector at @ref ledIdx during clocking out. */
	uint8_t ledBit = 0; /**< @brief Bit selector at \ref ledByte during clocking out. */
	extHandler startHandler = nullptr; /**< @brief Pointer to external handler called to start clocking out. */
	extHandler stopHandler = nullptr; /**< @brief Pointer to external handler called to stop clocking out. */
	ProgLED_status_t lineStatus = LINE_IDLE; /**< @brief LED line status. */
	uint8_t rIdx = 0; /**< @brief Index for red color in \ref Led::color class. */	
	uint8_t gIdx = 1; /**< @brief Index for green color in \ref Led::color class. */	
	uint8_t bIdx = 2; /**< @brief Index for blue color in \ref Led::color class. */	


	// METHOD DECLARATIONS
	#if USE_FPU == 0
	/**
	 * @brief Converts RGB color format to HSV color format using fixed point math.
	 * 
	 * @param in Reference to input array in RGB format.
	 * @param out Reference to HSV output array.
	 * @return No return value. 
	 * 
	 * @warning Method does not work without FPU.
	 */
	void rgb2HSV(uint8_t (&in)[3], int32_t (&out)[3]);

	/**
	 * @brief Converts HSV color format to RGB color format using fixed point math.
	 * 
	 * @param in Reference to input array in HSV format.
	 * @param out Reference to RGB output array.
	 * @return No return value.
	 * 
	 * @warning Method does not work without FPU.
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
	 * @note Method requires FPU for best performance during calculations.
	 */
	void rgb2HSV(uint8_t (&in)[3], float (&out)[3]);	

	/**
	 * @brief Converts HSV color format to RGB color using float point math.
	 * 
	 * @param in Reference to input array in HSV format.
	 * @param out Reference to RGB output array.
	 * @return No return value.
	 * 
	 * @note Method requires FPU for best performance during calculations.
	 * @warning Method does not convert right HSV(0 0 100)/RGB(255 255 255) back to RGB values.
	 */
	void hsv2RGB(float (&in)[3], uint8_t (&out)[3]);
	#endif // USE_FPU	
};

template <typename T>
/**
 * @brief Class representing single LED chip.
 * 
 */
class Led {
	// PUBLIC STUFF
	public:
	// METHOD DECLARATIONS
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
	 * @brief Fetch output color.
	 * 
	 * @param idx Color index.
	 * @return 8-bit color value from desired index. 
	 */
	inline uint8_t getColor(uint8_t idx) const; 

	/**
	 * @brief Get LED config.
	 * 
	 * @return \c 0 if LED is off.
	 * @return \c 1 if LED is on.
	 */
	inline uint8_t getConfig(void) const;

	/**
	 * @brief Reset LED to default values.
	 * 
	 * @return No return value.
	 */
	void reset(void);

	/**
	 * @brief Toggles LED.
	 * 
	 * @return No return value.
	 */
	inline void toggle(void);

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
	 * @brief Sets new LED's brightness value.
	 * 
	 * @param value New brightness value, ranging from \c 0 to \c 100 
	 * @return No return value.
	 * 
	 * @note \c value over 100 will be stored as 100.
	 */
	void brightness(uint8_t value);


	// PRIVATE STUFF
	private:
	// VARIABLES
	uint8_t color[3] = { 0x00, 0x00, 0x00 }; /**< @brief 24-bit color data in RGB format. */

	/**
	 * @brief LED config variable.
	 * 
	 * First seven bits are reserved for LED brightness. Last bit(MSB) is reserved for toggling LED.
	 * It is possible to turn off or on LED without changing LED's color.
	 */
	uint8_t config = (1 << PROG_LED_STATUS_BIT) | (100 << PROG_LED_BRGHT_BIT);
	uint8_t outputColor[3] = { 0x00, 0x00, 0x00 }; /**< @brief 24-bit color data calculated using brightness value. */


	// METHOD DECLARATIONS
	/**
	 * @brief Calculates new RGB values using brightness.
	 * 
	 * @return No return value.
	 */
	void adjustColor(void);	
};

/** @}*/

#endif // _PROGLED_H_

// END WITH NEW LINE

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
#define PROG_LED_NOK			0 /**< @brief Return code for not OK status. */
#define PROG_LED_OK				1 /**< @brief Return code for OK status. */
#define PROG_LED_CONTINUE		2 /**< @brief Return code for continue status during clocking. */

// CONFIGURATIONS
/**
 * @brief Use FPU for color format conversions.
 * 
 * \c 0 FPU will not be used for color conversions.
 * \c 1 FPU will be used for color conversions.
 */
#define USE_FPU					0

// BITFIELDS
#define PROG_LED_STATUS_BIT		7 /**< @brief Status bit in LED's config variable. */
#define PROG_LED_STATUS_MASK	0b10000000 /**< @brief Status mask in LED's config variable. */

#define PROG_LED_BRGHT_BIT		0 /**< @brief Number of first bit for LED brightness in config variable. */
#define PROG_LED_BRGHT_MASK		0b01111111 /**< @brief Brightness mask in LED's config variable. */

// COLORS
#define COL_WHITE				0xFF, 0xFF, 0xFF /**< @brief Snippet for white color. */
#define COL_BLACK				0x00, 0x00, 0x00 /**< @brief Snippet for black. */
#define COL_RED					0xFF, 0x00, 0x00 /**< @brief Snippet for red color. */
#define COL_GREEN				0x00, 0xFF, 0x00 /**< @brief Snippet for green color. */
#define COL_BLUE				0x00, 0x00, 0xFF /**< @brief Snippet for blue color. */
#define COL_PINK				0xFF, 0x00, 0xFF /**< @brief Snippet for pink color. */
#define COL_YELLOW				0xFF, 0xFF, 0x00 /**< @brief Snippet for yellow color. */
#define COL_CYAN				0x00, 0xFF, 0xFF /**< @brief Snippet for cyan color. */
#define COL_LIGHTCYAN			0xE0, 0xFF, 0xFF /**< @brief Snippet for light cyan color.  */
#define COL_DARKCYAN			0x00, 0x8B, 0x8B /**< @brief Snippet for dark cyan color. */
#define COL_TURQUOISE			0x40, 0xE0, 0xD0 /**< @brief Snippet for turquoise color. */
#define COL_ORANGE				0xFF, 0x8C, 0x00 /**< @brief Snippet for orange color. */
#define COL_AMBER				0xFF, 0xBF, 0x00 /**< @brief Snippet for amber color. */
#define COL_BROWN				0x8B, 0x45, 0x13 /**< @brief Snippet for brown color. */
#define COL_CHERRY				0xD2, 0x04, 0x2D /**< @brief Snippet for cherry color. */
#define COL_DARKRED				0x8B, 0x00, 0x00 /**< @brief Snippet for dark red color. */
#define COL_LIGHTRED			0xF8, 0x83, 0x79 /**< @brief Snippet for light red color. */
#define COL_LIME				0x9E, 0xFD, 0x38 /**< @brief Snippet for lime color. */
#define COL_LIGHTGREEN			0x00, 0xFF, 0x99 /**< @brief Snippet for light green color. */
#define COL_DARKGREEN			0x01, 0x32, 0x20 /**< @brief Snippet for dark green color. */
#define COL_ANDROID				0xA4, 0xC6, 0x39 /**< @brief Snippet for android green color. */
#define COL_NEONGREEN			0x39, 0xFF, 0x14 /**< @brief Snippet for neon green color. */
#define COL_PURPLE				0x5D, 0x3F, 0xD3 /**< @brief Snippet for purple color. */
#define COL_LIGHTVIOLET			0xCF, 0x9F, 0xFF /**< @brief Snippet for light violet color. */


// ----- ENUMATORS
// SOON: Edit method LED::rgb if new color format is added
/**
 * @brief Enum for 24-bit color format.
 * 
 */
typedef enum ProgLED_format_t: uint8_t {
	PROG_LED_RGB = 0b00100100,  /**< RGB 24-bit color format. */
	PROG_LED_GRB = 0b00100001 /**< GRB 24-bit color format. */
};

/**
 * @brief Enum for LED line status.
 * 
 */
typedef enum ProgLED_status_t: uint8_t {
	LINE_IDLE, /**< LED line is in idle state. */
	LINE_CLOCKING /**< LED line is in clocking state. */
};

/**
 * @brief Color channels bit positions.
 * 
 */
typedef enum ProgLED_chIdx_t : uint8_t {
	RED_IDX = 0,
	GREEN_IDX = 2,
	BLUE_IDX = 4
};


// ----- TYPEDEFS
/**
 * @brief External hanlder typedef for handling data clocking.
 * 
 * @param bit Value of first bit.
 * @return No return value.
 */
typedef void (*extHandler)(uint8_t bit);
typedef uint16_t ledIdx_t; /**< @brief Type definition for LED index. */


// ----- CLASSES
template <ledIdx_t ledNum>
/**
 * @brief Class representing single LED line.
 * 
 */
class ProgLED {
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
	LED led[ledNum]; /**< @brief LED array. */	


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

class LED { /**< @brief Class representing single LED chip. */
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
	 * @return 8-bit color value from desired \c idx 
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

	/**
	 * @brief Sets LED color format.
	 * 
	 * @param bitmap LED color format.
	 * @return No return value.
	 */
	inline void setChannelBitmap(uint8_t bitmap);


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

	/**
	 * @brief Color channel bitmap variable.
	 * 
	 * Variable format is \c 0b00BBGGRR where \c RR bits represent channel index for red color, \c GG bits represent channel index for green color and \c BB bits represent channel index for blue color.
	 * Variable is set via \ref setChannelBitmap method and color channel index is fetched via \ref getChannelIdx method.
	 * 
	 */
	uint8_t chBitmap = 0;


	// METHOD DECLARATIONS
	/**
	 * @brief Calculates new RGB values using brightness.
	 * 
	 * @return No return value.
	 */
	void adjustColor(void);

	/**
	 * @brief Gets channel index for desired color.
	 * 
	 * @param idx Desired color channel. See \ref ProgLED_chIdx_t
	 * @return Channel index.
	 */
	uint8_t getChannelIdx(ProgLED_chIdx_t idx);
};


/** @}*/

#endif // _PROGLED_H_

// END WITH NEW LINE

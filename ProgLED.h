/**
 * @file ProgLED.h
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Header file for ProgLED library.
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

// ----- CONFIGURATIONS
/**
 * @brief Use FPU for color format conversions.
 * 
 * \c 0 FPU will not be used for color conversions.
 * \c 1 FPU will be used for color conversions.
 */
#define USE_FPU					0

// ----- INCLUDE FILES
#include			<sStd.h>
#include            <stdint.h>
#if USE_FPU == 1
#include			<math.h>
#endif // USE_FPU

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

// BITFIELDS
#define PROG_LED_STATUS_BIT		7 /**< @brief Status bit in LED config variable. */
#define PROG_LED_STATUS_MASK	0b10000000 /**< @brief Status mask in LED config variable. */

#define PROG_LED_BRGHT_BIT		0 /**< @brief Number of first bit for LED brightness in config variable. */
#define PROG_LED_BRGHT_MASK		0b01111111 /**< @brief Brightness mask in LED config variable. */


// ----- MACRO FUNCTIONS
/**
 * @brief Code snippet for looping through LEDs
 * 
 */
#define ProgLED_LOOP \
	for (ledIdx_t i = 0; i < ledCount; i++)


// ----- ENUMATORS
/**
 * @brief RGB colors enum.
 * 
 */
enum ProgLED_rgb_t : uint32_t {
	BLACK = 0x000000, /**< @brief "Black" color. */
	WHITE = 0xFFFFFF, /**< @brief White color. */

	RED = 0xFF0000, /**< @brief Red color. */
	GREEN = 0x00FF00, /**< @brief Green color. */
	BLUE = 0x0000FF, /**< @brief Blue color. */
	PINK = 0xFF00FF, /**< @brief Pink color. */
	YELLOW = 0xFFFF00, /**< @brief Yellow color. */
	CYAN = 0x00FFFF, /**< @brief Cyan color. */

	ORANGE = 0xFF5000, /**< @brief Orange color. */
	AMBER = 0xFF9000, /**< @brief Amber color. */
	PURPLE = 0x2804B5, /**< @brief Purple color. */
	MAROON = 0x800000, /**< @brief Marron color. */

	ANDROID_GREEN = 0xA4C639, /**< @brief Android green color. */
	NEON_GREEN = 0x39FF14, /**< @brief Neon green color. */
};

/**
 * @brief 24-bit color type enum.
 * 
 */
enum ProgLED_format_t: uint8_t {
	PROG_LED_RGB = 0b00100100,  /**< RGB 24-bit color format. */
	PROG_LED_GRB = 0b00100001 /**< GRB 24-bit color format. */
};

/**
 * @brief LED line state enum.
 * 
 */
enum ProgLED_state_t: uint8_t {
	IDLE = 0, /**< LED line is in idle state. */
	CLOCKING = 1 /**< LED line is in clocking state. */
};

/**
 * @brief Color channels bit position.
 * 
 */
enum ProgLED_chIdx_t : uint8_t {
	IDX_RED = 0b0000, /**< @brief Red channel index. */
	IDX_GREEN = 0b0010, /**< @brief Green channel index. */
	IDX_BLUE = 0b0100, /**< @brief Blue channel index. */
	// IDX_WHITE = 0b0110 /**< @brief White channel index. */
};

/**
 * @brief LED status enum.
 * 
 */
enum ProgLED_status_t : uint8_t {
	OFF = 0, /**< @brief LED off status */
	ON = 1 /**< @brief LED on status. */
};

/**
 * @brief Line type enum.
 * 
 */
enum ProgLED_line_t : uint8_t {
	BLOCKING = 0, /**< @brief Blocking line type. */
	NON_BLOCKING = 1 /**< @brief Non-blocking line type. */
};


// ----- TYPEDEFS
/**
 * @brief External hanlder typedef for handling data clocking.
 * 
 * @param bit First bit to clock out. If it is \c -1 then ignore it.
 * @return No return value.
 */
typedef void (*extHandler)(int8_t bit);
typedef uint16_t ledIdx_t; /**< @brief Type definition for LED index. */


// ----- CLASSES
/**
 * @brief Class representing single LED chip.
 * 
 * @tparam format Color format. See \ref ProgLED_format_t
 */
template<ProgLED_format_t format>
class LED {
	// PUBLIC STUFF
	public:
	// METHOD DECLARATIONS
	/**
	 * @brief Set new LED color.
	 * 
	 * @param r New 8-bit red color value.
	 * @param g New 8-bit green color value.
	 * @param b New 8-bit blue color value.
	 * @param brght New brightness. Set to \c -1 to keep old brightness.
	 * @param on New LED status. See @ref LED_status_t
	 * @return No return value.
	 */
	void rgb(uint8_t r, uint8_t g, uint8_t b, int8_t brght = -1, ProgLED_status_t on = ProgLED_status_t::ON)
	{
		// Write new RGB color values
		color[getChannelIdx(IDX_RED)] = r;
		color[getChannelIdx(IDX_GREEN)] = g;
		color[getChannelIdx(IDX_BLUE)] = b;

		// Write new status bit keeping old brightness bits
		SSTD_BIT_CLEAR(config, PROG_LED_STATUS_BIT);
		config |= (on << PROG_LED_STATUS_BIT);

		// Set new brightness
		if (brght > -1) brightness((uint8_t)brght);		
	}

	/**
	 * @brief Set new LED color.
	 * 
	 * @param col New 24-bit color in RGB format.
	 * @param brght New brightness. Set to \c -1 to keep old brightness.
	 * @param on New LED status. See @ref LED_status_t
	 * @return No return value.
	 */	
	void rgb(uint32_t col, int8_t brght = -1, ProgLED_status_t on = ProgLED_status_t::ON)
	{
		// Extract RGB bytes from 32-bit color value and pass params to main RGB method
		rgb((col & 0x00FF0000) >> 16, (col & 0x0000FF00) >> 8, col & 0x000000FF, brght, on);		
	}

	/**
	 * @brief Set new LED color.
	 * 
	 * @param col New 24-bit color in RGB format. See \ref ProgLED_rgb_t
	 * @param brght New brightness. Set to \c -1 to keep old brightness.
	 * @param on New LED status. See @ref LED_status_t
	 * @return No return value.
	 */	
	void rgb(ProgLED_rgb_t col, int8_t brght = -1, ProgLED_status_t on = ProgLED_status_t::ON)
	{
		// Extract RGB bytes from 32-bit color value and pass params to main RGB method
		rgb((col & 0x00FF0000) >> 16, (col & 0x0000FF00) >> 8, col & 0x000000FF, brght, on);		
	}

	/**
	 * @brief Fetch output color.
	 * 
	 * @param idx Color index.
	 * @return 8-bit color value from desired \c idx 
	 */
	inline uint8_t getColor(uint8_t idx) const
	{
		// Return LED output color
		return outputColor[idx];
	}

	/**
	 * @brief Get LED config.
	 * 
	 * @return \c 0 LED configuration.
	 */
	inline uint8_t getConfig(void) const
	{
		// Return LED configuration
		return config;
	}

	/**
	 * @brief Reset LED to default values.
	 * 
	 * @return No return value.
	 */
	void reset(void)
	{
		// Reset color values to black
		for (uint8_t i = 0; i < 3; i++)
		{
			color[i] = 0x00;
			outputColor[i] = 0x00;
		}

		// Set default config value
		config = (1 << PROG_LED_STATUS_BIT) | (100 << PROG_LED_BRGHT_BIT);		
	}

	/**
	 * @brief Toggle LED.
	 * 
	 * @return No return value.
	 */
	inline void toggle(void)
	{
		SSTD_BIT_TOGGLE(config, PROG_LED_STATUS_BIT);
	}

	/**
	 * @brief Turn on LED.
	 * 
	 * @return No return value.
	 */
	inline void on(void)
	{
		SSTD_BIT_SET(config, PROG_LED_STATUS_BIT);
	}

	/**
	 * @brief Turn off LED.
	 * 
	 * @return No return value.
	 */
	inline void off(void)
	{
		SSTD_BIT_CLEAR(config, PROG_LED_STATUS_BIT);
	}

	/**
	 * @brief Set new LED brightness value.
	 * 
	 * @param value New brightness value, ranging from \c 0 to \c 100 
	 * @return No return value.
	 * 
	 * @note \c value over 100 will be stored as 100.
	 */
	void brightness(uint8_t value)
	{
		// Make sure 100 is maximum value
		if (value > 100) value = 100;

		// Clear brightness bits
		config &= ~PROG_LED_BRGHT_MASK;

		// Write new brightness
		config |= value;		
	}

	/**
	 * @brief Get LED brightness.
	 * 
	 * @return LED brightness.
	 */
	inline uint8_t brightness(void) const
	{
		// Return LED brightness
		return (config & PROG_LED_BRGHT_MASK);
	}

	/**
	 * @brief Calculate new output RGB values using brightness.
	 * 
	 * @return No return value.
	 */
	void adjust(void)
	{
		uint8_t tmp = config & PROG_LED_BRGHT_MASK;

		// Scale RGB values using brightness
		for (uint8_t i = 0; i < SSTD_ARRAY(outputColor); i++) outputColor[i] = color[i] * tmp * 10 / 100 / 10;		
	}


	// PRIVATE STUFF
	private:
	// VARIABLES
	uint8_t color[3] = { 0x00 }; /**< @brief Color data. */

	/**
	 * @brief LED config variable.
	 * 
	 * First seven bits are reserved for LED brightness. Last bit(MSB) is reserved for LED status(off/on)
	 * It is possible to turn off or on LED without changing LED color or brightness.
	 */
	uint8_t config = (1 << PROG_LED_STATUS_BIT) | (100 << PROG_LED_BRGHT_BIT);
	uint8_t outputColor[3] = { 0x00 }; /**< @brief Output color data. */

	/**
	 * @brief LED color format variable.
	 * 
	 * Variable format is \c 0bWWBBGGRR where \c RR bits represent channel index for red color, \c GG bits represent channel index for green color, \c BB bits represent channel index for blue color and \c WW bits represent channel index for white color.
	 */
	ProgLED_format_t ledFormat = format;


	// METHOD DECLARATIONS
	/**
	 * @brief Get channel index for desired color.
	 * 
	 * @param idx Desired color channel. See \ref ProgLED_chIdx_t
	 * @return Channel index.
	 */
	uint8_t getChannelIdx(ProgLED_chIdx_t idx)
	{
		// Return index of outputColor for desired color
		return (ledFormat & (0b11 << idx)) >> idx;		
	}
};


/**
 * @brief Class representing single LED line.
 * 
 * @tparam ledNum Number of LEDs in LED
 * @tparam ledFormat LED color format. See \ref ProgLED_format_t
 */
template <ledIdx_t ledNum, ProgLED_format_t ledFormat>
class ProgLED {
	// PUBLIC STUFF
	public:
	// VARIABLES
	LED<ledFormat> led[ledNum]; /**< @brief LED array. */

	// CONSTUCTORS AND DECONSTRUCTORS DECLARATIONS
	/**
	 * @brief ProgLED object constructor.
	 * 
	 * @param startFunc Pointer to external handler used for starting LEDs update.
	 * @param stopFunc Pointer to external handler used for stoping LEDs update.
	 * @return No return value.
	 */
	ProgLED(extHandler startFunc, extHandler stopFunc)
	{
		// Set external handlers
		startHandler = startFunc;
		stopHandler = stopFunc;

		// Set line status
		lineStatus = ProgLED_state_t::IDLE;		
	}

	/**
	 * @brief ProgLED object deconstructor.
	 * 
	 * @return No return value.
	 */
	~ProgLED(void)
	{
		// Set external handlers to nullptr
		startHandler = stopHandler = nullptr;		
	}

	// METHOD DECLARATIONS
	/**
	 * @brief Init LED line.
	 * 
	 * Checks external handlers pointers.
	 * Sets LEDs to default values.
	 * 
	 * @return \ref PROG_LED_OK if everything is OK.
	 * @return \ref PROG_LED_NOK if external handlers are \c nullptr.
	 */
	uint8_t init(void)
	{
		// Return NOK status if one of handlers is nullptr
		if (!startHandler || !stopHandler) return PROG_LED_NOK;

		// Set all LEDs to default values
		reset();

		// Reset LED and bit indexes
		ledIdx = 0;
		ledByte = 0;
		ledBit = 8;	

		// Return OK status
		return PROG_LED_OK;		
	}

	/**
	 * @brief Set color for whole LED line.
	 * 
	 * @param r New 8-bit red color value.
	 * @param g New 8-bit green color value.
	 * @param b New 8-bit blue color value.
	 * @param brght New brightness. Set to \c -1 to keep old brightness.
	 * @param on New LED status. See \ref ProgLED_status_t
	 * @return No return value.
	 */
	void rgb(uint8_t r, uint8_t g, uint8_t b, int8_t brght = -1, ProgLED_status_t on = ProgLED_status_t::ON)
	{
		ProgLED_LOOP led[i].rgb(r, g, b, brght, on);
	}

	/**
	 * @brief Set color for whole LED line.
	 * 
	 * @param color New 24-bit color in RGB format.
	 * @param brght New brightness. Set to \c -1 to keep old brightness.
	 * @param on New LED status. See \ref ProgLED_status_t
	 * @return No return value.
	 */
	void rgb(uint32_t color, int8_t brght = -1, ProgLED_status_t on = ProgLED_status_t::ON)
	{
		rgb((color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, color & 0x000000FF, brght, on);		
	}

	/**
	 * @brief Set new brightness for whole LED line.
	 * 
	 * @param value New brightness value, ranging from \c 0 to \c 100 
	 * @return No return value.
	 * 
	 * @note \c value over 100 will be stored as 100.
	 */
	void brightness(uint8_t value)
	{
		// Pass value to LED brightness method
		ProgLED_LOOP led[i].brightness(value);		
	}

	/**
	 * @brief Reset LED line to default color, brightness and status.
	 * 
	 * @return No return value.
	 */
	void reset(void)
	{
		// Call reset method for every LED
		ProgLED_LOOP led[i].reset();
	}

	/**
	 * @brief Toggle LED status between on and off for whole LED line.
	 * 
	 * @return No return value.
	 */
	void toggle(void)
	{
		// Toggle all LEDs
		ProgLED_LOOP led[i].toggle();
	}

	/**
	 * @brief Turn on all LEDs in LED line.
	 * 
	 * @return No return value.
	 */
	void on(void)
	{
		// Turn on every LED
		ProgLED_LOOP led[i].on();
	}

	/**
	 * @brief Turn off all LEDs in LED line.
	 * 
	 * @return No return value.
	 */
	void off(void)
	{
		// Turn off every LED
		ProgLED_LOOP led[i].off();
	}

	/**
	 * @brief Call external handler to start clocking out LED data.
	 * 
	 * @param type Update type. See \ref ProgLED_line_t
	 * @return No return value. 
	 */
	void update(ProgLED_line_t type)
	{
		// Exit if line is already clocking
		if (status() == ProgLED_state_t::CLOCKING) return;

		// Calculate new RGB values for each LED
		ProgLED_LOOP led[i].adjust();

		// Reset LED and bit index
		ledIdx = 0;
		ledByte = 0;
		ledBit = 8;	

		// Set new LED line status
		lineStatus = ProgLED_state_t::CLOCKING;

		// Call external handler
		if (type == ProgLED_line_t::NON_BLOCKING) startHandler(-1); // Just to fullfill parameter
		else
		{
			uint8_t bit = 0;

			// Fetch first bit
			fetchBit(bit);	

			// Call external handler with first bit to clock out
			startHandler(bit);
		}
	}

	/**
	 * @brief Call external hanlder to stop clocking out LED data.
	 * 
	 * @return No return value.
	 */
	void stop(void)
	{
		// Exit if line is at idle state
		if (status() == ProgLED_state_t::IDLE) return;

		// Set new LED line status
		lineStatus = ProgLED_state_t::IDLE;	

		// Call external handler, parameter is unused
		stopHandler(-1); // Just to fullfill parameter	
	}

	/**
	 * @brief Fetch bit from LED color output data.
	 * 
	 * @param bit Reference for output. 
	 * @return \c PROG_LED_CONTINUE if more bits exists.
	 * @return \c PROG_LED_OK if last bit is fetched.
	 */
	uint8_t fetchBit(uint8_t& bit)
	{	
		// Get bit from color output data
		bit = SSTD_BBIT(led[ledIdx].getColor(ledByte), (ledBit - 1));

		// Set bit to 0 if LED status is 0(OFF)
		if (!(SSTD_BIT(led[ledIdx].getConfig(), PROG_LED_STATUS_BIT))) bit = 0;

		// Move bit index
		ledBit--;

		// If end of byte is reached
		if (!ledBit)
		{
			// Reset bit index to 0
			ledBit = 8;

			// Move index to next byte
			ledByte++;

			// If end of LED is reached
			if (ledByte == 3)
			{
				// Move index to next LED
				ledByte = 0;
				ledIdx++;

				// If end is reached return OK status
				if (ledIdx == ledNum) return PROG_LED_OK;				
			}
		}

		// Return continue status
		return PROG_LED_CONTINUE;		
	}

	/**
	 * @brief Fill buffer with \c qty number of LED data.
	 * 
	 * @param buff Pointer to buffer.
	 * @param len Length of \c buff
	 * @param qty Number of LEDs to buff.
	 * @param low Value representing bit \c 0
	 * @param high Value representing bit \c 1
	 * @return \c PROG_LED_NOK if buffer is smaller than required by \c qty parameter.
	 * @return \c PROG_LED_OK if buffer is filled.
	 */
	uint8_t fillBuffer(uint8_t* buff, uint32_t len, uint32_t qty, uint8_t low, uint8_t high)
	{
		uint8_t bit = 0;
		uint8_t ret = PROG_LED_OK;
		uint32_t idx = 0;

		// Increase qty by 24 bits
		qty *= 24;

		// Buffer is smaller than required by qty parameter
		if (len < qty) return PROG_LED_NOK;

		// Fill buffer while continue flag is received and qty is not 0
		do
		{
			// Fetch bit
			ret = fetchBit(bit);

			// Write high or low value
			if (bit) buff[idx] = high;
				else buff[idx] = low;

			// Move index
			idx++;

			// Decrease qty
			qty--;
		}
		while (ret != PROG_LED_OK && qty);		

		// Return OK status
		return PROG_LED_OK;
	}

	/**
	 * @brief Get LED line status.
	 * 
	 * @return LED line status. See \ref ProgLED_status_t
	 */
	inline ProgLED_state_t status(void) const
	{
		// Return line status
		return lineStatus;
	}


	// PRIVATE STUFF
	private:
	// VARIABLES
	ledIdx_t ledCount = ledNum; /**< @brief Number of LEDs to control. */
	uint16_t ledIdx = 0; /**< @brief LED index during clocking out. */
	uint8_t ledByte = 0; /**< @brief LED data byte idex at \ref ledIdx during clocking out. */
	uint8_t ledBit = 8; /**< @brief Bit index at \ref ledByte during clocking out. */
	extHandler startHandler = nullptr; /**< @brief Pointer to external handler called to start clocking out. */
	extHandler stopHandler = nullptr; /**< @brief Pointer to external handler called to stop clocking out. */
	ProgLED_state_t lineStatus = ProgLED_state_t::IDLE; /**< @brief LED line status. */

	// METHOD DECLARATIONS
	#if (USE_FPU == 0)
	/**
	 * @brief Convert RGB color format to HSV color format using fixed point math.
	 * 
	 * @param in Reference to input array in RGB format.
	 * @param out Reference to HSV output array.
	 * @return No return value. 
	 */
	void rgb2HSV(uint8_t (&in)[3], int32_t (&out)[3])
	{
	/*	uint32_t min = 0, max = 0, delta = 0;
		uint16_t rgb[3];

		// Maps input RGB values to 0 to 100
		// SOON: This part works
		rgb[0] = ProgLED_MAP(in[rIdx], 0, 255, 0, 10000);
		rgb[1] = ProgLED_MAP(in[gIdx], 0, 255, 0, 10000);
		rgb[2] = ProgLED_MAP(in[bIdx], 0, 255, 0, 10000);

		// Finds min and max values in RGB format
		// SOON: This part works
		max = ProgLED_MAX(rgb[0], rgb[1], rgb[2]);
		min = ProgLED_MIN(rgb[0], rgb[1], rgb[2]);
		delta = (max - min);

		// SOON: Here is the problem
		if (max == min) out[0] = 0;
		else if (max == rgb[0]) out[0] = (60 * ((rgb[1] - rgb[2]) / (delta / 100)) + 360) % 360;
		else if (max == rgb[1]) out[0] = (60 * ((rgb[2] - rgb[0]) / (delta / 100)) + 120) % 360;
		else out[0] = (60 * ((rgb[0] - rgb[1]) / (delta / 100)) + 240) % 360;
		
		if (!max) out[1] = 0;
			else out[1] = ((delta * 100) / max);

		out[2] = max / 100;	*/		
	}

	/**
	 * @brief Convert HSV color format to RGB color format using fixed point math.
	 * 
	 * @param in Reference to input array in HSV format.
	 * @param out Reference to RGB output array.
	 * @return No return value.
	 */
	void hsv2RGB(int32_t (&in)[3], uint8_t (&out)[3])
	{
		// SOON
	}	
	#else
	/**
	 * @brief Convert RGB color format to HSV color format using float point math.
	 * 
	 * @param in Reference to input array in RGB format.
	 * @param out Reference to HSV output array.
	 * @return No return value.
	 * 
	 * @warning For best performance, FPU is needed for calculations.
	 */
	void rgb2HSV(uint8_t (&in)[3], float (&out)[3])
	{
		float rgb[3];
		float min, max, delta;

		rgb[0] = in[rIdx] / 255.00f;
		rgb[1] = in[gIdx] / 255.00f;
		rgb[2] = in[bIdx] / 255.00f;

		max = ProgLED_MAX(rgb[0], rgb[1], rgb[2]);
		min = ProgLED_MIN(rgb[0], rgb[1], rgb[2]);
		delta = max - min;

	if (max == min) out[0] = 0.00;
	else if (max == rgb[0]) out[0] = fmod((60.00f * ((rgb[1] - rgb[2]) / delta) + 360.00f), 360.00f);
	else if (max == rgb[1]) out[0] = fmod((60.00f * ((rgb[2] - rgb[0]) / delta) + 120.00f), 360.00f);
	else if (max == rgb[2]) out[0] = fmod((60.00f * ((rgb[0] - rgb[1]) / delta) + 240.00f), 360.00f);

		if (!max) out[1] = 0.00f;
			else out[1] = (delta / max) * 100.00f;

		out[2] = max * 100.00f;		
	}

	/**
	 * @brief Convert HSV color format to RGB color using float point math.
	 * 
	 * @param in Reference to input array in HSV format.
	 * @param out Reference to RGB output array.
	 * @return No return value.
	 * 
	 * @warning Method does not convert right HSV(0 0 100)/RGB(255 255 255) back to RGB values.
	 * @warning For best performance, FPU is needed for calculations.
	 */
	void hsv2RGB(float (&in)[3], uint8_t (&out)[3])
	{
		float temp[3];  
		temp[0] = in[0];
		temp[1] = in[1] / 100.00f;
		temp[2] = in[2] / 100.00f;

		if (!in[1])
		{
			out[rIdx] = temp[1] * 255;
			out[gIdx] = out[rIdx];
			out[bIdx] = out[rIdx];
		}
		else
		{
			int32_t i;
			float f, p, q, t;

			if (temp[0] == 360.00f) temp[0] = 0.00f;
				else temp[0] /= 60.00f;

			i = (int32_t)trunc(temp[0]);
			f = temp[0] - i;

			p = temp[2] * (1.00f - temp[1]);
			q = temp[2] * (1.00f - (temp[1] * f));
			t = temp[2] * (1.00f - (temp[1] * (1.00f - f)));

			switch (i)
			{
				case 0:
				{
					out[rIdx] = temp[2] * 255;
					out[gIdx] = t * 255;
					out[bIdx] = p * 255;
					break;
				}
		
				case 1:
				{
					out[rIdx] = q * 255;
					out[gIdx] = temp[2] * 255;
					out[bIdx] = p * 255;
					break;
				}
		
				case 2:
				{
					out[rIdx] = p * 255;
					out[gIdx] = temp[2] * 255;
					out[bIdx] = t * 255;
					break;
				}
		
				case 3:
				{
					out[rIdx] = p * 255;
					out[gIdx] = q * 255;
					out[bIdx] = temp[2] * 255;
					break;
				}
		
				case 4:
				{
					out[rIdx] = t * 255;
					out[gIdx] = p * 255;
					out[bIdx] = temp[2] * 255;
					break;
				}
				
				default:
				{
					out[rIdx] = temp[2] * 255;
					out[gIdx] = p * 255;
					out[bIdx] = q * 255;
					break;
				}
			}
		}		
	}
	#endif // USE_FPU	
};


/** @}*/

#endif // _PROGLED_H_

// END WITH NEW LINE

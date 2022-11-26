/**
 * @file ProgLED.cpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Source file ProgLED library.
 * @version v1.0
 * @date 2022-10-29
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

// ----- INCLUDE FILES
#include			"ProgLED.h"
#include			"sStd.h"

#if USE_FPU == 1
#include			<math.h>
#endif // USE_FPU


// ----- MACRO FUNCTIONS
/**
 * @brief Code snippet for looping through LEDs
 * 
 */
#define ProgLED_LOOP \
	for (ledIdx_t i = 0; i < ledCount; i++)


// ----- ProgLED METHODS DEFINITIONS
template<ledIdx_t ledNum>
ProgLED<ledNum>::ProgLED(extHandler start, extHandler stop, ProgLED_format_t ledFormat = PROG_LED_GRB)
{
	// Set external handlers
	startHandler = start;
	stopHandler = stop;

	// Set all LEDs to same color format
	ProgLED_LOOP led[i]->setChannelBitmap(ledFormat);	

	// Set line status
	lineStatus = LINE_IDLE;
}

template<ledIdx_t ledNum>
ProgLED<ledNum>::~ProgLED(void)
{
	// Set external handlers to nullptr
	startHandler = stopHandler = nullptr;
}

template<ledIdx_t ledNum>
uint8_t ProgLED<ledNum>::init(void)
{
	// Return NOK status if handlers are nullptr
	if (!startHandler || !stopHandler) return PROG_LED_NOK;

	// Set all LEDs to default values
	reset();

	return PROG_LED_OK;
}

template<ledIdx_t ledNum>
void ProgLED<ledNum>::rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t on = 1)
{
	ProgLED_LOOP led[i]->rgb(r, g, b, on);
}

template<ledIdx_t ledNum>
void ProgLED<ledNum>::rgb(uint32_t color, uint8_t on = 1)
{
	rgb((color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, color & 0x000000FF, on);
}

template<ledIdx_t ledNum>
void ProgLED<ledNum>::brightness(uint8_t value)
{
	ProgLED_LOOP led[i]->brightness(value);
}

template<ledIdx_t ledNum>
void ProgLED<ledNum>::reset(void)
{
	ProgLED_LOOP led[i]->reset();
}

template<ledIdx_t ledNum>
void ProgLED<ledNum>::toggle(void)
{
	ProgLED_LOOP led[i]->toggle();
}

template<ledIdx_t ledNum>
void ProgLED<ledNum>::on(void)
{
	ProgLED_LOOP led[i]->on();
}

template<ledIdx_t ledNum>
void ProgLED<ledNum>::off(void)
{
	ProgLED_LOOP led[i]->off();
}

template<ledIdx_t ledNum>
void ProgLED<ledNum>::update(void)
{
	// Exit if line is already clocking
	if (lineStatus == LINE_CLOCKING) return;

	uint8_t bit = 0;
	ledIdx = ledCount - 1;
	ledByte = ledBit = 0;

	// Fetch first bit in LED data
	fetchBit(bit);

	// Set new LED line status
	lineStatus = LINE_CLOCKING;

	// Call external handler
	startHandler(bit);	
}

template<ledIdx_t ledNum>
void ProgLED<ledNum>::stop(void)
{
	// Exit if line is at idle state
	if (lineStatus == LINE_IDLE) return;

	// Set new LED line status
	lineStatus = LINE_IDLE;

	// Call external handler, parameter is unused
	stopHandler(0);
}

template<ledIdx_t ledNum>
uint8_t ProgLED<ledNum>::fetchBit(uint8_t& bit)
{
	// If end is reached
	if (!ledIdx && ledByte == 3)
	{
		lineStatus = LINE_IDLE;
		return PROG_LED_OK;
	}
	
	// Get bit from output color
	bit = SSTD_BBIT(led[ledIdx]->getColor(ledByte), ledBit);

	// Set bit to 0 if LED status is 0(OFF)
	if (!(SSTD_BBIT(led[ledIdx]->getConfig(), PROG_LED_STATUS_BIT))) bit = 0;

	// Move bit selector
	ledBit++;

	// If end of byte is reached
	if (ledBit == 8)
	{
		// Reset bit selector to 0
		ledBit = 0;

		// Move selector to new byte
		ledByte++;

		// If end of LED is reached
		if (ledByte == 3)
		{
			// Move selector to next LED
			ledByte = 0;
			ledIdx--;
		}
	}

	return PROG_LED_CONTINUE;
}


// ----- PRIVATE ProgLED METHODS DEFINITIONS
#if USE_FPU == 0
#warning "Methods rgb2HSV and hsv2RGB do not work without FPU!"
template<ledIdx_t ledNum>
void ProgLED<ledNum>::rgb2HSV(uint8_t (&in)[3], int32_t (&out)[3])
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

template<ledIdx_t ledNum>
void ProgLED<ledNum>::hsv2RGB(int32_t (&in)[3], uint8_t (&out)[3])
{

}
#else
template<ledIdx_t ledNum>
void ProgLED<ledNum>::rgb2HSV(uint8_t (&in)[3], float (&out)[3])
{
 	float rgb[3];
	float min, max, delta;

	rgb[0] = in[rIdx] / 255.00;
	rgb[1] = in[gIdx] / 255.00;
	rgb[2] = in[bIdx] / 255.00;

	max = ProgLED_MAX(rgb[0], rgb[1], rgb[2]);
	min = ProgLED_MIN(rgb[0], rgb[1], rgb[2]);
	delta = max - min;

   if (max == min) out[0] = 0.00;
   else if (max == rgb[0]) out[0] = fmod((60.00 * ((rgb[1] - rgb[2]) / delta) + 360.00), 360.00);
   else if (max == rgb[1]) out[0] = fmod((60.00 * ((rgb[2] - rgb[0]) / delta) + 120.00), 360.00);
   else if (max == rgb[2]) out[0] = fmod((60.00 * ((rgb[0] - rgb[1]) / delta) + 240.00), 360.00);

	if (!max) out[1] = 0.00;
		else out[1] = (delta / max) * 100.00;

	out[2] = max * 100.00;
}

template<ledIdx_t ledNum>
void ProgLED<ledNum>::hsv2RGB(float (&in)[3], uint8_t (&out)[3])
{
 	float temp[3];  
	temp[0] = in[0];
    temp[1] = in[1] / 100.00;
	temp[2] = in[2] / 100.00;

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

		if (temp[0] == 360.00) temp[0] = 0.00;
		    else temp[0] /= 60.00;

		i = (int32_t)trunc(temp[0]);
		f = temp[0] - i;

		p = temp[2] * (1.00 - temp[1]);
		q = temp[2] * (1.00 - (temp[1] * f));
		t = temp[2] * (1.00 - (temp[1] * (1.00 - f)));

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


// ----- LED METHODS DEFINITIONS
void LED::rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t on = 1)
{
	// Write new RGB color values
	color[getChannelIdx(RED_IDX)] = r;
	color[getChannelIdx(GREEN_IDX)] = g;
	color[2] = b; // SOON: Since blue is always 2(for current supported color formats)

	// Make sure maximum value of on param is 1
	if (on) on = 1;

	// Write new status bit keeping old brightness bits
	config |= (on << PROG_LED_STATUS_BIT);

	// Calculate new RGB values
	adjustColor();
}

void LED::rgb(uint32_t color, uint8_t on = 1)
{
	// Extract RGB bytes from 32-bit color value and pass params to main RGB method
	rgb((color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, color & 0x000000FF, on);
}

void LED::brightness(uint8_t value)
{
	// Make sure 100 is maximum value
	value &= 0b01111111;

	// If value is not changed, abort
	if (value == (config & PROG_LED_STATUS_MASK)) return;

	// Write new brightness value keeping old status bit
	config |= value;

	// Calculate new RGB values
	adjustColor();
}

inline void LED::on(void)
{
	SSTD_BIT_SET(config, PROG_LED_STATUS_BIT);
}

inline void LED::off(void)
{
	SSTD_BIT_CLEAR(config, PROG_LED_STATUS_BIT);
}

inline void LED::toggle(void)
{
	SSTD_BIT_TOGGLE(config, PROG_LED_STATUS_BIT);
}

inline uint8_t LED::getColor(uint8_t idx) const
{
	return outputColor[idx];
}

inline uint8_t LED::getConfig(void) const
{
	return config;
}

void LED::reset(void)
{
	// Reset color values to black
	color[0] = 0x00;
	color[1] = 0x00;
	color[2] = 0x00;

	// Reset output color values to black
	outputColor[0] = 0x00;
	outputColor[1] = 0x00;
	outputColor[2] = 0x00;

	// Set default config value
	config = (1 << PROG_LED_STATUS_BIT) | (100 << PROG_LED_BRGHT_BIT);
}

inline void LED::setChannelBitmap(uint8_t bitmap)
{
	chBitmap = bitmap;
}


void LED::adjustColor(void)
{
	// Extract brightness bits
	uint8_t brightness = config & PROG_LED_BRGHT_MASK;

	// Scale RGB values using brightness
	outputColor[0] = color[0] * (brightness * 10 / 100) / 10;
	outputColor[1] = color[1] * (brightness * 10 / 100) / 10;
	outputColor[2] = color[2] * (brightness * 10 / 100) / 10;
}

uint8_t LED::getChannelIdx(ProgLED_chIdx_t idx)
{
	return (chBitmap & (3 << idx)) >> idx;
}


// END WITH NEW LINE

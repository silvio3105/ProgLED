
// ----- INCLUDE FILES
#include			"ProgLED.h"

#if USE_FPU == 1
#include			<math.h>
#endif // USE_FPU


// ----- MACRO FUNCTIONS
#define ProgLED_MAP(_in, _inMin, _inMax, _outMin, _outMax) \
	(_in - _inMin) * (_outMax - _outMin) / (_inMax - _inMin) + _outMin

#define ProgLED_MIN(_in1, _in2, _in3) \
	(_in1 < _in2) ? (_in1 < _in3 ? _in1 : _in3) : (_in2 < _in3 ? _in2 : _in3)

#define ProgLED_MAX(_in1, _in2, _in3) \
	(_in1 > _in2)? (_in1 > _in3 ? _in1 : _in3) : (_in2 > _in3 ? _in2 : _in3)


// ----- ProgLED FUNCTION DEFINITIONS
ProgLED::ProgLED(extHandler start, extHandler stop, uint16_t ledsNum, ProgLED_format_t ledFormat = PROG_LED_GRB)
{
	// Set external handlers
	startHandler = start;
	stopHandler = stop;

	// Allocate memory for ledsNum LEDs if ledsNum > 0. Memory allocation is checked in init() method
	if (ledsNum)
	{
		led = new Led[ledsNum];
		ledCount = ledsNum;
	}
	else
	{
		ledCount = 0;
		led = nullptr;
	}

	// Set LED line color format
	if (ledFormat == PROG_LED_GRB)
	{
		rIdx = 1;
		gIdx = 0;
		bIdx = 2;
	}
	else if (ledFormat == PROG_LED_RGB)
	{
		rIdx = 0;
		gIdx = 1;
		bIdx = 2;
	}
}

ProgLED::~ProgLED(void)
{
	// Set external handlers to nullptr
	startHandler = stopHandler = nullptr;

	// Remove LEDs from heap
	delete [] led;
	led = nullptr;
}


uint8_t ProgLED::init(void)
{
	// Return NOK status if handlers are nullptr ot memory for leds is not allocated
	if (!startHandler || !stopHandler || !led) return PROG_LED_NOK;

	// Set all LEDs to default values
	reset();

	return PROG_LED_OK;
}

void ProgLED::reset(void)
{
	for (uint16_t i = 0; i < ledCount; i++)
	{
		// Set LED color and status
		led[i].rgb(0, 0, 0);

		// Set brightness
		led[i].brightness(100);
	}
}

void ProgLED::toggle(void)
{
	for (uint16_t i = 0; i < ledCount; i++) led[i].toggle();
}

void ProgLED::on(void)
{
	for (uint16_t i = 0; i < ledCount; i++) led[i].on();
}

void ProgLED::off(void)
{
	for (uint16_t i = 0; i < ledCount; i++) led[i].off();
}

void ProgLED::update(void)
{
	uint8_t bit = 0;
	ledIdx = ledCount - 1;
	ledByte = ledBit = 0;

	// Fetch first bit in LED data
	fetchBit(bit);

	// Call external handler
	startHandler(bit);	
}

inline void ProgLED::stop(void)
{
	// Just pass zero
	stopHandler(0);
}

uint8_t ProgLED::fetchBit(uint8_t& bit)
{
	uint8_t status = led[ledIdx].getStatus() & PROG_LED_STATUS_MASK;
	bit = led[ledIdx].getColor(ledByte) & (1 << ledBit);

	if (bit) bit = 1;

	ledBit++;
	if (ledBit == 8)
	{
		ledBit = 0;
		ledByte++;

		if (ledByte == 3)
		{
			if (!ledIdx) return PROG_LED_OK;
			else
			{
				ledIdx--;
				return PROG_LED_CONTINUE;
			}
		}
	}

	return PROG_LED_CONTINUE;
}

void ProgLED::brightness(uint8_t value)
{
	for (uint16_t i = 0; i < ledCount; i++) led[i].brightness(value);
}


#if USE_FPU == 0
#warning "Functions rgb2HSV and hsv2RGB do not work without FPU!"
void ProgLED::rgb2HSV(uint8_t (&in)[3], int32_t (&out)[3])
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

void ProgLED::hsv2RGB(int32_t (&in)[3], uint8_t (&out)[3])
{

}
#else
void ProgLED::rgb2HSV(uint8_t (&in)[3], float (&out)[3])
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

void ProgLED::hsv2RGB(float (&in)[3], uint8_t (&out)[3])
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


// ----- Led FUNCTION DEFINITIONS
void Led::rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t on = 1)
{
	// Write new RGB color values
	color[ProgLED::rIdx] = r;
	color[ProgLED::gIdx] = g;
	color[ProgLED::bIdx] = b;

	// Make sure maximum value of on param is 1
	if (on) on = 1;

	// Write new status bit keeping old brightness bits
	config |= (on << PROG_LED_STATUS_BIT);

	// Calculate new RGB values
	adjustColor();
}

inline void Led::rgb(uint32_t color, uint8_t on = 1)
{
	// Extract RGB bytes from 32-bit color value
	rgb((color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, color & 0x000000FF, on);
}

void Led::brightness(uint8_t value)
{
	// Make sure 100 is maximum value
	if (value > 100) value = 100;

	// If value is not changed, abort
	if (value == (config & PROG_LED_STATUS_MASK)) return;

	// Write new brightness value keeping old status bit
	config |= value;

	// Calculate new RGB values
	adjustColor();
}

void Led::adjustColor(void)
{
	// Extract brightness bits
	uint8_t brightness = config & PROG_LED_BRGHT_MASK;

	// Scale RGB values using brightness
	outputColor[0] = color[0] * (brightness / 100);
	outputColor[1] = color[1] * (brightness / 100);
	outputColor[2] = color[2] * (brightness / 100);
}


// END WITH NEW LINE

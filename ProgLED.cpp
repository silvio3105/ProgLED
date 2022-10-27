
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


// FUNCTION DEFINITIONS

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

// END WITH NEW LINE

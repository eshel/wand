#ifndef _BREATH_H_
#define _BREATH_H_

#include "Animation.h"
#include "NeoPixelParallel.h"

// 10 seconds per animation cycle
#define BREATH_LENGTH 10000

class Breath : public Animation {
public:
	Breath(MultiNeoPixel& strip, uint32_t color, uint16_t firstRow, uint16_t len, bool active) : 
		Animation(strip, active),
		mColor(color),
		mFirstRow(firstRow),
		mLength(len) {
		mIsRainbow = false;
		mTimeDividerMicros = 20000;
	}

	virtual void begin() {

	}

	virtual void clear() {

	}

	float interpolate(float t) {
		return t*t*t; 
	}

	float animationCurve(float t) {
		// symmetry around 0.5
		float position = abs(0.5 - t)*2;
		return interpolate(position);
	}


protected:
	virtual void performDraw() {
		uint32_t clr;
		if (mIsRainbow == true) {
			clr = Wheel((uint8_t)((getTime() / mTimeDividerMicros) % 256));
		} else {
			clr = mColor;
		}

		// t is between 0..1, 0 being the start of the animation
		float t = (millis() % BREATH_LENGTH) / (float)BREATH_LENGTH;
		//uint8_t scale = 256*animationCurve(t);
		//uint8_t b = (((uint16_t)mColor & 255)*scale) >> 8;
		//uint8_t g = ((((uint16_t)mColor >> 8) & 255)*scale) >> 8;
		//uint8_t r = (((uint16_t)(mColor >> 16))*scale) >> 8;
		float scale = animationCurve(t);
		//uint8_t partOf256 = (uint8_t)(scale * 256);
		//clr = brightness(clr, partOf256);

		uint8_t b = (uint8_t)(getB(clr)*scale);
		uint8_t g = (uint8_t)(getG(clr)*scale);
		uint8_t r = (uint8_t)(getR(clr)*scale);
		for (uint16_t y = 0; y < mLength; ++y) {
			for (uint16_t x = 0; x < mStrip.getSizeX(); ++x) {
				mStrip.setPixelColor(x, mFirstRow + y, r, g, b);				
				//mStrip.setPixelColor(x, mFirstRow + y, clr);
			}
		}
	}

private:
	uint16_t mFirstRow;
	uint16_t mLength;
	uint32_t mColor;
	long mTimeDividerMicros;
	bool mIsRainbow;
};

#endif
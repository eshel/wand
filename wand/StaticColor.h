#ifndef _STATICCOLOR_H_
#define _STATICCOLOR_H_

#include "Animation.h"
#include "NeoPixelParallel.h"

class StaticColor : public Animation {
public:
	StaticColor(MultiNeoPixel& strip, uint32_t color, uint16_t firstRow, uint16_t len, bool active) : 
		Animation(strip, active),
		mColor(color),
		mFirstRow(firstRow),
		mLength(len) {
	}

	virtual void begin() {

	}

	virtual void clear() {

	}

protected:
	virtual void performDraw() {
		for (uint16_t y = 0; y < mLength; ++y) {
			for (uint16_t x = 0; x < mStrip.getSizeX(); ++x) {
				mStrip.setPixelColor(x, mFirstRow + y, mColor);				
			}
		}
	}

private:
	uint16_t mFirstRow;
	uint16_t mLength;
	uint32_t mColor;
};

#endif
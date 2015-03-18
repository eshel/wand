#ifndef _CYLON_H_
#define _CYLON_H_

#include "Animation.h"
#include "ColorUtils.h"

class Cylon : public Animation {
public:
	Cylon(MultiNeoPixel& strip, bool active) : Animation(strip, active) {
	}

	virtual void begin() {

	}

	virtual void clear() {

	}

protected:
	virtual void performDraw() {
		static uint16_t y = 0;

		for (uint16_t x = 0; x < mStrip.getSizeX(); x++) {
			mStrip.setPixelColor(x, y, Wheel((3 * getFrameCount()) % 768));
		}

		y = (y + 1) % mStrip.getSizeY();
	}

};


#endif // #ifndef _CYLON_H_

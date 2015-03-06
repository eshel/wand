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
		static uint8_t dir = 0;

		for (uint16_t x = 0; x < mStrip.getSizeX(); x++) {
			mStrip.setPixelColor(x, y, 255, 0, 0);
		}

		if (dir = 0) {
			if (y < mStrip.getSizeY() - 1) {
				y++;
			} else {
				dir = 1;
			}
		} else {
			if (y > 0) {
				y--;
			} else {
				dir = 0;
			}
		}
	}

};


#endif // #ifndef _CYLON_H_

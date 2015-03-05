#ifndef _RAIN_H_
#define _RAIN_H_

#include "Animation.h"
#include "ColorUtils.h"

class Rain : public Animation {
public:
	Rain(MultiNeoPixel& strip, bool active) : Animation(strip, active) {
	}

	virtual void begin() {

	}

	virtual void clear() {

	}

protected:
	virtual void performDraw() {
		static uint16_t pixelIndex = 0;
		uint16_t maxPixel = mStrip.getNumAddresses();

		uint32_t c = Wheel(getFrameCount() % 768);
		mStrip.setPixelColor(pixelIndex, c);

		pixelIndex++;
		if (pixelIndex >= maxPixel) {
			pixelIndex = 0;
		}
	}

};


#endif // #ifndef _RAIN_H_

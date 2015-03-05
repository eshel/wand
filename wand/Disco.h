#ifndef _DISCO_H_
#define _DISCO_H_

#include "Animation.h"

class Disco : public Animation {
public:
	Disco(MultiNeoPixel& strip, bool active) : Animation(strip, active) {
	}

	virtual void begin() {

	}

	virtual void clear() {

	}

	void addRandomBlips(int8_t minPixels, int8_t maxPixels) {
		int8_t newPixels = random(minPixels, maxPixels);
		if (newPixels < 0) {
			newPixels = 0;
		}
		for (uint8_t i=0; i<(uint8_t)newPixels; i++) {
			addOneRandomBlip();
		}
	}

	void addOneRandomBlip() {
		uint16_t maxPixel = mStrip.getNumAddresses();
		uint16_t colorOffset = random(0, (256*3));
		uint32_t color = Wheel(colorOffset);
		uint16_t pixelIndex = random(0, maxPixel);

		mStrip.setPixelColor(pixelIndex, color);
	}  


protected:
	virtual void performDraw() {
		addRandomBlips(-15, 5);
	}



};

#endif // #ifndef _DISCO_H_


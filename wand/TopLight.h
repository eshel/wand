#ifndef _TOP_LIGHT_H_
#define _TOP_LIGHT_H__

#include "NeoPixelParallel.h"
#include "Button.h"

class TopLight : public Animation {
public:
	TopLight(MultiNeoPixel& strip, Button& btn, uint32_t clr, uint16_t height, bool enabled) : 
			Animation(strip, enabled), mButton(btn), mColor(clr), mHeight(height) {
		
	}

	virtual void performDraw() {
		if (mButton.isPressed()) {
			for (uint16_t y = mStrip.getSizeY() - mHeight; y < mStrip.getSizeY(); y++) {
				for (uint16_t x = 0; x < mStrip.getSizeX(); x++) {
					mStrip.setPixelColor(x, y, mColor);
				}
			}
		}
	}

	virtual void begin() {

	}	

	virtual void clear() {

	}

private:
	Button& mButton;
	uint32_t mColor;
	uint16_t mHeight;
};

#endif // #ifndef _TOP_LIGHT_H_
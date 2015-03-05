#ifndef _MODE_INDICATOR_H_
#define _MODE_INDICATOR_H_

#include "NeoPixelParallel.h"
#include "ColorUtils.h"

class ModeIndicator : public Animation {
public:
	ModeIndicator(MultiNeoPixel& strip, volatile uint8_t* mode, bool active, uint8_t firstRow, uint8_t lastRow) : Animation(strip, active), mMode(mode) {
		mFirstRow = firstRow;
		mLastRow = lastRow;

		uint8_t rowsNum = mLastRow - mFirstRow + 1;

		mLastMode = *mMode;
		mLastChange = 0;
		if (rowsNum == 0) {
			mHueInterval = 256;
		} else {
			mHueInterval = 768 / (uint16_t)rowsNum;
		}
		mTimeout = 1000;
		mLuminosity = 255;
	}

	void clear() {
		mLastChange = 0;
	}

	void begin() {

	}

	void forceChange() {
		mLastChange = millis();
	}

	void checkMode() {
		uint8_t mode = *mMode;
		if (mode != mLastMode) {
			mLastChange = millis();
			mLastMode = mode;
		}
	}

	bool shouldDraw() {
		checkMode();
		if ((millis() - mLastChange) <= mTimeout) {
			return true;
		} else {
			return false;
		}
	}

	uint8_t getFirstRow() {
		return mFirstRow;
	}

	uint8_t getLastRow() {
		return mLastRow;
	}

	virtual void performDraw() {
		if (shouldDraw()) {
			uint8_t mode = mLastMode;
			uint32_t color = Wheel(((uint32_t)mode * (uint32_t)mHueInterval) % 768, mLuminosity);
			uint8_t visibleRow = getFirstRow() + mode % (getLastRow() - getFirstRow() + 1);
			for (uint8_t y = getFirstRow(); y <= getLastRow(); ++y) {
				if (y == visibleRow) {
					for (uint8_t x=0; x<mStrip.getSizeX(); x++) {
						mStrip.setPixelColor(x, y, color);
					}
				} else {
					for (uint8_t x=0; x<mStrip.getSizeX(); x++) {
						mStrip.setPixelColor(x, y, 0, 0, 0);
					}					
				}
			}
		}
	}

protected:
	volatile uint8_t* mMode;
	uint8_t mLastMode;
	uint32_t mTimeout;
	uint32_t mLastChange;
	uint16_t mHueInterval;
	uint8_t mFirstRow;
	uint8_t mLastRow;
	uint8_t mLuminosity;
};

#endif // #ifndef _MODE_INDICATOR_H_
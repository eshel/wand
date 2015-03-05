#ifndef _METER_H_
#define _METER_H_

#include "Animation.h"
#include "ColorUtils.h"
#include "Motion.h"

class Meter : public Animation {
public:
	Meter(MultiNeoPixel& strip, Motion& motion, bool active) : Animation(strip, active), mMotion(motion) {
		mCurrentPower = 0;
		mCurrentX = 0;
		setIsLog(true);
		setPowerRange(1200, 5000);
	}

	void setIsLog(bool isLog) {
		mIsLog = isLog;
	}

	void setPowerRange(uint16_t minPower, uint16_t maxPower) {
		mMinPower = minPower;
		mMaxPower = maxPower;
	}

	virtual void begin() {
		mCurrentX = mStrip.getSizeX() - 1;
	}

	virtual void clear() {

	}

	int16_t filter(int16_t value) {
		if (value < 0) {
			value = -value;
		}
		return value;
	}

	virtual void update() {
		int32_t v0 = mMotion.getSample(0).apower;
		int32_t v1 = mMotion.getSample(1).apower;
		int32_t v2 = mMotion.getSample(2).apower;

		int16_t value = (int16_t)((v0 * 4 + v1 * 2 + v2)) / 7;
		value = filter(value);
		mCurrentPower = value;
	}

	uint32_t getColor(uint8_t yVal) {
		uint16_t part = ((uint16_t)yVal * 768) / mStrip.getSizeY();
		return Wheel(part);
	}

	uint16_t calcLevel(uint16_t currentPower) {
		int16_t lvl;
		if (mIsLog) {
			float minLog = log((float)mMinPower);
			float maxLog = log((float)mMaxPower);
			float currentLog = log((float)currentPower); 
			float powerStep = ((maxLog - minLog) / (float)mStrip.getSizeY());
			float relativeLog = currentLog - minLog;
			lvl = (int16_t)(relativeLog / powerStep);
		} else {
			int16_t relativePower = mCurrentPower - mMinPower;			
			int16_t powerStep = (mMaxPower - mMinPower) / mStrip.getSizeY();
			lvl = relativePower / powerStep;
		}
		if (lvl > mStrip.getSizeY() - 1) {
			lvl = mStrip.getSizeY() - 1;
		} else if (lvl < 0) {
			lvl = 0;
		}

		return (uint16_t)lvl;
	}

protected:
	bool shouldDrawX(uint8_t x) {
		if (x == mCurrentX) {
			return true;
		} else {
			return false;
		}
	}

	virtual void performDraw() {
		update();
		uint16_t xsize = mStrip.getSizeX();
		mCurrentX++;
		if (mCurrentX == xsize) {
			mCurrentX = 0;
		}

		uint16_t powerStep = ((mMaxPower - mMinPower) / mStrip.getSizeY());
		int8_t ysize = calcLevel(mCurrentPower);

		for (uint8_t x = 0; x < xsize; x++) {
			if (shouldDrawX(x)) {
				for (uint8_t y = 0; y < ysize; y++) {
					mStrip.setPixelColor(x, (ysize-1)-y, getColor(y));			
				}
			}
		}
	}	

private:
	Motion& mMotion;
	int16_t mCurrentPower;
	uint16_t mMinPower;
	uint16_t mMaxPower;
	float mLogMinPower;
	float mLogMaxPower;
	bool mIsLog;
	uint16_t mCurrentX;
};

#endif // #ifndef _METER_H_
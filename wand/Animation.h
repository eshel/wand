#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "NeoPixelParallel.h"

class Animation {
public:
	Animation(MultiNeoPixel& strip, bool active = false) : mStrip(strip) {
		mIsActive = active;
		mFrameCount = 0;
		mTime = 0;
	}

	void setActive(bool active) {
		mIsActive = active;
	}

	bool isActive() const {
		return mIsActive;
	}

	void draw() {
		mTime = micros();
		performDraw();
		mFrameCount++;
	}

	inline uint32_t getFrameCount() const {
		return mFrameCount;
	}

	virtual void begin() = 0;
	virtual void clear() = 0;

	unsigned long getTime() const {
		return mTime;
	}

protected:
	virtual void performDraw() = 0;

protected:
	MultiNeoPixel& mStrip;
	uint32_t mFrameCount;
	unsigned long mTime;
	bool mIsActive;

private:
};

#endif // #ifndef _ANIMATON_H_


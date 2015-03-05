#ifndef _BOOM_H_
#define _BOOM_H_

class Boom : public Animation {
public:
	Boom(MultiNeoPixel& strip, bool enabled) : Animation(strip, enabled) {
		mCX = mCY = 0;
		mRadius = 0.0f;
		mSpeed = 150.0f;
		mMaxRadius = 100.0f;
		mInProgress = false;
		mPeriod = 0;
		mColor1Hue = 0;
		mColor2Hue = 60;
		mBeginTime = 0;		
	}

	void setPeriod(uint16_t period) {
		mPeriod = period;
	}

	virtual void begin() {
		clear();
	}

	virtual void clear() {
		mInProgress = false;
		mRadius = 0.0f;
		mBeginTime = 0;		
	}

	bool inProgress() const {
		return mInProgress;
	}

	bool shouldExpand() {
		if (mPeriod == 0) {
			return false;
		}
		return (getFrameCount() % mPeriod) == 0;
	}

	void randomCenter() {
		mCX = randomX();
		mCY = randomY();
	}

	void setSpeed(float speed) {
		mSpeed = speed;
	}

	void setDuration(uint32_t durationMs) {
		mSpeed = (mMaxRadius * 1000.0f) / (float)durationMs;
	}

	void setMaxRadius(float maxRadius) {
		mMaxRadius = maxRadius;
	}

	void set(float maxRadius, uint32_t durationMs) {
		setMaxRadius(maxRadius);
		setDuration(durationMs);
	}

	void expand() {
		mInProgress = true;

		mRadius = mSpeed * (float)(millis() - mBeginTime) / 1000.0f;

		int8_t dx = (int8_t)((int16_t)mRadius / (int16_t)SPACE_X_MM);
		int8_t dy = (int8_t)((int16_t)mRadius / (int16_t)SPACE_Y_MM);
		int8_t xmin = mStrip.clampX(mCX - dx);
		int8_t xmax = mStrip.clampX(mCX + dx);
		int8_t ymin = mStrip.clampY(mCY - dy);
		int8_t ymax = mStrip.clampY(mCY + dy);

		for (int8_t x = xmin; x <= xmax; x++) {
			for (int8_t y = ymin; y <= ymax; y++) {
				float xsqr = (float)abs(mCX - x) * (float)SPACE_X_MM;
				float ysqr = (float)abs(mCY - y) * (float)SPACE_Y_MM;
				float distance = sqrt(xsqr*xsqr + ysqr*ysqr);
				if (distance <= mRadius) {
					uint8_t byteDistance = (uint8_t)(((distance / mRadius) * (float)(abs(mColor2Hue - mColor1Hue))) / 255.0f);
					uint32_t color = Wheel(byteDistance);
					color = brightness(color, (uint8_t)((distance / mRadius) * 255.0f));
					mStrip.setPixelColor(x, y, color);
				}
			}
		}

		if (mRadius >= mMaxRadius) {
			mInProgress = false;
		}
	}

	void beginExpand() {
		mBeginTime = millis();
		randomCenter();
		randomColors();
		mRadius = 0.0f;
		mInProgress = true;
	}

	int8_t randomX() {
		return random(0, mStrip.getSizeX());
	}

	int8_t randomY() {
		return random(1, mStrip.getSizeY()-1);
	}

	void randomColors() {
		mColor1Hue = random(0, 768);
		mColor2Hue = random(0, 768);
	}

protected:
	virtual void performDraw() {
		if (mInProgress) {
			expand();
		} else {
			if (shouldExpand()) {
				beginExpand();
			}
		}
	}

private:
	int8_t mCX;
	int8_t mCY;
	float mRadius;
	float mMaxRadius;
	float mSpeed;
	uint16_t mPeriod;
	bool mInProgress;
	uint16_t mColor1Hue;
	uint16_t mColor2Hue;
	unsigned long mBeginTime;
};

#endif // #ifndef _BOOM_H_

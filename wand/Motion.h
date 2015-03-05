#ifndef _MOTION_H_
#define _MOTION_H_

#include "MPU6050.h"
#include "MotionSample.h"

#define WINDOW_SIZE		16

class Motion {
public:
	Motion() : mSensor(0x68), mWindowSize(WINDOW_SIZE) {
		clearBuffer();
	}

	~Motion() {

	}

	void clearBuffer() {
		mWindowHead = mWindowSize - 1;
		mSamplesCollected = 0;
	}

	void print() const {
		Serial.print("samples=");
		Serial.print(mSamplesCollected);
		Serial.print(", head=");
		Serial.println(mWindowHead);
	}

	void sample() {
		int16_t ax, ay, az, gx, gy, gz;
		mSensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

		// 1G = 1024 (when in 8G mode)
		ax >>= 2;
		ay >>= 2;
		az >>= 2;

		if (mSamplesCollected < mWindowSize) {
			mSamplesCollected++;
		}
		mWindowHead = (mWindowHead + 1) % mWindowSize;		

		mSamples[mWindowHead].set(ax, ay, az, gx, gy, gz);
	}

	const uint16_t getWindowSize() const {
		return mWindowSize;
	}

	const uint16_t getSamplesCount() const {
		return mSamplesCollected;
	}

	const MotionSample& getSample(uint16_t offset = 0) const {
		if (offset > mSamplesCollected) {
			offset = mSamplesCollected;
		}

		uint16_t sampleIndex = mWindowHead;
		if (sampleIndex >= offset) {
			sampleIndex = sampleIndex - offset;
		} else {
			sampleIndex = mWindowSize + sampleIndex - offset;
		}

		return mSamples[sampleIndex];
	}

	void begin() {
		clearBuffer();
  		mSensor.initialize();
  		mSensor.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
	}

	bool test() {
		return mSensor.testConnection() ? true : false;
	}

private:
	MPU6050 mSensor;

	MotionSample mSamples[WINDOW_SIZE];

	const uint16_t mWindowSize;
	uint16_t mWindowHead;
	uint16_t mSamplesCollected;
};

#endif // #ifndef _MOTION_H_


#ifndef _MOTION_SAMPLE_H_
#define _MOTION_SAMPLE_H_

class MotionSample {
public:
	MotionSample() {
		ax = ay = az = 0;
		gx = gy = gz = 0;
		apower = 0;
		alog = 0.0f;
		t = 0;
	}

public:
	void set(const MotionSample& other) {
		ax = other.ax;
		ay = other.ay;
		az = other.az;
		gx = other.gx;
		gy = other.gy;
		gz = other.gz;
		apower = other.apower;
		alog = other.alog;
	}

	void add(const MotionSample& other) {
		ax += other.ax;
		ay += other.ay;
		az += other.az;
		gx += other.gx;
		gy += other.gy;
		gz += other.gz;
		apower += other.apower;
		alog += other.alog;		
	}

	void set(int16_t _ax, int16_t _ay, int16_t _az, int16_t _gx, int16_t _gy, int16_t _gz) {
		t = millis();

		ax = _ax;
		ay = _ay;
		az = _az;
		gx = _gx;
		gy = _gy;
		gz = _gz;

		float fx = (float)_ax;
		float fy = (float)_ay;
		float fz = (float)_az;

		apower = (int16_t)sqrt(fx*fx + fy*fy + fz*fz);
		alog = log(apower);
	}

	// Sets this sample's values to be s2 - s1
	void setDiff(const MotionSample& s1, const MotionSample& s2) {
		t = s2.t - s1.t;
		ax = s2.ax - s1.ax;
		ay = s2.ay - s1.ay;
		az = s2.az - s1.az;
		gx = s2.gx - s1.gx;
		gy = s2.gy - s1.gy;
		gz = s2.gz - s1.gz;
		apower = s2.apower - s1.apower;
		alog = s2.alog - s1.alog;
	}

	void print() const {
		// display tab-separated accel/gyro x/y/z values
		Serial.print("a/g:\t");
		Serial.print(ax); Serial.print('\t');
		Serial.print(ay); Serial.print('\t');
		Serial.print(az); Serial.print('\t');
		Serial.print(apower); Serial.print('\t');
		Serial.print(gx); Serial.print('\t');
		Serial.print(gy); Serial.print('\t');
		Serial.print(gz); Serial.print('\t');
		Serial.println();
	}

private:
	// private so it won't be accessed - we don't want copy constructors
	MotionSample(const MotionSample& other) {}

public:
	// acceleration values: 1g = 1024
	unsigned long t;
	float alog;
	int16_t ax, ay, az;
	int16_t gx, gy, gz;
	int16_t apower;
};

#endif // #ifndef _MOTION_SAMPLE_H_
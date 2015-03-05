#ifndef _LED_H_
#define _LED_H_

class Led {
public:
	inline Led(uint8_t pin) {
		mPin = pin;
		mState = false;
	}

	inline void begin() {
		pinMode(mPin, OUTPUT);
		off();
	}

	inline void invert() {
		set(!mState);
	}

	inline void on() {
		set(true);
	}

	inline void off() {
		set(false);
	}

	inline void set(bool enabled) {
		mState = enabled;
		digitalWrite(mPin, !mState);
	}

	inline bool get() {
		return mState;
	}

private:
	bool mState;
	uint8_t mPin;
};

#endif // #ifndef _LED_H_
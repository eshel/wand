#ifndef _BUTTON_H_
#define _BUTTON_H_

class Button {
public:
	Button(int pin) : mPin(pin) {
		mPrevIsPressed = false;
		mIsPressed = false;
	}

	void begin() {
		pinMode(mPin, INPUT_PULLUP);
	}

	void read() {
		mPrevIsPressed = mIsPressed;
		mIsPressed = (LOW == digitalRead(mPin));
	}

	bool isPressed() const {
		return mIsPressed;
	}

	bool shouldHandleOn() const {
		return mIsPressed && (!mPrevIsPressed);
	}

	bool shouldHandleOff() const {
		return (!mIsPressed) && mPrevIsPressed;
	}

private:
	const int mPin;
	bool mIsPressed;
	bool mPrevIsPressed;
};

#endif // #ifndef _BUTTON_H_


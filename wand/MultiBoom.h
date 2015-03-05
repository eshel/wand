#ifndef _MULTI_BOOM_H_
#define _MULTI_BOOM_H_

#include "Boom.h"
#include "NeoPixelParallel.h"


class MultiBoom : public Animation {
public:
	MultiBoom(MultiNeoPixel& strip, bool enabled) : 
			Animation(strip, enabled), 
			mBoom1(strip, true), mBoom2(strip, true), 
			mBoom3(strip, true), mBoom4(strip, true) {

	}

	virtual void performDraw() {
		mBoom1.draw();
		mBoom2.draw();
		mBoom3.draw();
		mBoom4.draw();
	}

	virtual void begin() {
		mBoom1.begin();
		mBoom2.begin();
		mBoom3.begin();
		mBoom4.begin();
	}	

	virtual void clear() {
		mBoom1.clear();
		mBoom2.clear();
		mBoom3.clear();
		mBoom4.clear();
	}

	void explodeOne(float maxRadius = 80.0f, uint32_t durationMs = 250) {
		Boom* boom = findReadyBoom();
		if (boom != 0) {
		    boom->set(maxRadius, durationMs);
		    boom->beginExpand();			
		}
	}

	Boom* findReadyBoom() {
		if (!mBoom1.inProgress()) {
			return &mBoom1;
		} else if (!mBoom2.inProgress()) {
			return &mBoom2;
		} else if (!mBoom3.inProgress()) {
			return &mBoom3;
		} else if (!mBoom4.inProgress()) {
			return &mBoom4;
		} else {
			return 0;
		}
	}

private:
	Boom mBoom1;
	Boom mBoom2;
	Boom mBoom3;
	Boom mBoom4;
};

#endif // #ifndef _MULTI_BOOM_H_
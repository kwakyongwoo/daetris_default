#pragma once

#include "ofMain.h"

#include "Constant.hpp"
#include "LowerHalf.hpp"
#include "Serial.hpp"
#include "UpperHalf.hpp"

class ofApp : public ofBaseApp
{
public:
	ofApp(std::string port) :
		serial(port, 9600),
		lower_half(0, Constant::HALF, Constant::WIDTH, Constant::HALF),
		upper_half(0, 0, Constant::WIDTH, Constant::HALF)
	{
		serial.set_callback([this](int) { trigger(); });
		upper_half.set_lower_trigger([this] { lower_half.update(); });
	}

	void setup() {
		serial.setup();
	}

	void update() {
		serial.update();
	}

	void draw() {
		upper_half.draw();
		lower_half.draw();
	}

	void mouseReleased(int x, int y, int button) {
		if (Constant::DEBUG) {
			trigger();
		}
	}

	void trigger() {
		upper_half.update();
		// lower_half.update();
	}

private:
	LibDaetris::SerialApp serial;

	LowerHalf lower_half;
	UpperHalf upper_half;
};

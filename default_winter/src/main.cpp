#include "ofMain.h"

#include "ofApp.hpp"
#include "Constant.hpp"

#include <iostream>

//========================================================================
int main() {
	ofWindowMode mode = Constant::FULL_SCREEN ? OF_FULLSCREEN : OF_WINDOW;
	if (Constant::DEBUG) {
		ofSetupOpenGL(Constant::WIDTH, Constant::HEIGHT, mode);            // <-------- setup the GL context
	}
	else {
		ofSetupOpenGL(Constant::HEIGHT, Constant::WIDTH, mode);
	}

	std::string port;

	std::cout << "port name : ";
	std::cin >> port;

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp(port));

	return 0;
}

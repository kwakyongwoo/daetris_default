#ifndef UPPER_HALF_HPP
#define UPPER_HALF_HPP

#include "ofMain.h"
#include "Constant.hpp"

#include <chrono>
#include <thread>
#include <Windows.h>
#include <ctime>

class UpperHalf {
public:
	UpperHalf(size_t x, size_t y, size_t width, size_t height) :
		x(x), y(y), width(width), height(height),
		bg(0), bag(0),
		taged(false),
		lion1(0), lion2(1), thing_y(x)
	{
		srand((unsigned int)time(0));
		size_t i = 0;
		// background
		for (std::string filenum : { "1", "2" }) {
			background[i++].load("top/BACKGROUND" + filenum + ".png");
		}

		i = 0;
		// bag
		for (std::string filenum : { "1", "2" }) {
			bags[i++].load("top/BAG" + filenum + ".png");
		}

		i = 0;
		// hy_lion1
		for (std::string filenum : { "1", "2", "3" }) {
			hy_lion1[i++].load("top/HYLION1-" + filenum + ".png");
		}

		i = 0;
		// hy_lion2
		for (std::string filenum : { "1", "2", "3", "4", "5" }) {
			hy_lion2[i++].load("top/HYLION2-" + filenum + ".png");
		}

		i = 0;
		// thing
		for (std::string filenum : { "1", "2", "3", "4", "5", "6", "7" }) {
			things[i++].load("top/THING" + filenum + ".png");
		}
	}

	void set_lower_trigger(std::function<void()> const& callback) {
		lower_trigger = callback;
	}

	void draw() {
		ofPushMatrix();
		if (!Constant::DEBUG) {
			ofTranslate(0, width);
			ofRotateDeg(270);
		}

		// background
		if (duration_check(bg_start, bg_time)) {
			bg = 1 - bg;
			bg_start = std::chrono::steady_clock::now();
		}
		background[bg].draw(x, y, width, height);

		if(!taged) {
			if (duration_check(hy_lion_start, hy_lion_time)) {
				lion1 = 1 - lion1;
				hy_lion_start = std::chrono::steady_clock::now();
			}
			hy_lion1[lion1].draw(x, y, width, height);
		}
		else {
			tag();
		}

		// bag
		if (duration_check(bag_start, bag_time)) {
			bag = 1 - bag;
			bag_start = std::chrono::steady_clock::now();
		}
		bags[bag].draw(x, y, width, height);

		if (thing_show) {
			if (thing_down) {
				thing_y += 10 * Constant::RATIO;
			}
			if(thing_y < 800 * Constant::RATIO) things[thing_rand].draw(x, thing_y, width, height);
			else {
				thing_show = false;
				thing_down = false;
				thing_y = y;
				thing_done = true;
				lower_trigger();
			}
		}

		ofPopMatrix();
	}

	void update() {
		if (!taged && thing_done) {
			thing_done = false;
			taged = true;
			thing_rand = rand() % 7;
			Constant::num = thing_rand;
		}
	}

private:
	size_t x, y;
	size_t width, height;

	bool taged;
	unsigned int thing_rand;

	size_t bg;
	ofImage background[2];

	size_t bag;
	ofImage bags[2];

	size_t lion1;
	ofImage hy_lion1[3];

	size_t lion2;
	ofImage hy_lion2[5];

	ofImage things[7];
	int thing_y;

	std::function<void()> lower_trigger;

	using time_point = std::chrono::time_point<std::chrono::steady_clock>;
	time_point hy_lion_start;
	time_point bg_start;
	time_point bag_start;

	static constexpr std::chrono::milliseconds hy_lion_time = 100ms;
	static constexpr std::chrono::milliseconds bg_time = 500ms;
	static constexpr std::chrono::milliseconds bag_time = 500ms;

	template <typename T>
	bool duration_check(time_point base, T thold) {
		using namespace std::chrono;
		return duration_cast<T>(steady_clock::now() - base) > thold;
	}

	bool done1_1to2 = false;
	size_t lion = 1;
	size_t tick = 0;
	bool done2_1to2 = false;
	bool done2_1to4 = false;
	bool done2_3to1 = false;
	bool thing_show = false;
	bool thing_down = false;
	bool thing_done = true;

	void tag() {
		// HYLION1-1~1-2 3회 반복 후 0.5초 대기.
		if (!done1_1to2) {
			if (tick != 6) {
				if (duration_check(hy_lion_start, hy_lion_time)) {
					lion = 1 - lion;
					hy_lion_start = std::chrono::steady_clock::now();
					tick++;
				}
				hy_lion1[lion].draw(x, y, width, height);
			}
			else {
				done1_1to2 = true;
				tick = 0;
				ofSleepMillis(500);
			}
		}
		else {
			// HYLION2-1 -> 2-2 -> [2-3 -> 2-4] * 3 (2-3, 2-4 3회 반복) 후 0.5초 대기.
			if (!done2_1to4) {
				if (!done2_1to2) {
					if (duration_check(hy_lion_start, hy_lion_time)) {
						lion = 1 - lion;
						hy_lion_start = std::chrono::steady_clock::now();
					}
					hy_lion2[lion].draw(x, y, width, height);
					if (lion == 1) {
						done2_1to2 = true;
						thing_show = true;
					}
				}
				else {
					if (tick != 6) {
						if (duration_check(hy_lion_start, hy_lion_time)) {
							lion = 1 - lion;
							hy_lion_start = std::chrono::steady_clock::now();
							tick++;
						}
						hy_lion2[lion + 2].draw(x, y, width, height);
					}
					else {
						done2_1to4 = true;
						tick = 0;
						lion = 0;
						ofSleepMillis(500);
					}
				}
			}
			else {
				thing_down = true;
				if (tick != 8) {
					if (duration_check(hy_lion_start, hy_lion_time)) {
						lion = 1 - lion;
						hy_lion_start = std::chrono::steady_clock::now();
						tick++;
					}
					hy_lion2[lion + 3].draw(x, y, width, height);
					if (tick == 8) lion = 2;
				}
				else {
					if (!done2_3to1) {
						if (duration_check(hy_lion_start, hy_lion_time)) {
							lion--;
							hy_lion_start = std::chrono::steady_clock::now();
						}
						hy_lion2[lion].draw(x, y, width, height);
						if (lion == 0) done2_3to1 = true;
					}
					else {
						done1_1to2 = false;
						lion = 1;
						tick = 0;
						done2_1to2 = false;
						done2_1to4 = false;
						done2_3to1 = false;
						taged = false;
					}
				}
			}
		}
	}
};

#endif
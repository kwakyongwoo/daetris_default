#ifndef LOWER_HALF_HPP
#define LOWER_HALF_HPP

#include "ofMain.h"
#include "Constant.hpp"

#include <chrono>
#include <functional>
#include <thread>

#define ODD 340
#define EVEN 16

class LowerHalf {
public:
	LowerHalf(size_t x, size_t y, size_t width, size_t height) :
		x(x), y(y), width(width), height(height), trigged(false), thing_y(y - 1000 * Constant::RATIO), item_num(0),
		bg(0), hn(0)
	{
		// background
		size_t i = 0;
		for (std::string filenum : { "1", "2" }) {
			background[i++].load("bottom/BACKGROUND" + filenum + ".png");
		}

		// hy_nari
		i = 0;
		for (std::string filenum : { "1", "2" }) {
			hy_nari[i++].load("bottom/HYNARI" + filenum + ".png");
		}

		// thing
		i = 0;
		for (std::string filenum : { "1", "2", "3", "4", "5", "6", "7" }) {
			things[i++].load("top/THING" + filenum + ".png");
		}
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

		// hy_nari
		if (duration_check(hn_start, hn_time)) {
			hn = 1 - hn;
			hn_start = std::chrono::steady_clock::now();
		}
		hy_nari[hn].draw(x, y, width, height);

		if (trigged) {
			thing_y += 10 * Constant::RATIO;
			items[item_num - 1].draw(x, thing_y, width, height);
			if (thing_y > y + item_y[item_num] * Constant::RATIO) {
				trigged = false;
				thing_y = y - 1000 * Constant::RATIO;
			}
		}
		for (int i = 0; i < item_num; i++) {
			if (i == item_num - 1 && trigged) break;
			if (i % 2 == 0) items[i].draw(x + EVEN * Constant::RATIO, y + item_y[i] * Constant::RATIO, width, height);
			else items[i].draw(x + ODD * Constant::RATIO, y + item_y[i] * Constant::RATIO, width, height);
		}

		ofPopMatrix();
	}

	void update() {
		if (item_num == 9) item_num = 0;
		trigged = true;
		items[item_num] = things[Constant::num];
		item_num++;
	}

private:
	size_t x, y;
	size_t height, width;

	bool trigged;

	size_t bg;
	ofImage background[2];

	size_t hn;
	ofImage hy_nari[2];

	ofImage things[7];
	int thing_y;

	ofImage items[9];
	int item_y[9] = { 600, 400, 200, 0, -200, -400, -600, -800, -1000};
	size_t item_num;

	using time_point = std::chrono::time_point<std::chrono::steady_clock>;
	time_point bg_start;
	time_point hn_start;

	static constexpr std::chrono::milliseconds bg_time = 500ms;
	static constexpr std::chrono::milliseconds hn_time = 300ms;

	template <typename T>
	bool duration_check(time_point base, T thold) {
		using namespace std::chrono;
		return duration_cast<T>(steady_clock::now() - base) > thold;
	}
};

#endif
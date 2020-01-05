#ifndef CONSTANT_HPP
#define CONSTANT_HPP

namespace Constant
{
	constexpr double RATIO = 1;

	constexpr size_t WIDTH = 1080 * RATIO;
	constexpr size_t HEIGHT = 1920 * 2 * RATIO;

	constexpr size_t HALF = HEIGHT / 2;

	constexpr bool DEBUG = false;
	constexpr bool FULL_SCREEN = true;

	int num;
}

#endif
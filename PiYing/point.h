#pragma once

namespace piying {

	struct Point {
	public:
		Point() = default;
		Point(double x, double y) : x(x), y(y) {}

		double x{ 0.0 };
		double y{ 0.0 };
	};

}

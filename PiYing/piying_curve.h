#pragma once

#include <vector>
#include <qpointf>

class QPolygonF;

namespace piying{

	class Curve final
	{
	public:
		Curve(const QPolygonF& poly);
		Curve(double x, double y);

	public:
		QPointF get_position(int value) const noexcept { return (this->*vari_get)(value); }

	private:
		QPointF get_position_poly(int value) const noexcept;
		QPointF get_position_insert(int value) const noexcept;
		QPointF get_position_linear(int value) const noexcept;

	private:
		struct Point {
			double x{ 0.0 };
			double y{ 0.0 };
		};

	private:
		QPointF(Curve::* vari_get)(int value) const noexcept;
		const std::vector<QPointF> points;
	};

}

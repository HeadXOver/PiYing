#pragma once

#include <vector>
#include <qpointf>

class QPolygonF;

namespace piying{

	class Curve final
	{
	public:
		Curve(const QPolygonF& poly);
		Curve(const QPointF& point);

	public:
		QPointF get_position(int value) const noexcept { return (this->*vari_get)(value); }

	private:
		QPointF get_position_poly(int value) const noexcept;
		QPointF get_position_insert(int value) const noexcept;
		QPointF get_position_linear(int value) const noexcept;

	private:
		QPointF(Curve::* vari_get)(int value) const noexcept;
		const std::vector<QPointF> points;
	};

}

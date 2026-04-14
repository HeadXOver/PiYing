#pragma once

#include <vector>
#include <qpointf>

class QPolygonF;

namespace piying{

	class Curve final
	{
	public:
		Curve() {}
		Curve(const QPolygonF& poly);

	public:
		QPointF get_positon(int value) const noexcept;

	private:
		std::vector<QPointF> points;
	};

}

#pragma once

#include <qlist>
#include <vector>
#include <memory>

class QOpenGLTexture;
class PointVector;
class QPointF;
class QPolygonF;
class CtrlSlideWidget;

class Part final
{
public:
	Part(QOpenGLTexture& texture, const QList<unsigned int>& indices, bool isTexture);

	float* float_data() const;
	const unsigned int* index_data() const;

	size_t float_size() const;
	size_t index_size() const;
	size_t vertex_size() const;

	QPointF get_vert(int index, bool isSkelen) const;

	void bind_texture();
	void add_trace(int index, const QPolygonF& polygon);

	float x() const;
	float y() const;
	float height() const;
	float width() const;

	unsigned int vao_timeline() const;
	unsigned int vbo_timeline() const;
	unsigned int ebo_timeline() const;

	unsigned int vao_piying() const;
	unsigned int vbo_piying() const;
	unsigned int ebo_piying() const;

	CtrlSlideWidget* slider_widget() const;

private:
	float _x;
	float _y;
	float _height;
	float _width;

	unsigned int _vao_timeline, _vbo_timeline, _ebo_timeline;
	unsigned int _vao_piying, _vbo_piying, _ebo_piying;

private:
	QOpenGLTexture& _texture;

	std::vector<unsigned int> _indices;
	std::unique_ptr<PointVector> _vert_texture;

	CtrlSlideWidget* _sliderWidget;
};
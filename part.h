#pragma once

#include <qlist>
#include <vector>
#include <memory>
#include <map>

class QOpenGLTexture;
class PointVector;
class QPointF;
class QPolygonF;
class CtrlSlideWidget;
class CharacterTrace;
class SlideApplier;

class Part final
{
public:
	Part(QOpenGLTexture& texture, const QList<unsigned int>& indices, bool isTexture);
	~Part();

	float* float_data() const;
	const unsigned int* index_data() const;

	size_t float_size() const;
	size_t index_size() const;
	size_t vertex_size() const;

	QPointF get_vert(int index, bool isSkelen) const;

	SlideApplier& get_slide_applier();

	void bind_texture();
	void add_trace(int index, const QPolygonF& polygon);
	void apply_slide(const std::map<int, std::unique_ptr<CharacterTrace>>& traces, int value);
	void update_scale();
	void same_texture_merge(const Part& other);
	void change_slider_value(int sliderIndex, int value);

	float x() const;
	float y() const;
	float height() const;
	float width() const;

	unsigned int vao_timeline() const;
	unsigned int vao_piying() const;

private:
	float _x;
	float _y;
	float _height;
	float _width;

	unsigned int _vao_timeline;
	unsigned int _vao_piying;
	unsigned int _vbo, _ebo;

private:
	QOpenGLTexture& _texture;

	std::vector<unsigned int> _indices_origin;
	std::unique_ptr<PointVector> _vert_texture_origin;

	std::vector<unsigned int> _indices;
	std::unique_ptr<PointVector> _vert_texture;

	SlideApplier* slide_applier;
};
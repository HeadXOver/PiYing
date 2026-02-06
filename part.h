#pragma once

#include <vector>
#include <memory>
#include <qmatrix4x4>

class CharacterTexture;
class QOpenGLTexture;
class PointVector;
class QPointF;
class QPolygonF;
class CtrlSlideWidget;
class CharacterTrace;
class SlideApplier;
class Joint;

class Part final
{
public:
	Part(CharacterTexture& texture, const QList<unsigned int>& indices, bool isTexture);
	Part(const Part& part1, const Part& part2);
	Part(const Part& part);
	~Part();

	const float* float_data() const;
	const unsigned int* index_data() const;

	size_t float_size() const;
	size_t index_size() const;
	size_t vertex_size() const;
	size_t n_children() const;

	QPointF get_vert(int index, bool isSkelen) const;

	SlideApplier& get_slide_applier();

	void bind_texture();
	void add_trace(int index, const QPolygonF& polygon);
	void update_scale();
	bool eat_another_part(Part& other);
	void change_slider_value(int sliderIndex, int value);
	void remove_slider(int sliderIndex);
	void release_buffers();
	void add_child(Part* child);

	void update_transform(const QMatrix4x4& parentWorld = QMatrix4x4());

	float x() const;
	float y() const;
	float height() const;
	float width() const;

	float get_prescale() const;

	bool same_texture_as(Part* other) const;
	bool is_root() const;
	bool have_child() const;

	Part* get_child(size_t index) const;

	unsigned int vao_timeline() const;
	unsigned int vao_piying() const;

public:
	/// 用于标记部件在数组中的位置
	unsigned int _lay_index{ 0 };

private:
	float _x{ 0.f };
	float _y{ 0.f };
	float _height{ 2.f };
	float _width{ 2.f };
	
	const float _prescale;

	unsigned int _vao_timeline;
	unsigned int _vao_piying;
	unsigned int _vbo, _ebo;

	QOpenGLTexture& _texture;

	std::unique_ptr<PointVector> _vert_texture_origin;
	std::unique_ptr<PointVector> _vert_texture;

	std::vector<unsigned int> _indices;

	SlideApplier* _slide_applier;

	std::vector<Part*> _children;
	Part* _parent{ nullptr };
	
	std::unique_ptr<Joint> _joint;

	QMatrix4x4 localTransform;   // 相对于父节点
	QMatrix4x4 worldTransform;   // 缓存的世界变换
};
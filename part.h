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

	const float* float_data() const noexcept;
	const unsigned int* index_data() const noexcept;

	size_t float_size() const noexcept;
	size_t index_size() const noexcept;
	size_t vertex_size() const noexcept;
	size_t n_children() const noexcept;

	QPointF get_vert(int index, bool isSkelen) const;

	SlideApplier& get_slide_applier() noexcept;

	void bind_texture();
	void add_trace(int index, const QPolygonF& polygon);
	void update_scale();
	void update_global_scale();
	void change_slider_value(size_t sliderIndex, int value);
	void remove_slider(size_t sliderIndex) noexcept;
	void release_buffers() noexcept;
	void add_child(Part* child);

	void update_transform(const QMatrix4x4& parentWorld = QMatrix4x4());

	float x() const noexcept { return _x; }
	float y() const noexcept { return _y; }
	float height() const noexcept { return _height; }
	float width() const noexcept { return _width; }

	float local_top() const;
	float local_bottom() const;
	float local_left() const;
	float local_right() const;

	float global_top() const;
	float global_bottom() const;
	float global_left() const;
	float global_right() const;

	float get_prescale() const noexcept { return _prescale; }

	bool same_texture_as(Part* other) const noexcept;
	bool is_root() const noexcept;
	bool have_child() const noexcept;
	bool eat_another_part(Part& other);

	Part* get_child(size_t index) const;
	Part* get_parent() const noexcept { return _parent; }

	unsigned int vao_timeline() const noexcept { return _vao_timeline; }
	unsigned int vao_piying() const noexcept { return _vao_piying; }

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
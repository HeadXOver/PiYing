#pragma once

#include <QOpenGLWidget>
#include <qopenglfunctions_3_3_Core>
#include <memory>
#include <vector>

class QOpenGLShaderProgram;
class QOpenGLTexture;
class Timeline;
class Part;
class PointVector;
class QMenu;

struct ScaleTrans;

namespace spTimelineGL {

	constexpr float scroll_width = 0.02f;

	enum class UiType
	{
		Part,
		Timeline,
	};

	enum class DragType
	{
		None,
		Part,
		Cursor,
		Scroll,
	};

	struct PartCursor
	{
		int _index{ -1 };
		float x{ 0.f };
		float y{ 0.f };

		void set_cursor(int index);
		void update_after_resize();
	};
}

class TimelineGl : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

private:
	TimelineGl();
	~TimelineGl();

public:
	TimelineGl(const TimelineGl&) = delete;
	TimelineGl& operator=(const TimelineGl&) = delete;

	static TimelineGl& getInstance();

	float x_map_to_gl(const float x) const;
	float ratio() const { return _ratio; }

	void set_to_timeline();
	void set_to_part();

	void generate_vbo(const PointVector& pointVector, unsigned int& vbo);
	void update_vbo(const PointVector& pointVector, unsigned int vbo);
	void update_ebo(const std::vector<unsigned int>& indices, unsigned int ebo);
	void update_buffers(const PointVector& pointVector, const std::vector<unsigned int>& indices, unsigned int vbo, unsigned int ebo);
	void generate_ebo(const std::vector<unsigned int>& indices, unsigned int& vbo);
	void generate_vao(unsigned int& vao, unsigned int vbo, unsigned int ebo);
	void init_part_cursor();
	void release_buffers(unsigned int vao, unsigned int vbo, unsigned int ebo);
	void update_showing_parts();
	void update_is_draw_by_piying();

	Part* get_current_part() const;

protected: // gl function
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

protected: /// event
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	void move_time_cursor(float mouse_x);
	void paint_timeline();
	void draw_scroll();
	void paint_parts();
	void update_part_shader_program_data(Part& root, float x, float y);
	void draw_part_and_child(Part& part);
	void draw_insert_cursor();
	void ask_merge_parts();
	void insert_from_to(int from, int to);

	void part_beside_merge();
	void part_layer_merge();
	void part_exchange();
	void part_delete();
	void part_copy();
	void part_paste();
	void part_swap_by_showing_index(int from, int to);

	int get_index_by_mouse(const QPoint& mouse) const;
	int get_index_by_mouse(const QPoint& mouse, int& o_dragType) const;
	int current_layer() const;

private:
	unsigned int tlVAO = 0, tlVBO = 0, rectEBO = 0;
	unsigned int sVAO = 0, sVBO = 0;
	unsigned int tVAO = 0, tVBO = 0;

	int _current_select_timeline{ -1 };
	int _insert_part_index{ -1 };

	float time_cursor{ 0.f };
	float _scroll_positon{ 0.f };
	float _last_scroll_positon{ 0.f };
	float _ratio;
	float _part_total_scale{ 1.f };

	bool _pressing{ false };

	ScaleTrans* _scale_trans;
	ScaleTrans* _last_scale_trans;

	QPointF lastMiddleButtonPos;
	QPointF _last_mouse_pos;

	QOpenGLShaderProgram* _timeline_shader_program;
	QOpenGLShaderProgram* _simple_shader_program;
	QOpenGLShaderProgram* _simple_scroll_block_program;
	QOpenGLShaderProgram* _simple_with_trans_shader_program;
	QOpenGLShaderProgram* _rect_select_program;
	QOpenGLShaderProgram* _part_shader_program;

	std::unique_ptr<QOpenGLTexture> _texture;

	std::vector<Timeline*> _timelines;

	spTimelineGL::UiType _ui_type;

	spTimelineGL::PartCursor _part_cursor;
	spTimelineGL::PartCursor _moving_select_part;

	spTimelineGL::DragType _drag_type = spTimelineGL::DragType::None;

	QMenu* _drag_same_texture_menu;
	QMenu* _drag_diff_texture_menu;
	QMenu* _part_menu;

	QAction* _part_paste_action;

	std::vector<Part*> _showing_parts;

	Part* _part_to_show{ nullptr };
	Part* _part_copying{ nullptr };
};


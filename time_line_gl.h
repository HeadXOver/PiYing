#pragma once

#include <QOpenGLWidget>
#include <qopenglfunctions_3_3_Core>
#include <memory>
#include <vector>

#include "static_rect_vert.h"

class QOpenGLShaderProgram;
class QOpenGLTexture;
class Timeline;
class Part;
class PointVector;
class QMenu;

struct ScaleTrans;

enum class UiType
{
	Part,
	Timeline,
};

struct PartCursor
{
	int _index{ -1 };
	float x{ 0.f };
	float y{ 0.f };

	void set_cursor(int index);
	void update_after_resize();
};

class TimelineGl : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

public:
	TimelineGl(QWidget* parent);
	~TimelineGl();

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

	std::shared_ptr<Part> get_current_part() const;

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void wheelEvent(QWheelEvent* ev) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	void move_time_cursor(float mouse_x);
	void paint_timeline();
	void paint_parts();
	void draw_part_and_child(Part& part, float x, float y);
	void ask_merge_parts();

	void part_beside_new();
	void part_beside_ref();
	void part_layer_new();
	void part_layer_ref();

	int get_index_by_mouse(const QPoint& mouse) const;

private:
	unsigned int tVAO = 0, tVBO = 0, tEBO = 0;

	int _current_select_timeline{ -1 };

	float time_cursor{ 0.f };
	float _ratio;

	bool _draging_cursor{ false };
	bool _pressing{ false };

	ScaleTrans* _scale_trans;
	ScaleTrans* _last_scale_trans;

	QPointF lastMiddleButtonPos;

	QOpenGLShaderProgram* _rect_shader_program;
	QOpenGLShaderProgram* _rect_select_program;
	QOpenGLShaderProgram* _part_shader_program;

	std::unique_ptr<QOpenGLTexture> _texture;

	std::vector<Timeline*> _timelines;

	UiType _ui_type;

	PartCursor _part_cursor;
	PartCursor _moving_select_part;

	QMenu* _merge_menu;
};


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
	void update();
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

	void generate_vbo(PointVector& pointVector, unsigned int& vbo);
	void update_vbo(PointVector& pointVector, unsigned int vbo);
	void generate_ebo(std::vector<unsigned int>& indices, unsigned int& vbo);
	void generate_vao(unsigned int& vao, unsigned int vbo, unsigned int ebo);
	void init_part_cursor();

	Part* get_current_part() const;

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

private:
	unsigned int tVAO = 0, tVBO = 0, tEBO = 0;

	int _current_select{ -1 };

	float time_cursor{ 0.f };
	float _ratio;

	bool _draging_cursor{ false };

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
};


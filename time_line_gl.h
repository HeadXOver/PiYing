#pragma once

#include <QOpenGLWidget>
#include <qopenglfunctions_3_3_Core>
#include <memory>
#include <vector>

#include "static_rect_vert.h"

class QOpenGLShaderProgram;
class QOpenGLTexture;
class Timeline;

struct ScaleTrans;

enum class UiType
{
	Part,
	Timeline,
};

class TimelineGl : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

public:
	TimelineGl(QWidget* parent);
	~TimelineGl();

	float x_map_to_gl(const float x) const;
	void set_to_timeline();
	void set_to_part();

protected:
	void initializeGL() override;
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
	unsigned int VAO = 0, VBO = 0, EBO = 0;

	int _current_select{ -1 };

	float time_cursor{ 0.f };

	bool _draging_cursor{ false };

	ScaleTrans* _scale_trans;
	ScaleTrans* _last_scale_trans;

	QPointF lastMiddleButtonPos;

	QOpenGLShaderProgram* _rect_shader_program;
	QOpenGLShaderProgram* _part_shader_program;

	std::unique_ptr<QOpenGLTexture> _texture;

	std::vector<Timeline*> _timelines;

	UiType _ui_type = UiType::Timeline;
};


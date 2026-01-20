#include "time_line_gl.h"

#include "time_line.h"
#include "scale_trans.h"

#include <QOpenGLShaderProgram>
#include <qopengltexture>
#include <qmenu>

TimelineGl::TimelineGl(QWidget* parent) : QOpenGLWidget(), _ratio(1.f), _ui_type(UiType::Timeline)
{
	_merge_menu = new QMenu(this);

	QAction* beside_action = new QAction("合并", this);
	QAction* layer_action = new QAction("父子", this);
	QAction* exchange_action = new QAction("交换顺序", this);

	_merge_menu->addAction(beside_action);
	_merge_menu->addAction(layer_action);
	_merge_menu->addAction(exchange_action);

	connect(beside_action, &QAction::triggered, this, [this] { part_beside_merge(); });
	connect(layer_action, &QAction::triggered, this, [this] { part_layer_merge(); });
	connect(exchange_action, &QAction::triggered, this, [this] { part_exchange(); });

	_rect_shader_program = new QOpenGLShaderProgram(this);
	_part_shader_program = new QOpenGLShaderProgram(this);
	_rect_select_program = new QOpenGLShaderProgram(this);

	_scale_trans = new ScaleTrans();
	_last_scale_trans = new ScaleTrans();

	_timelines.push_back(new Timeline(2));
}

TimelineGl::~TimelineGl()
{
	for (Timeline* it : _timelines) delete it;

	makeCurrent();

	////////////////////////////////////////

	glDeleteBuffers(1, &tVBO);
	glDeleteVertexArrays(1, &tVAO);
	glDeleteBuffers(1, &tEBO);

	////////////////////////////////////////

	doneCurrent();
}
#include "time_line_gl.h"

#include "time_line.h"
#include "scale_trans.h"

#include <QOpenGLShaderProgram>
#include <qopengltexture>
#include <qmenu>

TimelineGl::TimelineGl(QWidget* parent) : QOpenGLWidget(), _ratio(1.f), _ui_type(UiType::Timeline)
{
	_merge_menu = new QMenu(this);

	QAction* beside_new = new QAction("合并 新建", this);
	QAction* beside_ref = new QAction("合并 覆盖", this);
	QAction* layer_new = new QAction("父子 新建", this);
	QAction* layer_ref = new QAction("父子 覆盖", this);

	_merge_menu->addAction(beside_new);
	_merge_menu->addAction(beside_ref);
	_merge_menu->addAction(layer_new);
	_merge_menu->addAction(layer_ref);

	connect(beside_new, &QAction::triggered, this, [this] { part_beside_new(); });
	connect(beside_ref, &QAction::triggered, this, [this] { part_beside_ref(); });
	connect(layer_new, &QAction::triggered, this, [this] { part_layer_new(); });
	connect(layer_ref, &QAction::triggered, this, [this] { part_layer_ref(); });

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
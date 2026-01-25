#include "time_line_gl.h"

#include "time_line.h"
#include "scale_trans.h"

#include <QOpenGLShaderProgram>
#include <qopengltexture>
#include <qmenu>

TimelineGl::TimelineGl(QWidget* parent) : QOpenGLWidget(), _ratio(1.f), _ui_type(spTimelineGL::UiType::Timeline)
{
	_drag_same_texture_menu = new QMenu(this);
	_drag_diff_texture_menu = new QMenu(this);
	_part_menu = new QMenu(this);

#pragma region [merge menu]

	QAction* beside_action = new QAction("同纹合并", this);
	QAction* layer_action = new QAction("父子", this);
	QAction* exchange_action = new QAction("交换顺序", this);

	connect(beside_action, &QAction::triggered, this, [this] { part_beside_merge(); });
	connect(layer_action, &QAction::triggered, this, [this] { part_layer_merge(); });
	connect(exchange_action, &QAction::triggered, this, [this] { part_exchange(); });

	_drag_same_texture_menu->addAction(beside_action);
	_drag_same_texture_menu->addAction(layer_action);
	_drag_same_texture_menu->addAction(exchange_action);

	_drag_diff_texture_menu->addAction(layer_action);
	_drag_diff_texture_menu->addAction(exchange_action);

#pragma endregion

#pragma region [part menu]

	QAction* delete_action = new QAction("删除", this);

	_part_menu->addAction(delete_action);

	connect(delete_action, &QAction::triggered, this, [this] { part_delete(); });

#pragma endregion

	_timeline_shader_program			= new QOpenGLShaderProgram(this);
	_simple_shader_program				= new QOpenGLShaderProgram(this);
	_simple_scroll_block_program		= new QOpenGLShaderProgram(this);
	_simple_with_trans_shader_program	= new QOpenGLShaderProgram(this);
	_part_shader_program				= new QOpenGLShaderProgram(this);
	_rect_select_program				= new QOpenGLShaderProgram(this);

	_scale_trans = new ScaleTrans();
	_last_scale_trans = new ScaleTrans();

	_timelines.push_back(new Timeline(2));
}

TimelineGl::~TimelineGl()
{
	for (Timeline* it : _timelines) delete it;

	makeCurrent();

	////////////////////////////////////////

	glDeleteBuffers(1, &tlVBO);
	glDeleteVertexArrays(1, &tlVAO);
	glDeleteBuffers(1, &rectEBO);

	////////////////////////////////////////

	doneCurrent();
}
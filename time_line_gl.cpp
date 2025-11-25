#include "time_line_gl.h"

#include "time_line.h"
#include "base_math.h"
#include "scale_trans.h"

#include <QOpenGLShaderProgram>
#include <qmessagebox>
#include <qimage>
#include <qopengltexture>
#include <qmouseevent>

TimeLineGL::TimeLineGL(QWidget* parent) : QOpenGLWidget(parent)
{
	_rect_shader_program = new QOpenGLShaderProgram(this);

	_scale_trans = new ScaleTrans();
	_last_scale_trans = new ScaleTrans();

	_timelines.push_back(new Timeline(3.f));
	_timelines.push_back(new Timeline(2.f));
	_timelines.push_back(new Timeline(5.f));
	_timelines.push_back(new Timeline(2.2f));
	_timelines.push_back(new Timeline(1.f));
	_timelines.push_back(new Timeline(4.6f));
}

TimeLineGL::~TimeLineGL()
{
	for (Timeline* it : _timelines) delete it;

	makeCurrent();

	////////////////////////////////////////

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	////////////////////////////////////////

	doneCurrent();
}

float TimeLineGL::x_map_to_gl(const float x) const
{
	return x / width() * 2.f - 1.f;
}

void TimeLineGL::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.1f, 0.1f, 0.2f, 0.4f);
	_texture = std::make_unique<QOpenGLTexture>(QImage(":/PiYing/timeline.png").flipped());
	_texture->setWrapMode(QOpenGLTexture::Repeat);

	//////////////initialize background///////////////////////

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/////////////////////////////////////////////

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(RECTANGLE_VERT), RECTANGLE_VERT, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDECES), RECTANGLE_INDECES, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	_rect_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/timeline_rect_shape.vert");
	_rect_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/timeline_rect_shape.frag");
	_rect_shader_program->link();
	_rect_shader_program->setUniformValue("texture1", 0);

	glBindVertexArray(0);

	// global setting

	glActiveTexture(GL_TEXTURE0);
}

void TimeLineGL::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO);///////////////////////////////////////////////////////

	_rect_shader_program->bind();
	_texture->bind();

	for (int i = 0; i < _timelines.size(); i++) {
		Timeline* timeline = _timelines[i];
		_rect_shader_program->setUniformValue("selected", i == _current_select);
		_rect_shader_program->setUniformValue("lenth", timeline->lenth());
		_rect_shader_program->setUniformValue("trans", timeline->get_transform(i, _scale_trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);////////////////////////////////////////////////////////////
}

void TimeLineGL::wheelEvent(QWheelEvent* ev)
{
	float scaleFactor = 1.0f + ev->angleDelta().y() / 1200.f;
	if (scaleFactor < 0.1f) scaleFactor = 0.1f;

	_scale_trans->scale_lenth *= scaleFactor;

	const float gl_mouse_x = x_map_to_gl(ev->position().x());
	const float diff_x = gl_mouse_x - _scale_trans->trans_x;
	const float to_trans_x = diff_x * (1.f - scaleFactor);

	_scale_trans->trans_x = cus::min(_scale_trans->scale_lenth - 1.f, _scale_trans->trans_x + to_trans_x);

	*_last_scale_trans = *_scale_trans;
	update();
	ev->accept();
}

void TimeLineGL::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::MiddleButton) {
		lastMiddleButtonPos = event->position();
		*_last_scale_trans = *_scale_trans;
	}
}

void TimeLineGL::mouseReleaseEvent(QMouseEvent* e)
{
}

void TimeLineGL::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::MiddleButton) {
		const float to_trans = _last_scale_trans->trans_x + (event->position().x() - lastMiddleButtonPos.x()) * 2.f / width();
		_scale_trans->trans_x = cus::min(_scale_trans->scale_lenth - 1.f, to_trans);
		update();
	}
}

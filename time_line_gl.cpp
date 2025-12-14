#include "time_line_gl.h"

#include "time_line.h"
#include "base_math.h"
#include "scale_trans.h"
#include "KeyboardStateWin.h"
#include "part.h"
#include "piYingGL.h"
#include "point_vector.h"
#include "global_objects.h"

#include <QOpenGLShaderProgram>
#include <qmessagebox>
#include <qimage>
#include <qopengltexture>
#include <qmouseevent>
#include <qpainter>

TimelineGl::TimelineGl(QWidget* parent) : QOpenGLWidget(), _ratio(1.f), _ui_type(UiType::Timeline)
{
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

float TimelineGl::x_map_to_gl(const float x) const
{
	return x / width() * 2.f - 1.f;
}

void TimelineGl::set_to_timeline()
{
	_ui_type = UiType::Timeline;
}

void TimelineGl::set_to_part()
{
	_ui_type = UiType::Part;
}

void TimelineGl::generate_vbo(PointVector& pointVefctor, unsigned int& vbo)
{
	makeCurrent();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pointVefctor.float_size() * sizeof(float), pointVefctor.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	doneCurrent();
}

void TimelineGl::generate_ebo(std::vector<unsigned int>& indices, unsigned int& ebo)
{
	makeCurrent();

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	doneCurrent();
}

void TimelineGl::generate_vao(unsigned int& vao, unsigned int vbo, unsigned int ebo)
{
	makeCurrent();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
	doneCurrent();
}

void TimelineGl::init_part_cursor()
{
	if (parts.size() > 0 && _part_cursor._index < 0) _part_cursor.set_cursor(0);
}

Part* TimelineGl::get_current_part() const
{
	if(parts.size() == 0) return nullptr;

	if (_part_cursor._index < 0) return nullptr;

	return parts[_part_cursor._index];
}

void TimelineGl::initializeGL()
{
	if (_texture) return;

	initializeOpenGLFunctions();
	glClearColor(0.1f, 0.1f, 0.2f, 0.4f);
	_texture = std::make_unique<QOpenGLTexture>(QImage(":/PiYing/timeline.png").flipped());
	_texture->setWrapMode(QOpenGLTexture::Repeat);

	//////////////initialize background///////////////////////

	glGenVertexArrays(1, &tVAO);
	glGenBuffers(1, &tVBO);
	glGenBuffers(1, &tEBO);

	/////////////////////////////////////////////

	glBindVertexArray(tVAO);
	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tEBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(RECTANGLE_VERT), RECTANGLE_VERT, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDECES), RECTANGLE_INDECES, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	_rect_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/timeline_rect_shape.vert");
	_rect_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/timeline_rect_shape.frag");
	_rect_shader_program->link();
	_rect_shader_program->setUniformValue("texture1", 0);

	//////////////////////////////////////////////

	_rect_select_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/part_select_rect.vert");
	_rect_select_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/part_select_rect.frag");
	_rect_select_program->link();

	glBindVertexArray(0);

	//////////////////////////////////////////////

	_part_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/square_icon.vert");
	_part_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/square_icon.frag");
	if (!_part_shader_program->link()) QMessageBox::warning(this, "Error", "Part shader program link error");

	_part_shader_program->setUniformValue("texture1", 0);

	// global setting

	glActiveTexture(GL_TEXTURE0);
}

void TimelineGl::resizeGL(int w, int h)
{
    _ratio = (width() * piYingGL->height()) / (float)(piYingGL->width() * height());
	_part_cursor.update();
}

void TimelineGl::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (_ui_type) {
	case UiType::Timeline: paint_timeline(); break;
	case UiType::Part: paint_parts(); break;
	}
}

void TimelineGl::wheelEvent(QWheelEvent* ev)
{
	if (_ui_type == UiType::Timeline) {
		float scaleFactor = 1.0f + ev->angleDelta().y() / 1200.f;
		if (scaleFactor < 0.1f) scaleFactor = 0.1f;

		_scale_trans->scale_lenth *= scaleFactor;

		const float gl_mouse_x = x_map_to_gl(ev->position().x());
		const float diff_x = gl_mouse_x - _scale_trans->trans_x;
		const float to_trans_x = diff_x * (1.f - scaleFactor);

		_scale_trans->trans_x = cus::min(_scale_trans->scale_lenth - 1.f, _scale_trans->trans_x + to_trans_x);

		const float xTimelineCursor = width() * _scale_trans->trans_x / 2.f + time_cursor;
		const float diffTimelineCursor = xTimelineCursor - width() * (_scale_trans->trans_x + 1.f) / 2.f;
		time_cursor -= diffTimelineCursor * (1.f - scaleFactor);

		*_last_scale_trans = *_scale_trans;
	}
	else if (_ui_type == UiType::Part) {

	}
	update();
	ev->accept();
}

void TimelineGl::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::MiddleButton) {
		lastMiddleButtonPos = event->position();
		*_last_scale_trans = *_scale_trans;
	}
	else if (event->buttons() == Qt::LeftButton) {
		if (_ui_type == UiType::Timeline) {
			if (KeyboardStateWin::isCtrlHeld()) {
				_draging_cursor = true;
				move_time_cursor(event->position().x());
				update();
				return;
			}
			const float diff_map = event->position().x() - (width() * _scale_trans->trans_x / 2.f + time_cursor);
			const float diff_gl = diff_map * 2.f / (float)width();
			if (cus::abs(diff_gl) < 0.02f) {
				_draging_cursor = true;
			}
		}
		else if (_ui_type == UiType::Part) {
			const int x = event->pos().x() * 5 / width();
			const int y = event->pos().y() * 5 / (height() * _ratio);

			const int index = 5 * y + x;
			if (index >= parts.size() || index == _part_cursor._index) return;

			_part_cursor.set_cursor(5 * y + x);

			update();
		}
	}
}

void TimelineGl::mouseReleaseEvent(QMouseEvent* e)
{
	_draging_cursor = false;
}

void TimelineGl::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton) {
		if (_draging_cursor || KeyboardStateWin::isCtrlHeld()) {
			move_time_cursor(event->position().x());
			update();
		}
	}
	else if (event->buttons() == Qt::MiddleButton) {
		const float to_trans_x = _last_scale_trans->trans_x + (event->position().x() - lastMiddleButtonPos.x()) * 2.f / width();
		_scale_trans->trans_x = cus::min(_scale_trans->scale_lenth - 1.f, to_trans_x);

		size_t timeline_size = _timelines.size();
		if (timeline_size > 5) {
			float to_trans_y = _last_scale_trans->trans_y - (event->position().y() - lastMiddleButtonPos.y()) * 2.f / height();
			to_trans_y = cus::max(_scale_trans->scale_height - 1.f, to_trans_y);
			_scale_trans->trans_y = cus::min(0.4f * timeline_size - 2.f, to_trans_y);
		}
		update();
	}
}

void TimelineGl::move_time_cursor(float mouse_x)
{
	const float half_width = width() / 2.f;
	time_cursor = cus::max(half_width * (1.f - _scale_trans->scale_lenth), mouse_x - _scale_trans->trans_x * half_width); ///< 确保非负
}

void TimelineGl::paint_timeline()
{
	glBindVertexArray(tVAO);///////////////////////////////////////////////////////

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

	QPainter painter(this);
	QPen pen(Qt::yellow, 4);
	painter.setPen(pen);
	const float timeCursorPosition = width() * _scale_trans->trans_x / 2.f + time_cursor;
	painter.drawLine(timeCursorPosition, 0, timeCursorPosition, height());
}

void TimelineGl::paint_parts()
{
	if (parts.size() == 0) return;

	float x, y, scale;

	_rect_select_program->bind();

	glBindVertexArray(tVAO);///////////////////////////////////////////////////////

	_rect_select_program->setUniformValue("trans", QVector2D(_part_cursor.x, _part_cursor.y));
	_rect_select_program->setUniformValue("ratio", _ratio);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	////////////////////////////////////////////////////////////

	_part_shader_program->bind();
	_part_shader_program->setUniformValue("ratio", _ratio);

	Part* part;
	for (int i = 0; i < parts.size(); i++) {
		part = parts[i];

		x = -0.8f + (i % 5) * 0.4f;
		y = 0.8f - (i / 5) * 0.4f;

		glBindVertexArray(part->vao_timeline());

		scale = 0.38f / cus::max(part->width(), part->height());
		_part_shader_program->setUniformValue("scale", scale);
		_part_shader_program->setUniformValue("x", x - scale * part->x());
		_part_shader_program->setUniformValue("y", y - scale * part->y());

		part->bind_texture();
		glDrawElements(GL_TRIANGLES, (GLsizei)part->index_size(), GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);////////////////////////////////////////////////////////////
}

void PartCursor::set_cursor(int index)
{
	_index = index;

	x = -0.8f + (index % 5) * 0.4f;
	y = 0.8f - (index / 5) * 0.4f * timelineGl->ratio();

	piYingGL->update();
}

void PartCursor::update()
{
	y = 0.8f - (_index / 5) * 0.4f * timelineGl->ratio();
}

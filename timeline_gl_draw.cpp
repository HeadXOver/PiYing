#include "time_line_gl.h"

#include "base_math.h"
#include "time_line.h"
#include "scale_trans.h"
#include "part.h"
#include "global_objects.h"

#include <qpainter>
#include <qopengltexture>
#include <QOpenGLShaderProgram>

void TimelineGl::paint_timeline()
{
	glBindVertexArray(tVAO);///////////////////////////////////////////////////////

	_timeline_shader_program->bind();
	_texture->bind();

	for (int i = 0; i < _timelines.size(); i++) {
		Timeline* timeline = _timelines[i];
		_timeline_shader_program->setUniformValue("lenth", timeline->lenth());
		_timeline_shader_program->setUniformValue("trans", timeline->get_transform(i, _scale_trans));

		if (i == _current_select_timeline) {
			_timeline_shader_program->setUniformValue("aColor", QVector4D(0.6f, 0.6f, 1.f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			_timeline_shader_program->setUniformValue("aColor", QVector4D(1.f, 1.f, 1.f, 1.f));
			continue;
		}

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

	_rect_select_program->bind();

	glBindVertexArray(tVAO);///////////////////////////////////////////////////////

	if (_pressing) {
		_rect_select_program->setUniformValue("trans", QVector2D(_moving_select_part.x, _moving_select_part.y));
		_rect_select_program->setUniformValue("aColor", QVector4D(0.3f, 0.3f, 0.6f, 1.0f));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		_rect_select_program->setUniformValue("aColor", QVector4D(0.6f, 0.3f, 0.3f, 1.0f));
	}
	_rect_select_program->setUniformValue("trans", QVector2D(_part_cursor.x, _part_cursor.y));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	////////////////////////////////////////////////////////////

	_part_shader_program->bind();

	float x, y;
	for (int i = 0; i < parts.size(); i++) {
		x = -0.8f + (i % 5) * 0.4f;
		y = +0.8f - (i / 5) * 0.4f;

		draw_part_and_child(*parts[i], x, y);
	}

	glBindVertexArray(0);////////////////////////////////////////////////////////////
}

void TimelineGl::draw_part_and_child(Part& part, float x, float y)
{
	glBindVertexArray(part.vao_timeline());

	const float scale = 0.38f / cus::max(part.width(), part.height());
	_part_shader_program->setUniformValue("scale", scale);
	_part_shader_program->setUniformValue("x", x - scale * part.x());
	_part_shader_program->setUniformValue("y", y - scale * part.y());

	part.bind_texture();
	glDrawElements(GL_TRIANGLES, (GLsizei)part.index_size(), GL_UNSIGNED_INT, 0);
}

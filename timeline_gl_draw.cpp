#include "time_line_gl.h"

#include "base_math.h"
#include "time_line.h"
#include "scale_trans.h"
#include "part.h"

#include <qpainter>
#include <qopengltexture>
#include <QOpenGLShaderProgram>

void TimelineGl::paint_timeline()
{
	glBindVertexArray(tlVAO);///////////////////////////////////////////////////////

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

void TimelineGl::draw_scroll()
{
	glBindVertexArray(sVAO); // 绘制滑动条 /////////////////////////////////////////////

	_simple_shader_program->bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	_simple_scroll_block_program->bind();
	
	const size_t y = _showing_parts.size() / 5 + 1;

	_part_total_scale = y * 0.2f * _ratio;

	if (_part_total_scale <= 1.f) {
		_simple_scroll_block_program->setUniformValue("scale", 1.f);
		_simple_scroll_block_program->setUniformValue("trans", 0.f);
	}
	else {
		_simple_scroll_block_program->setUniformValue("scale", _part_total_scale);
		_simple_scroll_block_program->setUniformValue("trans", _scroll_positon * 2.f);
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);////////////////////////////////////////////////////////////
}

void TimelineGl::paint_parts()
{
	if (_showing_parts.size() == 0) return;

	_rect_select_program->bind();

	glBindVertexArray(tlVAO);///////////////////////////////////////////////////////

	/// 绘制选择框
	if (_pressing && _insert_part_index < 0) {
		_rect_select_program->setUniformValue("trans", QVector2D(_moving_select_part.x, _moving_select_part.y + _scroll_positon * 2.f * _part_total_scale));
		_rect_select_program->setUniformValue("aColor", QVector4D(0.3f, 0.3f, 0.6f, 1.0f));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		_rect_select_program->setUniformValue("aColor", QVector4D(0.6f, 0.3f, 0.3f, 1.0f));
	}

	_rect_select_program->setUniformValue("trans", QVector2D(_part_cursor.x, _part_cursor.y + _scroll_positon * 2.f * _part_total_scale));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	////////////////////////////////////////////////////////////

	/// 绘制每个部件
	_part_shader_program->bind();

	float x, y;
	for (int i = 0; i < _showing_parts.size(); i++) {
		x = -0.8f + (i % 5) * 0.4f;
		y = +0.8f - (i / 5) * 0.4f;

		update_part_shader_program_data(*_showing_parts[i], x, y);

		draw_part_and_child(*_showing_parts[i]);
	}

	glBindVertexArray(0);////////////////////////////////////////////////////////////

	draw_insert_cursor();

	draw_scroll();
}

void TimelineGl::update_part_shader_program_data(Part& root, float x, float y)
{
	const float scale = 0.35f / PiYingCus::max(root.width() * (1.f + spTimelineGL::scroll_width), root.height());
	_part_shader_program->setUniformValue("scale", scale);
	_part_shader_program->setUniformValue("scroll", _scroll_positon * 2.f * _part_total_scale);
	_part_shader_program->setUniformValue("x", x - scale * root.x());
	_part_shader_program->setUniformValue("y", y - scale * root.y());
	_part_shader_program->setUniformValue("prescale", root.get_prescale());
}

void TimelineGl::draw_part_and_child(Part& part)
{
	part.bind_texture();
	glBindVertexArray(part.vao_timeline());
	glDrawElements(GL_TRIANGLES, (GLsizei)part.index_size(), GL_UNSIGNED_INT, 0);

	for (int i = 0; i < part.n_children(); i++) {
		draw_part_and_child(*part.get_child(i));
	}
}

void TimelineGl::draw_insert_cursor()
{
	if(_insert_part_index < 0) return;

	if(_insert_part_index > _showing_parts.size()) _insert_part_index = (int)_showing_parts.size();

	glBindVertexArray(tVAO);// 绘制插入光标 /////////////////////////////////////////////

	_simple_with_trans_shader_program->bind();

	const int x = _insert_part_index % 5;
	const int y = _insert_part_index / 5;

	constexpr float partWidth = 0.4f * (1.f - spTimelineGL::scroll_width);

	const float fx = x * partWidth - 1.f;
	const float fy = 1.f - (y * 0.4f + 0.2f) * _ratio;

	_simple_with_trans_shader_program->setUniformValue("trans", QVector2D(fx, fy + _scroll_positon * 2.f * _part_total_scale));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);////////////////////////////////////////////////////////////
}

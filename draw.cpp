#include "piYingGL.h"
#include "piYing.h"
#include "ch_element_tool.h"
#include "image_texture.h"
#include "point_vector.h"
#include "point_vector_layer.h"
#include "vert_groups.h"
#include "vert_group.h"
#include "base_math.h"
#include "enum_edit_mode.h"
#include "time_line_gl.h"
#include "part.h"
#include "global_objects.h"

#include <qpainter>
#include <qopengltexture>
#include <QOpenGLShaderProgram.h>
#include <qmessagebox>

void PiYingGL::draw_selected_points(int nSelectedPoint)
{
	glBindVertexArray(svVAO);
	glBindBuffer(GL_ARRAY_BUFFER, svVBO);
	_selected_vert_shader_program->bind();

	_selected_vert_shader_program->setUniformValue("trc", getViewProjMatrix());
	_selected_vert_shader_program->setUniformValue("is_out", true);
	glDrawArrays(GL_POINTS, 0, nSelectedPoint);
	_selected_vert_shader_program->setUniformValue("is_out", false);
	glDrawArrays(GL_POINTS, 0, nSelectedPoint);
	glBindVertexArray(0);
}

void PiYingGL::draw_selected_triangle(int nSelectedPoint)
{
	const int currentVector = getCurrentChRow();
	if (currentVector < 0) return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(ttVAO); ////////////////////////////////////////////////////

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ttEBO);

	_texture_tri_shader_program->bind();

	_texture_tri_shader_program->setUniformValue("is_skelen", editMode == EditMode::characterSkeleton);
	_texture_tri_shader_program->setUniformValue("trc", getViewProjMatrix());
	_texture_tri_shader_program->setUniformValue("is_line", false);
	_texture_tri_shader_program->setUniformValue("is_selected", true);
	glDrawElements(GL_TRIANGLES, nSelectedPoint * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0); ////////////////////////////////////////////////////
}

void PiYingGL::draw_triangle_frame(bool isSkelen)
{
	const int currentVector = getCurrentChRow();
	if (currentVector < 0) return;

	glBindVertexArray(ttVAO); ////////////////////////////////////////////////////

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);

	_texture_tri_shader_program->bind();

	_texture_tri_shader_program->setUniformValue("is_skelen", isSkelen);
	_texture_tri_shader_program->setUniformValue("trc", getViewProjMatrix());
	_texture_tri_shader_program->setUniformValue("is_line", false);
	_texture_tri_shader_program->setUniformValue("is_selected", false);
	glDrawElements(GL_TRIANGLES, (GLsizei)characterTriangleIndices[currentVector].size(), GL_UNSIGNED_INT, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // 把填充改成“线框”
	_texture_tri_shader_program->setUniformValue("is_line", true);
	glDrawElements(GL_TRIANGLES, (GLsizei)characterTriangleIndices[currentVector].size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(0); ////////////////////////////////////////////////////
}

void PiYingGL::draw_rectangle(float cx, float cy, float width, float height)
{
	glBindVertexArray(rtVAO); ////////////////////////////////////////////////////

	_rectangle_shader_program->bind();

	QMatrix4x4 trc;
	trc.translate(cx, cy);
	trc.scale(width / 2.f, height / 2.f);
	_rectangle_shader_program->setUniformValue("trc", getViewProjMatrix() * trc);

	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glBindVertexArray(0); ////////////////////////////////////////////////////////
}

void PiYingGL::draw_group_rectangle(int groupIndex)
{
	int currentRow = getCurrentChRow();
	if (currentRow < 0) return;

	VertGroups& groups = *_character_vert_groups[currentRow];
	VertGroup& group = *groups[groupIndex];

	int vertSize = group.vert_size();
	if (vertSize <= 1) return;

	PointVectorLayer layer(*characterVerts[currentRow]);

	bool edit_skelon = editMode == EditMode::characterSkeleton;

	QPointF pointInGroup = edit_skelon ? layer[group[0]] : layer(group[0]);

	float top = pointInGroup.y();
	float left = pointInGroup.x();
	float bottom = pointInGroup.y();
	float right = pointInGroup.x();

	for (int i = 1; i < vertSize; i++) {
		pointInGroup = edit_skelon ? layer[group[i]] : layer(group[i]);
		top = cus::max(top, pointInGroup.y());
		left = cus::min(left, pointInGroup.x());
		bottom = cus::min(bottom, pointInGroup.y());
		right = cus::max(right, pointInGroup.x());
	}

	draw_rectangle((left + right) / 2.f, (top + bottom) / 2.f, right - left + 0.05f / viewScale.value(), top - bottom + 0.05f / viewScale.value());
}

void PiYingGL::paintBackgrounds()
{
	draw_view_rectangle();

	glBindVertexArray(bgVAO);///////////////////////////////////////////////////////

	bgShaderProgram->bind();

	for (int i = 0; i < backGrounds.size(); i++) {
		ImageTexture* it = backGrounds[i];
		it->bind();

		bgShaderProgram->setUniformValue("trc", getBgShaderMatrix(it->transform()));
		bgShaderProgram->setUniformValue("selected", i == getCurrentBgRow());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);////////////////////////////////////////////////////////////
}

void PiYingGL::paintCharacterTexture()
{
	/// 绘制标准视图框
	draw_view_rectangle();

	int i = getCurrentChRow();
	if (i < 0) return;

	glBindVertexArray(bgVAO);////////////////////////////////////////////////////////

	bgShaderProgram->bind();

	characterTextures[i]->bind();

	bgShaderProgram->setUniformValue("trc", getViewProjMatrix());
	bgShaderProgram->setUniformValue("selected", false);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0); ////////////////////////////////////////////////////

	draw_triangle_frame(false);
	
	if (ch_element_tool_) ch_element_tool_->draw();
}

void PiYingGL::paint_applied_texture()
{
	draw_view_rectangle();

	int i = getCurrentChRow();
	if (i < 0) return;

	glBindVertexArray(chVAO); ////////////////////////////////////////////////////

	chShaderProgram->bind();

	characterTextures[i]->bind();
	chShaderProgram->setUniformValue("trc", getViewProjMatrix());
	glDrawElements(GL_TRIANGLES, (GLsizei)characterTriangleIndices[i].size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void PiYingGL::paint_selected_part()
{
	if(parts.size() == 0) return;

	Part* currentPart = timelineGl->get_current_part();
	if (!currentPart) return;

	chShaderProgram->bind();
	chShaderProgram->setUniformValue("trc", getViewProjMatrix());

	glBindVertexArray(currentPart->vao_piying());

	currentPart->bind_texture();
	glDrawElements(GL_TRIANGLES, (GLsizei)currentPart->index_size(), GL_UNSIGNED_INT, 0);

	_texture_tri_shader_program->bind();
	_texture_tri_shader_program->setUniformValue("is_skelen", false);
	_texture_tri_shader_program->setUniformValue("trc", getViewProjMatrix());
	_texture_tri_shader_program->setUniformValue("is_selected", false);
	_texture_tri_shader_program->setUniformValue("is_line", true);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // 把填充改成“线框”
	glDrawElements(GL_TRIANGLES, (GLsizei)currentPart->index_size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(0);
}

void PiYingGL::draw_view_rectangle()
{
	glBindVertexArray(rtVAO); ////////////////////////////////////////////////////

	_rectangle_shader_program->bind();

	_rectangle_shader_program->setUniformValue("trc", getViewProjMatrix());

	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glBindVertexArray(0); ////////////////////////////////////////////////////
}

void PiYingGL::update_ch_verts()
{
	int currentVector = getCurrentChRow();
	if (currentVector < 0) return;

	makeCurrent();
	glBindBuffer(GL_ARRAY_BUFFER, chVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);
	glBufferData(GL_ARRAY_BUFFER, characterVerts[currentVector]->float_size() * sizeof(float), characterVerts[currentVector]->data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, characterTriangleIndices[currentVector].size() * sizeof(unsigned int), characterTriangleIndices[currentVector].data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	doneCurrent();
}

void PiYingGL::update_selected_verts(const std::vector<float>& selected_points)
{
	makeCurrent();
	glBindBuffer(GL_ARRAY_BUFFER, svVBO);
	glBufferData(GL_ARRAY_BUFFER, selected_points.size() * sizeof(float), selected_points.data(), GL_DYNAMIC_DRAW);
	doneCurrent();
}

void PiYingGL::update_selected_triangle(const unsigned int* data, int size)
{
	makeCurrent();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ttEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	doneCurrent();
}

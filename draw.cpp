#include "piYingGL.h"
#include "piYing.h"
#include "ch_element_tool.h"
#include "image_texture.h"
#include "character_texture.h"
#include "point_vector.h"
#include "point_vector_layer.h"
#include "vert_groups.h"
#include "vert_group.h"
#include "base_math.h"
#include "time_line_gl.h"
#include "part.h"
#include "parts.h"

#include "enum_character_texture_tool_state.h"
#include "enum_edit_mode.h"

#include <qpainter>
#include <qopengltexture>
#include <QOpenGLShaderProgram.h>
#include <qmessagebox>

void PiYingGL::draw_selected_points(int nSelectedPoint)
{
	glBindVertexArray(svVAO);

	_selected_vert_shader_program->bind();

	_selected_vert_shader_program->setUniformValue("aColor", QVector4D(0.f, 0.f, 0.f, 1.f));
	_selected_vert_shader_program->setUniformValue("is_out", true);
	glDrawArrays(GL_POINTS, 0, nSelectedPoint);
	_selected_vert_shader_program->setUniformValue("aColor", QVector4D(1.f, 0.f, 0.f, 1.f));
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
	_texture_tri_shader_program->setUniformValue("aColor", QVector4D(0.0f, 0.0f, 1.0f, 0.15f));
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
	_texture_tri_shader_program->setUniformValue("aColor", QVector4D(1.0f, 0.0f, 0.0f, 0.15f));
	glDrawElements(GL_TRIANGLES, (GLsizei)characterTriangleIndices[currentVector].size(), GL_UNSIGNED_INT, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // 把填充改成“线框”
	_texture_tri_shader_program->setUniformValue("aColor", QVector4D(0.0f, 1.0f, 0.0f, 0.8f));
	glDrawElements(GL_TRIANGLES, (GLsizei)characterTriangleIndices[currentVector].size(), GL_UNSIGNED_INT, 0);
	_texture_tri_shader_program->setUniformValue("aColor", QVector4D(1.0f, 0.0f, 0.0f, 0.15f));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(0); ////////////////////////////////////////////////////
}

void PiYingGL::draw_rectangle(float cx, float cy, float width, float height)
{
	glBindVertexArray(RECTANGLE_TEXTURE_VAO); ////////////////////////////////////////////////////

	_rectangle_shader_program->bind();

	QMatrix4x4 trc;
	trc.translate(cx, cy);
	trc.scale(width / 2.f, height / 2.f);
	_rectangle_shader_program->setUniformValue("trc", getViewProjMatrix() * trc);

	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glBindVertexArray(0); ////////////////////////////////////////////////////////
}

void PiYingGL::paintBackgrounds()
{
	draw_view_rectangle();

	glBindVertexArray(RECTANGLE_TEXTURE_VAO);///////////////////////////////////////////////////////

	_texture_color_shader_programme->bind();

	for (int i = 0; i < backGrounds.size(); i++) {
		ImageTexture* it = backGrounds[i];
		it->bind();

		_texture_color_shader_programme->setUniformValue("trc", getBgShaderMatrix(it->getMatrix()));
		if (i == getCurrentBgRow()) {
			_texture_color_shader_programme->setUniformValue("aColor", QVector4D(1.0f, 0.8f, 0.8f, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			_texture_color_shader_programme->setUniformValue("aColor", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
			continue;
		}

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);////////////////////////////////////////////////////////////
}

void PiYingGL::paint_over_view()
{
	draw_view_rectangle();

	glBindVertexArray(RECTANGLE_TEXTURE_VAO);///////////////////////////////////////////////////////

	_texture_color_shader_programme->bind();

	for (int i = 0; i < backGrounds.size(); i++) {
		backGrounds[i]->bind();

		_texture_color_shader_programme->setUniformValue("trc", getBgShaderMatrix(backGrounds[i]->getMatrix()));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);////////////////////////////////////////////////////////////
}

void PiYingGL::paintCharacterTexture()
{
	int i = getCurrentChRow();
	if (i < 0) return;

	glBindVertexArray(RECTANGLE_TEXTURE_VAO);////////////////////////////////////////////////////////

	_texture_color_shader_programme->bind();

	characterTextures[i]->bind();

	_texture_color_shader_programme->setUniformValue("trc", getViewMatrix() * characterTextures[i]->getMatrix());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0); ////////////////////////////////////////////////////

	draw_triangle_frame(false);
	
	if (ch_element_tool_) ch_element_tool_->draw();
}

void PiYingGL::paint_character_skeleton()
{
	paint_applied_texture();
	if (_ch_tool_state != CharacterToolState::LibreSelectVert &&
		_ch_tool_state != CharacterToolState::RectSelectVert &&
		_ch_tool_state != CharacterToolState::RectSelectTriangle &&
		_ch_tool_state != CharacterToolState::LibreSelectTriangle
		) return;
	draw_triangle_frame(true);
	if (ch_element_tool_) ch_element_tool_->draw();
}

void PiYingGL::paint_slider_platform()
{
	paint_in_vector_part();
	if (ch_element_tool_) ch_element_tool_->draw();
}

void PiYingGL::paint_applied_texture()
{
	int i = getCurrentChRow();
	if (i < 0) return;

	glBindVertexArray(chVAO); ////////////////////////////////////////////////////

	_ch_shader_program->bind();
	_ch_shader_program->setUniformValue("prescale", characterTextures[i]->get_prescale());

	characterTextures[i]->bind();
	glDrawElements(GL_TRIANGLES, (GLsizei)characterTriangleIndices[i].size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void PiYingGL::paint_selected_part()
{
	if(Parts::getInstance().is_empty()) return;

	Part* currentPart = TimelineGl::getInstance().get_current_part();
	if (!currentPart) return;

	_ch_shader_program->bind();
	_ch_shader_program->setUniformValue("prescale", currentPart->get_prescale());

	glBindVertexArray(currentPart->vao_piying());

	currentPart->bind_texture();
	glDrawElements(GL_TRIANGLES, (GLsizei)currentPart->index_size(), GL_UNSIGNED_INT, 0);

	_texture_tri_shader_program->bind();
	_texture_tri_shader_program->setUniformValue("is_skelen", false);
	_texture_tri_shader_program->setUniformValue("aColor", QVector4D(0.0f, 1.0f, 0.0f, 0.8f));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   ///< 把填充改成“线框”
	glDrawElements(GL_TRIANGLES, (GLsizei)currentPart->index_size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   ///< 恢复填充

	glBindVertexArray(0);  /// 解绑VAO ///////////////////////////////
}

void PiYingGL::paint_in_vector_part()
{
	size_t partSize = Parts::getInstance().size();

	if (partSize == 0) return;

	_ch_shader_program->bind();

	bool nothing = true;
	for (size_t i = 0; i < partSize; i++) {
		/// 如果当前part没有标记，则跳过
		if(!Parts::getInstance().part_is_draw(i)) continue;

		nothing = false;

		/// 开始绘制纹理
		glBindVertexArray(Parts::getInstance().get_vao_piying(i));
		Parts::getInstance().bind_texture(i);
		_ch_shader_program->setUniformValue("prescale", Parts::getInstance().get_prescale(i));
		glDrawElements(GL_TRIANGLES, (GLsizei)Parts::getInstance().get_part(i)->index_size(), GL_UNSIGNED_INT, 0);
	}

	if (nothing) {
		glBindVertexArray(0); ///< 解绑VAO
		return;
	}

	/// 获取选中的part
	Part* currentPart = TimelineGl::getInstance().get_current_part();
	glBindVertexArray(currentPart->vao_piying());

	/// 开始绘制选中part的边框
	_texture_tri_shader_program->bind();
	_texture_tri_shader_program->setUniformValue("is_skelen", false);
	_texture_tri_shader_program->setUniformValue("aColor", QVector4D(0.0f, 1.0f, 0.0f, 0.8f));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  ///< 把填充改成“线框”
	glDrawElements(GL_TRIANGLES, (GLsizei)currentPart->index_size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  ///< 恢复填充

	/// 解绑VAO
	glBindVertexArray(0);
}

void PiYingGL::draw_view_rectangle()
{
	glBindVertexArray(RECTANGLE_TEXTURE_VAO); ////////////////////////////////////////////////////

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

void PiYingGL::update_trc()
{
	makeCurrent();
	_ch_shader_program->bind();
	_ch_shader_program->setUniformValue("trc", getViewProjMatrix());
    _selected_vert_shader_program->bind();
	_selected_vert_shader_program->setUniformValue("trc", getViewProjMatrix());
    _texture_tri_shader_program->bind();
	_texture_tri_shader_program->setUniformValue("trc", getViewProjMatrix());
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

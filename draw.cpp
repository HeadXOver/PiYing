#include "piYingGL.h"
#include "piYing.h"
#include "image_texture.h"
#include "ch_element_tool.h"
#include "point_vector.h"
#include "point_vector_layer.h"

#include <qpainter>
#include <qopengltexture>
#include <QOpenGLShaderProgram.h>

void PiYingGL::drawChEditVert(int currentVector)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));

	PointVectorLayer pointVectorLayer(*(characterVerts[currentVector]));

	int index;
	int j;
	painter.setPen(QPen(Qt::black, 3));
	for (int i = 0; i < characterTriangleIndices[currentVector].size();) {
		QPolygonF poly;
		j = 0;
		for (; j < 3; j++) {
			index = characterTriangleIndices[currentVector][i + j];
			poly << glToMap(getViewProjMatrix().map(pointVectorLayer(index)));
		}
		painter.drawPolygon(poly);
		i += j;
	}

	painter.setPen(QPen(Qt::green, 1));
	for (int i = 0; i < characterTriangleIndices[currentVector].size();) {
		QPolygonF poly;
		j = 0;
		for (; j < 3; j++) {
			index = characterTriangleIndices[currentVector][i + j];
			poly << glToMap(getViewProjMatrix().map(pointVectorLayer(index)));
		}
		painter.drawPolygon(poly);
		i += j;
	}

	if (ch_element_tool_) ch_element_tool_->draw(painter);
}

void PiYingGL::draw_selected_points()
{
	glBindVertexArray(svVAO);
	glBindBuffer(GL_ARRAY_BUFFER, svVBO);
	_selected_vert_shader_program->bind();

	_selected_vert_shader_program->setUniformValue("trc", getViewProjMatrix());
	_selected_vert_shader_program->setUniformValue("is_out", true);
	glDrawArrays(GL_POINTS, 0, selected_points.size() / 2);
	_selected_vert_shader_program->setUniformValue("is_out", false);
	glDrawArrays(GL_POINTS, 0, selected_points.size() / 2);
	glBindVertexArray(0);
}

void PiYingGL::draw_ch_applied_vert()
{
	const int currentVector = getCurrentChRow();
	if (currentVector < 0) return;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));

	PointVectorLayer pointVectorLayer(*(characterVerts[currentVector]));

	int index;
	int j;
	painter.setPen(QPen(Qt::black, 3));
	for (int i = 0; i < characterTriangleIndices[currentVector].size();) {
		QPolygonF poly;
		j = 0;
		for (; j < 3; j++) {
			index = characterTriangleIndices[currentVector][i + j];
			poly << glToMap(getViewProjMatrix().map(pointVectorLayer[index]));
		}
		painter.drawPolygon(poly);
		i += j;
	}

	painter.setPen(QPen(Qt::green, 1));
	for (int i = 0; i < characterTriangleIndices[currentVector].size();) {
		QPolygonF poly;
		j = 0;
		for (; j < 3; j++) {
			index = characterTriangleIndices[currentVector][i + j];
			poly << glToMap(getViewProjMatrix().map(pointVectorLayer[index]));
		}
		painter.drawPolygon(poly);
		i += j;
	}

	if (ch_element_tool_) ch_element_tool_->draw(painter);
}

void PiYingGL::paintBackgrounds()
{
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
	int i = getCurrentChRow();
	if (i < 0) return;

	glBindVertexArray(bgVAO);////////////////////////////////////////////////////////

	bgShaderProgram->bind();

	characterTextures[i]->bind();

	bgShaderProgram->setUniformValue("trc", getViewProjMatrix());
	bgShaderProgram->setUniformValue("selected", false);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);////////////////////////////////////////////////////////////
	
	drawChEditVert(i);
}

void PiYingGL::paint_applied_texture()
{
	int i = getCurrentChRow();
	if (i < 0) return;

	glBindVertexArray(chVAO); ////////////////////////////////////////////////////

	chShaderProgram->bind();

	characterTextures[i]->bind();
	chShaderProgram->setUniformValue("trc", getViewProjMatrix());
	glDrawElements(GL_TRIANGLES, (GLsizei)characterTriangleIndices[i].size(), GL_UNSIGNED_INT, 0);

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

void PiYingGL::update_selected_verts()
{
	glBindBuffer(GL_ARRAY_BUFFER, svVBO);
	glBufferData(GL_ARRAY_BUFFER, selected_points.size() * sizeof(float), selected_points.data(), GL_DYNAMIC_DRAW);
}

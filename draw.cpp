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

	if (ch_element_tool_) ch_element_tool_->draw(&painter);
}

void PiYingGL::drawChSkeleVert()
{
	if (ch_tool_state_ != CharacterToolState::LibreSelectVert && ch_tool_state_ != CharacterToolState::RectSelectVert) return;

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

	if (ch_element_tool_) ch_element_tool_->draw(&painter);
}

void PiYingGL::drawChControlSlideVert()
{
	if (ch_tool_state_ != CharacterToolState::AddVertTrace) return;

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

	if (ch_element_tool_) ch_element_tool_->draw(&painter);
}

void PiYingGL::paintBackgrounds()
{
	glBindVertexArray(bgVAO);///////////////////////////////////////////////////////

	bgShaderProgram->bind();

	for (int i = 0; i < backGrounds.size(); i++) {
		ImageTexture* it = backGrounds[i];
		it->texture()->bind();

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

	characterTextures[i]->texture()->bind();

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

	glBindBuffer(GL_ARRAY_BUFFER, chVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);
	glBufferData(GL_ARRAY_BUFFER, characterVerts[i]->float_size() * sizeof(float), characterVerts[i]->data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, characterTriangleIndices[i].size() * sizeof(unsigned int), characterTriangleIndices[i].data(), GL_DYNAMIC_DRAW);

	characterTextures[i]->texture()->bind();
	chShaderProgram->setUniformValue("trc", getViewProjMatrix());
	glDrawElements(GL_TRIANGLES, (GLsizei)characterTriangleIndices[i].size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0); ////////////////////////////////////////////////////
}

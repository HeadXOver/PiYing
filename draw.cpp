#include "piYingGL.h"
#include "piYing.h"
#include "image_texture.h"
#include "ch_element_tool.h"
#include "point_vector.h"
#include "point_vector_layer.h"

#include <qpainter>
#include <qopengltexture>
#include <QOpenGLShaderProgram.h>

void PiYingGL::drawChEditVert()
{
	int currentVector = getCurrentChRow();
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

	if (chElementTool) chElementTool->draw(&painter);
}

void PiYingGL::drawChSkeleVert()
{
	if (ch_tool_state_ != CharacterToolState::LibreSelectSkelenVert && ch_tool_state_ != CharacterToolState::RectSelectSkelenVert) return;

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

	if (chElementTool) chElementTool->draw(&painter);
}

void PiYingGL::paintBackgrounds()
{
	glBindVertexArray(bgVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
	bgShaderProgram->bind();
	glActiveTexture(GL_TEXTURE0);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < backGrounds.size(); i++) {
		ImageTexture* it = backGrounds[i];
		it->texture()->bind();

		bgShaderProgram->setUniformValue("texture1", 0);
		bgShaderProgram->setUniformValue("trc", getBgShaderMatrix(it->transform()));
		if (i == getCurrentBgRow())bgShaderProgram->setUniformValue("selected", true);
		else bgShaderProgram->setUniformValue("selected", false);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PiYingGL::paintCharacterTexture()
{
	glBindVertexArray(bgVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
	bgShaderProgram->bind();
	glActiveTexture(GL_TEXTURE0);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int i = getCurrentChRow();
	if (i >= 0) {
		characterTextures[i]->texture()->bind();
		bgShaderProgram->setUniformValue("texture1", 0);
		bgShaderProgram->setUniformValue("trc", getViewProjMatrix());
		bgShaderProgram->setUniformValue("selected", false);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	drawChEditVert();
}

void PiYingGL::paintCharacterSkeleton()
{
	int i = getCurrentChRow();
	if (i < 0) return;

	glBindVertexArray(chVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);
	chShaderProgram->bind();
	glActiveTexture(GL_TEXTURE0);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	////////////////////////////

	glBufferData(GL_ARRAY_BUFFER, characterVerts[i]->float_size() * sizeof(float), characterVerts[i]->data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, characterTriangleIndices[i].size() * sizeof(unsigned int), characterTriangleIndices[i].data(), GL_DYNAMIC_DRAW);
	characterTextures[i]->texture()->bind();
	chShaderProgram->setUniformValue("texture1", 0);
	chShaderProgram->setUniformValue("trc", getViewProjMatrix());
	glDrawElements(GL_TRIANGLES, (GLsizei)characterTriangleIndices[i].size(), GL_UNSIGNED_INT, 0);

	//////////////////////////////

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	drawChSkeleVert();
}

#include "piYingGL.h"
#include "piYing.h"

void PiYingGL::drawChEditVert()
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	for (int i = 0; i < characterTriangleIndices.size();) {
		QPolygonF poly;
		int j = 0;
		for (; j < 3; j++) {
			QPointF p(characterVerts[characterTriangleIndices[i + j] * 2], characterVerts[characterTriangleIndices[i + j] * 2 + 1]);
			poly << glToMap(p);
			painter.setPen(QPen(Qt::black, 3));
			painter.drawPolygon(poly);
			painter.setPen(QPen(Qt::green, 1));
			painter.drawPolygon(poly);
		}
		i += j;
	}

	if (first2VertState == First2VertState::None) return;
	else if (first2VertState == First2VertState::Halfselect) {
		QPointF selectPoint(characterVerts[first2Index.first * 2], characterVerts[first2Index.first * 2 + 1]);
		selectPoint = glToMap(selectPoint);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(selectPoint);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(selectPoint);
	}
	else if (first2VertState == First2VertState::HalfPoint) {
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(first2Vert.first);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(first2Vert.first);
	}

	else if (first2VertState == First2VertState::Full2Point) {
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(first2Vert.first);
		painter.drawPoint(first2Vert.second);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(first2Vert.first);
		painter.drawPoint(first2Vert.second);
	}
	else if (first2VertState == First2VertState::Full2Select) {
		QPointF selectPoint(characterVerts[first2Index.first * 2], characterVerts[first2Index.first * 2 + 1]);
		selectPoint = glToMap(selectPoint);
		QPointF selectPoint2(characterVerts[first2Index.second * 2], characterVerts[first2Index.second * 2 + 1]);
		selectPoint2 = glToMap(selectPoint2);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(selectPoint);
		painter.drawPoint(selectPoint2);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(selectPoint);
		painter.drawPoint(selectPoint2);
	}
	else if (first2VertState == First2VertState::FullSelectPoint) {
		QPointF selectPoint(characterVerts[first2Index.first * 2], characterVerts[first2Index.first * 2 + 1]);
		selectPoint = glToMap(selectPoint);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(selectPoint);
		painter.drawPoint(first2Vert.first);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(selectPoint);
		painter.drawPoint(first2Vert.first);
	}
}

void PiYingGL::paintBackgrounds()
{
	glBindVertexArray(bgVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
	bgShaderProgram.bind();
	glActiveTexture(GL_TEXTURE0);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < backGrounds.size(); i++) {
		ImageTexture& it = backGrounds[i];
		it.tex->bind();

		bgShaderProgram.setUniformValue("texture1", 0);
		bgShaderProgram.setUniformValue("trc", getBgShaderMatrix(it.transform));
		if (i == currentSelectedBackGround)bgShaderProgram.setUniformValue("selected", true);
		else bgShaderProgram.setUniformValue("selected", false);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PiYingGL::paintCharactersOverView()
{
	glBindVertexArray(bgVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
	bgShaderProgram.bind();
	glActiveTexture(GL_TEXTURE0);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int i = parent->chImageList->currentRow();
	if (i >= 0) {
		characterTextures[i].tex->bind();
		bgShaderProgram.setUniformValue("texture1", 0);
		bgShaderProgram.setUniformValue("trc", QMatrix4x4());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	drawChEditVert();
}
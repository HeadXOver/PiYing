#include "piYingGL.h"
#include "piYing.h"

void PiYingGL::drawChEditVert()
{
	int currentVector = parent->chImageList->currentRow();
	if (currentVector < 0) return;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));

	for (int i = 0; i < characterTriangleIndices[currentVector].size();) {
		QPolygonF poly;
		int j = 0;
		for (; j < 3; j++) {
			int index = characterTriangleIndices[currentVector][i + j];
			QPointF p(characterVerts[currentVector][index * 2], characterVerts[currentVector][index * 2 + 1]);
			poly << glToMap(getViewProjMatrix().map(p));
		}
		painter.setPen(QPen(Qt::black, 3));
		painter.drawPolygon(poly);
		painter.setPen(QPen(Qt::green, 1));
		painter.drawPolygon(poly);
		i += j;
	}

	if (chToolState == ChToolState::AddTriangle) {
		if (chElementTool->getNumInd() == 0 && chElementTool->getNumVert() == 0) return;

		if (chElementTool->getNumInd() == 1 && chElementTool->getNumVert() == 0) {
			int index = chElementTool->getIndex(0) * 2;
			QPointF selectPoint(characterVerts[currentVector][index], characterVerts[currentVector][index + 1]);
			selectPoint = glToMap(getViewProjMatrix().map(selectPoint));
			painter.setPen(QPen(Qt::black, 8));
			painter.drawPoint(selectPoint);
			painter.setPen(QPen(Qt::red, 6));
			painter.drawPoint(selectPoint);
		}
		else if (chElementTool->getNumInd() == 0 && chElementTool->getNumVert() == 1) {
			painter.setPen(QPen(Qt::black, 8));
			painter.drawPoint(mapViewProjMatrix(chElementTool->getVert(0)));
			painter.setPen(QPen(Qt::red, 6));
			painter.drawPoint(mapViewProjMatrix(chElementTool->getVert(0)));
		}

		else if (chElementTool->getNumVert() == 2) {
			painter.setPen(QPen(Qt::black, 8));
			painter.drawPoint(mapViewProjMatrix(chElementTool->getVert(0)));
			painter.drawPoint(mapViewProjMatrix(chElementTool->getVert(1)));
			painter.setPen(QPen(Qt::red, 6));
			painter.drawPoint(mapViewProjMatrix(chElementTool->getVert(0)));
			painter.drawPoint(mapViewProjMatrix(chElementTool->getVert(1)));
		}
		else if (chElementTool->getNumInd() == 2) {
			int index = chElementTool->getIndex(0) * 2;
			QPointF selectPoint(characterVerts[currentVector][index], characterVerts[currentVector][index + 1]);
			selectPoint = glToMap(getViewProjMatrix().map(selectPoint));
			index = chElementTool->getIndex(1) * 2;
			QPointF selectPoint2(characterVerts[currentVector][index], characterVerts[currentVector][index + 1]);
			selectPoint2 = glToMap(getViewProjMatrix().map(selectPoint2));
			painter.setPen(QPen(Qt::black, 8));
			painter.drawPoint(selectPoint);
			painter.drawPoint(selectPoint2);
			painter.setPen(QPen(Qt::red, 6));
			painter.drawPoint(selectPoint);
			painter.drawPoint(selectPoint2);
		}
		else if (chElementTool->getNumInd() == 1 && chElementTool->getNumVert() == 1) {
			int index = chElementTool->getIndex(0) * 2;
			QPointF selectPoint(characterVerts[currentVector][index], characterVerts[currentVector][index + 1]);
			selectPoint = glToMap(getViewProjMatrix().map(selectPoint));
			painter.setPen(QPen(Qt::black, 8));
			painter.drawPoint(selectPoint);
			painter.drawPoint(mapViewProjMatrix(chElementTool->getVert(0)));
			painter.setPen(QPen(Qt::red, 6));
			painter.drawPoint(selectPoint);
			painter.drawPoint(mapViewProjMatrix(chElementTool->getVert(0)));
		}
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

void PiYingGL::paintCharacterTexture()
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
		bgShaderProgram.setUniformValue("trc", getViewProjMatrix());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	drawChEditVert();
}

void PiYingGL::paintCharacterSkeleton()
{
	glBindVertexArray(chVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);
	chShaderProgram.bind();
	glActiveTexture(GL_TEXTURE0);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int i = parent->chImageList->currentRow();
	if (i >= 0) {
		glBufferData(GL_ARRAY_BUFFER, characterVerts[i].size() * sizeof(float), characterVerts[i].data(), GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, characterTriangleIndices[i].size() * sizeof(unsigned int), characterTriangleIndices[i].data(), GL_DYNAMIC_DRAW);
		characterTextures[i].tex->bind();
		chShaderProgram.setUniformValue("texture1", 0);
		chShaderProgram.setUniformValue("trc", getViewProjMatrix());
		glDrawElements(GL_TRIANGLES, (GLsizei)characterTriangleIndices[i].size(), GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

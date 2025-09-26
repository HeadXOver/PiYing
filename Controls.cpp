#include "piYingGL.h"
#include "piYing.h"

void PiYingGL::bgRotationControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::CursorShape::ClosedHandCursor);
	QPointF center = getBgShaderMatrix(lastImageTransform).map(QPointF(0.f, 0.f));
	QPointF vec1 = insProj.map(lastMousePos - center);
	QPointF vec2 = insProj.map(mouse - center);
	image.transform = lastImageTransform;
	image.addRot(angleBetweenPointDegree(vec2, vec1));
}

void PiYingGL::bgTranslateControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::ClosedHandCursor);
	image.transform = lastImageTransform;
	image.addTrans((getViewMatrixInvertWithoutTrans() * insProj).map(mouse - lastMousePos));
}

void PiYingGL::bgScaleControl(const QPointF& mouse, ImageTexture& image)
{
	QPointF mouseRaletive = getRaletiveToRect(mouse, lastImageTransform);
	QPointF LastMouseRaletive = getRaletiveToRect(lastMousePos, lastImageTransform);
	QPointF pAspect(1.0f, 1.0f);
	short PN[2] = { 1, 1 };

	image.transform = lastImageTransform;

	if (lastMousePosType == MousePos::BottomEdge || lastMousePosType == MousePos::LeftBottom || lastMousePosType == MousePos::RightBottom) {
		pAspect.setY((1.0f - mouseRaletive.y()) / (1.0f - LastMouseRaletive.y()));
	}
	else if (lastMousePosType == MousePos::TopEdge || lastMousePosType == MousePos::LeftTop || lastMousePosType == MousePos::RightTop) {
		pAspect.setY((1.0f + mouseRaletive.y()) / (1.0f + LastMouseRaletive.y()));
		PN[0] = -1;
	}
	if (lastMousePosType == MousePos::LeftEdge || lastMousePosType == MousePos::LeftTop || lastMousePosType == MousePos::LeftBottom) {
		pAspect.setX((1.0f - mouseRaletive.x()) / (1.0f - LastMouseRaletive.x()));
	}
	else if (lastMousePosType == MousePos::RightEdge || lastMousePosType == MousePos::RightBottom || lastMousePosType == MousePos::RightTop) {
		pAspect.setX((1.0f + mouseRaletive.x()) / (1.0f + LastMouseRaletive.x()));
		PN[1] = -1;
	}
	if (KeyboardStateWin::isShiftHeld()) {
		pAspect.setY(pAspect.x());
	}
	image.addScale(pAspect);
	pAspect.setX(PN[1] * (1.f - pAspect.x()));
	pAspect.setY(PN[0] * (1.f - pAspect.y()));
	image.addTrans((lastImageTransform.rot * lastImageTransform.scale * insProj).map(pAspect));
}

void PiYingGL::viewRotationControl(const QPointF& mouse)
{
	setCursor(Qt::CursorShape::ClosedHandCursor);
	float r = angleBetweenPoint(insProj.map(mouse), insProj.map(lastMiddleButtonPos));
	viewRotate.setValue(lastViewRotate + r * 180.f / 3.1415926f);
	viewTransX.setValue(lastViewTransX * cos(r) - lastViewTransY * sin(r));
	viewTransY.setValue(lastViewTransX * sin(r) + lastViewTransY * cos(r));
}

void PiYingGL::chAddTriangleControl(const QPointF& origMouse)
{
	int currentVector = parent->chImageList->currentRow();
	if (currentVector < 0) return;

	QPointF mouse = glToMap(getViewProjMatrixInvert().map(mapToGL(origMouse)));

	if (chElementTool->getNumInd() + chElementTool->getNumVert() <= 1) {
		if (chElementTool->checkPointRepeat(mouse))  return;

		for (unsigned int i = 0; i < characterVerts[currentVector].size() / 2; i++) {
			QPointF readyPoint(characterVerts[currentVector][i + i], characterVerts[currentVector][i + i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				if (chElementTool->addIndex(i)) return;
			}
		}

		chElementTool->addVert(mouse);
	}
	else if (chElementTool->getNumVert() == 2) {
		if (chElementTool->checkPointRepeat(mouse))  return;

		for (unsigned int i = 0; i < characterVerts[currentVector].size() / 2; i++) {
			QPointF readyPoint(characterVerts[currentVector][i + i], characterVerts[currentVector][i + i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				chElementTool->setNumInd(0);
				chElementTool->setNumVert(0);
				characterTriangleIndices[currentVector].push_back(i);
				addChVert(mapToGL(chElementTool->getVert(0)), currentVector);
				addChVert(mapToGL(chElementTool->getVert(1)), currentVector);
				return;
			}
		}

		chElementTool->setNumInd(0);
		chElementTool->setNumVert(0);
		addChVert(mapToGL(chElementTool->getVert(0)), currentVector);
		addChVert(mapToGL(chElementTool->getVert(1)), currentVector);
		addChVert(mapToGL(mouse), currentVector);
	}
	else if (chElementTool->getNumInd() == 2) {
		if (chElementTool->checkPointRepeat(mouse))  return;

		for (unsigned int i = 0; i < characterVerts[currentVector].size() / 2; i++) {
			QPointF readyPoint(characterVerts[currentVector][i + i], characterVerts[currentVector][i + i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				if (i == chElementTool->getIndex(0) && i == chElementTool->getIndex(1)) return;
				for (int j = 0; j < characterTriangleIndices[currentVector].size(); j += 3) {
					unsigned int x[3] = { characterTriangleIndices[currentVector][j + 0], characterTriangleIndices[currentVector][j + 1], characterTriangleIndices[currentVector][j + 2] };
					unsigned int y[3] = { i, chElementTool->getIndex(0), chElementTool->getIndex(1) };
					std::sort(x, x + 3);
					std::sort(y, y + 3);
					if (x[0] == y[0] && x[1] == y[1] && x[2] == y[2]) 
						return;
				}

				chElementTool->setNumInd(0);
				chElementTool->setNumVert(0);
				characterTriangleIndices[currentVector].push_back(chElementTool->getIndex(0));
				characterTriangleIndices[currentVector].push_back(chElementTool->getIndex(1));
				characterTriangleIndices[currentVector].push_back(i);

				return;
			}
		}

		chElementTool->setNumInd(0);
		chElementTool->setNumVert(0);
		addChVert(getViewProjMatrixInvert().map(lastMousePos), currentVector);
		characterTriangleIndices[currentVector].push_back(chElementTool->getIndex(0));
		characterTriangleIndices[currentVector].push_back(chElementTool->getIndex(1));
	}
	else if (chElementTool->getNumInd() == 1 && chElementTool->getNumVert() == 1) {
		if (chElementTool->checkPointRepeat(mouse))  return;

		for (unsigned int i = 0; i < characterVerts[currentVector].size() / 2; i++) {
			QPointF readyPoint(characterVerts[currentVector][i + i], characterVerts[currentVector][i + i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				if (chElementTool->getIndex(0) != i) {
					chElementTool->setNumInd(0);
					chElementTool->setNumVert(0);
					characterTriangleIndices[currentVector].push_back(i);
					characterTriangleIndices[currentVector].push_back(chElementTool->getIndex(0));
					addChVert(mapToGL(chElementTool->getVert(0)), currentVector);
				}
				return;
			}
		}

		chElementTool->setNumInd(0);
		chElementTool->setNumVert(0);
		characterTriangleIndices[currentVector].push_back(chElementTool->getIndex(0));
		addChVert(mapToGL(mouse), currentVector);
		addChVert(mapToGL(chElementTool->getVert(0)), currentVector);
	}
}
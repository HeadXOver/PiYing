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

void PiYingGL::chEditVertControl(const QPointF& origMouse)
{
	int currentVector = parent->chImageList->currentRow();
	if (currentVector < 0) return;

	QPointF mouse = glToMap(getViewProjMatrixInvert().map(mapToGL(origMouse)));
	if (first2VertState == First2VertState::None) {
		for (int i = 0; i < characterVerts[currentVector].size(); i += 2) {
			QPointF readyPoint(characterVerts[currentVector][i], characterVerts[currentVector][i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				first2VertState = First2VertState::HalfSelect;
				first2Index.first = (unsigned int)i / 2;
				return;
			}
		}

		first2VertState = First2VertState::HalfPoint;
		first2Vert.first = mouse;
	}
	else if (first2VertState == First2VertState::HalfSelect) {
		for (int i = 0; i < characterVerts[currentVector].size(); i += 2) {
			QPointF readyPoint(characterVerts[currentVector][i], characterVerts[currentVector][i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				if (first2Index.first != i / 2) {
					first2VertState = First2VertState::Full2Select;
					first2Index.second = (unsigned int)i / 2;
				}
				return;
			}
		}

		first2VertState = First2VertState::FullSelectPoint;
		first2Vert.first = mouse;
	}
	else if (first2VertState == First2VertState::HalfPoint) {
		if (QLineF(first2Vert.first, mouse).length() < 6)  return;

		for (int i = 0; i < characterVerts[currentVector].size(); i += 2) {
			QPointF readyPoint(characterVerts[currentVector][i], characterVerts[currentVector][i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				first2VertState = First2VertState::FullPointSelect;
				first2Index.first = (unsigned int)i / 2;
				return;
			}
		}

		first2VertState = First2VertState::Full2Point;
		first2Vert.second = mouse;
	}
	else if (first2VertState == First2VertState::Full2Point) {
		if (QLineF(first2Vert.first, mouse).length() < 6 || QLineF(first2Vert.second, mouse).length() < 6)  return;

		for (int i = 0; i < characterVerts[currentVector].size(); i += 2) {
			QPointF readyPoint(characterVerts[currentVector][i], characterVerts[currentVector][i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				first2VertState = First2VertState::None;
				characterTriangleIndices[currentVector].push_back((unsigned int)i / 2);
				addChVert(mapToGL(first2Vert.first), currentVector);
				addChVert(mapToGL(first2Vert.second), currentVector);
				return;
			}
		}

		first2VertState = First2VertState::None;
		addChVert(mapToGL(first2Vert.first), currentVector);
		addChVert(mapToGL(first2Vert.second), currentVector);
		addChVert(getViewProjMatrixInvert().map(lastMousePos), currentVector);
	}
	else if (first2VertState == First2VertState::Full2Select) {
		for (unsigned int i = 0; i < characterVerts[currentVector].size(); i += 2) {
			QPointF readyPoint(characterVerts[currentVector][i], characterVerts[currentVector][i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				if (i / 2 == first2Index.first && i / 2 == first2Index.second) return;
				for (int j = 0; j < characterTriangleIndices[currentVector].size(); j += 3) {
					unsigned int x[3] = { characterTriangleIndices[currentVector][j + 0], characterTriangleIndices[currentVector][j + 1], characterTriangleIndices[currentVector][j + 2] };
					unsigned int y[3] = { i / 2, first2Index.first, first2Index.second };
					std::sort(x, x + 3);
					std::sort(y, y + 3);
					if (x[0] == y[0] && x[1] == y[1] && x[2] == y[2]) 
						return;
				}

				first2VertState = First2VertState::None;
				characterTriangleIndices[currentVector].push_back(first2Index.first);
				characterTriangleIndices[currentVector].push_back(first2Index.second);
				characterTriangleIndices[currentVector].push_back(i / 2);

				return;
			}
		}

		first2VertState = First2VertState::None;
		addChVert(getViewProjMatrixInvert().map(lastMousePos), currentVector);
		characterTriangleIndices[currentVector].push_back(first2Index.first);
		characterTriangleIndices[currentVector].push_back(first2Index.second);
	}
	else if (first2VertState == First2VertState::FullSelectPoint || first2VertState == First2VertState::FullPointSelect) {
		if (QLineF(first2Vert.first, mouse).length() < 6)  return;

		for (int i = 0; i < characterVerts[currentVector].size(); i += 2) {
			QPointF readyPoint(characterVerts[currentVector][i], characterVerts[currentVector][i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				if (first2Index.first != i / 2) {
					first2VertState = First2VertState::None;
					characterTriangleIndices[currentVector].push_back((unsigned int)i / 2);
					characterTriangleIndices[currentVector].push_back(first2Index.first);
					addChVert(mapToGL(first2Vert.first), currentVector);
				}
				return;
			}
		}

		first2VertState = First2VertState::None;
		characterTriangleIndices[currentVector].push_back(first2Index.first);
		addChVert(getViewProjMatrixInvert().map(lastMousePos), currentVector);
		addChVert(mapToGL(first2Vert.first), currentVector);
	}
}
#include "piYingGL.h"

void PiYingGL::bgRotationControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::CursorShape::ClosedHandCursor);
	QPointF center = (getViewMatrix() * lastImageTransform.trans).map(QPointF(0.f, 0.f));
	center.setX(center.x() / aspect);
	QPointF vec1 = lastMousePos - center;
	QPointF vec2 = mouse - center;
	float r = (atan2f(vec2.y(), vec2.x()) - atan2f(vec1.y(), vec1.x())) * 180.f / 3.1415926f;

	image.transform = lastImageTransform;
	image.addRot(r);
}

void PiYingGL::bgTranslateControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::ClosedHandCursor);
	image.transform = lastImageTransform;
	image.addTrans((getViewMatrixInvertWithoutTrans() * insProj).map(QPointF(mouse.x() - lastMousePos.x(), mouse.y() - lastMousePos.y())));
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
	float r = (atan2f(mouse.y(), mouse.x()) - atan2f(lastMiddleButtonPos.y(), lastMiddleButtonPos.x())) * 180.f / 3.1415926f;
	viewRotate.setValue(lastViewRotate + r);

	QMatrix4x4 toTrans;
	toTrans.rotate(r, 0.f, 0.0f, 1.0f);
	QPointF viewTrans(lastViewTransX, lastViewTransY);
	QPointF rotatedTrans = toTrans.map(viewTrans) - viewTrans;
	viewTransX.setValue(lastViewTransX + rotatedTrans.x());
	viewTransY.setValue(lastViewTransY + rotatedTrans.y());
}

void PiYingGL::chEditVertControl(const QPointF& origMouse)
{
	QPointF mouse = glToMap(getViewProjMatrixInvert().map(mapToGL(origMouse)));
	if (first2VertState == First2VertState::None) {
		for (int i = 0; i < characterVerts.size(); i += 2) {
			QPointF readyPoint(characterVerts[i], characterVerts[i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				first2VertState = First2VertState::HalfSelect;
				first2Index.first = (unsigned int)i / 2;
				return;
			}
		}

		// 如果与已有的点不重复
		first2VertState = First2VertState::HalfPoint;
		first2Vert.first = mouse;
		return;
	}
	else if (first2VertState == First2VertState::HalfSelect) {
		for (int i = 0; i < characterVerts.size(); i += 2) {
			QPointF readyPoint(characterVerts[i], characterVerts[i + 1]);
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
		return;
	}
	else if (first2VertState == First2VertState::HalfPoint) {
		if (QLineF(first2Vert.first, mouse).length() < 6)  return;

		for (int i = 0; i < characterVerts.size(); i += 2) {
			QPointF readyPoint(characterVerts[i], characterVerts[i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				first2VertState = First2VertState::FullPointSelect;
				first2Index.first = (unsigned int)i / 2;
				return;
			}
		}

		first2VertState = First2VertState::Full2Point;
		first2Vert.second = mouse;
		return;
	}
	else if (first2VertState == First2VertState::Full2Point) {
		if (QLineF(first2Vert.first, mouse).length() < 6 || QLineF(first2Vert.second, mouse).length() < 6)  return;

		for (int i = 0; i < characterVerts.size(); i += 2) {
			QPointF readyPoint(characterVerts[i], characterVerts[i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				first2VertState = First2VertState::None;
				characterTriangleIndices.push_back((unsigned int)i / 2);
				addChVert(mapToGL(first2Vert.first));
				addChVert(mapToGL(first2Vert.second));
				return;
			}
		}

		first2VertState = First2VertState::None;
		addChVert(mapToGL(first2Vert.first));
		addChVert(mapToGL(first2Vert.second));
		addChVert(getViewProjMatrixInvert().map(lastMousePos));
		return;
	}
	else if (first2VertState == First2VertState::Full2Select) {
		for (unsigned int i = 0; i < characterVerts.size(); i += 2) {
			QPointF readyPoint(characterVerts[i], characterVerts[i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				if (i / 2 == first2Index.first && i / 2 == first2Index.second) return;
				for (int j = 0; j < characterTriangleIndices.size(); j += 3) {
					unsigned int x[3] = { characterTriangleIndices[j + 0], characterTriangleIndices[j + 1], characterTriangleIndices[j + 2] };
					unsigned int y[3] = { i / 2, first2Index.first, first2Index.second };
					std::sort(x, x + 3);
					std::sort(y, y + 3);
					if (x[0] == y[0] && x[1] == y[1] && x[2] == y[2]) 
						return;
				}

				first2VertState = First2VertState::None;
				characterTriangleIndices.push_back(first2Index.first);
				characterTriangleIndices.push_back(first2Index.second);
				characterTriangleIndices.push_back(i / 2);

				return;
			}
		}

		first2VertState = First2VertState::None;
		addChVert(getViewProjMatrixInvert().map(lastMousePos));
		characterTriangleIndices.push_back(first2Index.first);
		characterTriangleIndices.push_back(first2Index.second);
		return;
	}
	else if (first2VertState == First2VertState::FullSelectPoint || first2VertState == First2VertState::FullPointSelect) {
		if (QLineF(first2Vert.first, mouse).length() < 6)  return;

		for (int i = 0; i < characterVerts.size(); i += 2) {
			QPointF readyPoint(characterVerts[i], characterVerts[i + 1]);
			readyPoint = glToMap(readyPoint);
			if (QLineF(readyPoint, mouse).length() < 6) {
				if (first2Index.first != i / 2) {
					first2VertState = First2VertState::None;
					characterTriangleIndices.push_back((unsigned int)i / 2);
					characterTriangleIndices.push_back(first2Index.first); 
					addChVert(mapToGL(first2Vert.first));
				}
				return;
			}
		}

		first2VertState = First2VertState::None;
		characterTriangleIndices.push_back(first2Index.first);
		addChVert(getViewProjMatrixInvert().map(lastMousePos));
		addChVert(mapToGL(first2Vert.first));
		return;
	}
}
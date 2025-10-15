#include "ChElementSelect.h"
#include "glVertReference.h"
#include "piYingGL.h"
#include "SelectedPoints.h"
#include "static_handle_zone.h"
#include "point_vector.h"
#include "point_vector_layer.h"

#include "cus_func_zone.h"
#include "cus_func_point2d.h"

#include <qpainter>

ChElementSelect::ChElementSelect(GlVertReference* glReference) : glVertReference(glReference)
{
    selectedPoints = new SelectedPoints(false, *(glVertReference->pointLayer));
}

ChElementSelect::~ChElementSelect()
{
    delete selectedPoints;
}

void ChElementSelect::escape() 
{ 
    if (selectedPoints->size() > 0) selectedPoints->removeLast(); 
}

void ChElementSelect::deleteElement()
{
    std::vector<unsigned int>& idx = glVertReference->glIndex;
    PointVectorLayer& pointLayer = *(glVertReference->pointLayer);
    const size_t nVert = pointLayer.size();
    const size_t nTri = idx.size() / 3;

    std::vector<bool> killVert(nVert, false);
    std::vector<bool> killTri(nTri, false);
    for (unsigned v : selectedPoints->index()) killVert[v] = true;

    std::vector<unsigned> refCount(nVert, 0);
    for (size_t t = 0; t < nTri; ++t) {
        unsigned v0 = idx[t * 3], v1 = idx[t * 3 + 1], v2 = idx[t * 3 + 2];
        if (killVert[v0] || killVert[v1] || killVert[v2]) {
            killTri[t] = true;
        }
        else {
            refCount[v0]++;
            refCount[v1]++;
            refCount[v2]++;
        }
    }

    for (size_t t = 0; t < nTri; ++t) {
        unsigned v0 = idx[t * 3], v1 = idx[t * 3 + 1], v2 = idx[t * 3 + 2];
        if (killTri[t] == true) {
            if (!killVert[v1] && refCount[v1] == 0) killVert[v1] = true;
            if (!killVert[v2] && refCount[v2] == 0) killVert[v2] = true;
            if (!killVert[v0] && refCount[v0] == 0) killVert[v0] = true;
        }
    }

    std::vector<unsigned> old2new(nVert, UINT_MAX);
    unsigned newVertCount = 0;
    for (unsigned old = 0; old < nVert; ++old) {
        if (!killVert[old]) {
            old2new[old] = newVertCount++;
            pointLayer.copy_from_to(old, newVertCount - 1);
        }
    }
    if (newVertCount == 0) {
        pointLayer.clear();
    }
    else {
        pointLayer.resize(newVertCount);
    }

    size_t outIdx = 0;
    for (size_t t = 0; t < nTri; ++t) {
        if (killTri[t]) continue;
        unsigned v0 = idx[t * 3], v1 = idx[t * 3 + 1], v2 = idx[t * 3 + 2];
        idx[outIdx++] = old2new[v0];
        idx[outIdx++] = old2new[v1];
        idx[outIdx++] = old2new[v2];
    }
    if (outIdx != 0) idx.resize(outIdx);
    else idx.clear();

    selectedPoints->clear();
}

void ChElementSelect::drawHandle(QPainter* painter)
{
    if(selectedPoints->size() == 0) return;

    painter->setBrush(QColor(0, 0, 0, 0));

    // 计算中心点
    handleCenterPoint = QPointF();
    for (unsigned int i : selectedPoints->index()) handleCenterPoint += glVertReference->pointLayer->get_uv_point(i);
    
    handleCenterPoint /= selectedPoints->size();

    dHandleCenterPoint = glVertReference->gl->mapViewProjMatrix(handleCenterPoint);

	// 绘制圆
    painter->setPen(QPen(Qt::black, 4));
	painter->drawEllipse(dHandleCenterPoint, ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS);
    painter->setPen(QPen(Qt::yellow, 2));
    painter->drawEllipse(dHandleCenterPoint, ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS);

    // 绘制移动控制柄
	painter->setPen(QPen(Qt::black, 6));
    painter->drawLine(dHandleCenterPoint.x() - MOVEHANDLE_LENTH, dHandleCenterPoint.y(), dHandleCenterPoint.x() + MOVEHANDLE_LENTH, dHandleCenterPoint.y());
    painter->drawLine(dHandleCenterPoint.x(), dHandleCenterPoint.y() - MOVEHANDLE_LENTH, dHandleCenterPoint.x(), dHandleCenterPoint.y() + MOVEHANDLE_LENTH);
    painter->setPen(QPen(Qt::green, 4));
    painter->drawLine(dHandleCenterPoint.x() - MOVEHANDLE_LENTH, dHandleCenterPoint.y(), dHandleCenterPoint.x() + MOVEHANDLE_LENTH, dHandleCenterPoint.y());
    painter->setPen(QPen(Qt::red, 4));
    painter->drawLine(dHandleCenterPoint.x(), dHandleCenterPoint.y() - MOVEHANDLE_LENTH, dHandleCenterPoint.x(), dHandleCenterPoint.y() + MOVEHANDLE_LENTH);

    // 绘制中心点
    painter->setPen(QPen(Qt::black, 12));
    painter->drawPoint(dHandleCenterPoint);
    painter->setPen(QPen(Qt::yellow, HANDLE_ZONE));
    painter->drawPoint(dHandleCenterPoint);

	// 绘制缩放控制柄
    painter->setPen(QPen(Qt::black, 12));
    painter->drawPoint(dHandleCenterPoint.x(), dHandleCenterPoint.y() + SCALEHANDLE_DISTANCE);
    painter->drawPoint(dHandleCenterPoint.x() + SCALEHANDLE_DISTANCE, dHandleCenterPoint.y());
    painter->drawPoint(dHandleCenterPoint.x() + ROTATEHANDLE_RADIUS, dHandleCenterPoint.y() + ROTATEHANDLE_RADIUS);
    painter->setPen(QPen(Qt::yellow, HANDLE_ZONE));
    painter->drawPoint(dHandleCenterPoint.x(), dHandleCenterPoint.y() + SCALEHANDLE_DISTANCE);
    painter->drawPoint(dHandleCenterPoint.x() + SCALEHANDLE_DISTANCE, dHandleCenterPoint.y());
    painter->drawPoint(dHandleCenterPoint.x() + ROTATEHANDLE_RADIUS, dHandleCenterPoint.y() + ROTATEHANDLE_RADIUS);

}

void ChElementSelect::changeEditMode()
{
    if (selectedPoints->size() == 0) {
        editMode = ChElementEditMode::None;
        return;
    }

    qreal length = QLineF(dHandleCenterPoint, lastPos).length();

    if (length <= ROTATEHANDLE_RADIUS + HANDLE_ZONE && length >= ROTATEHANDLE_RADIUS - HANDLE_ZONE)                 editMode = ChElementEditMode::Rotate;   // 判断是否在旋转控制柄上
    else if (isInRect(lastPos, dHandleCenterPoint, HANDLE_ZONE))                                                     editMode = ChElementEditMode::Move;     // 判断是否在中心点上
    else if (isInRect(lastPos, dHandleCenterPoint, MOVEHANDLE_LENTH * 2, HANDLE_ZONE))                               editMode = ChElementEditMode::MoveX;    // 判断是否在移动控制柄X上
    else if (isInRect(lastPos, dHandleCenterPoint, HANDLE_ZONE, MOVEHANDLE_LENTH * 2))                               editMode = ChElementEditMode::MoveY;    // 判断是否在移动控制柄Y上
    else if (isInRect(lastPos, dHandleCenterPoint + QPoint(ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS), HANDLE_ZONE))  editMode = ChElementEditMode::Scale;    // 判断是否在缩放控制柄上
    else if (isInRect(lastPos, dHandleCenterPoint + QPoint(SCALEHANDLE_DISTANCE, 0), HANDLE_ZONE))                   editMode = ChElementEditMode::ScaleX;   // 判断是否在缩放控制柄X上
    else if (isInRect(lastPos, dHandleCenterPoint + QPoint(0, SCALEHANDLE_DISTANCE), HANDLE_ZONE))                   editMode = ChElementEditMode::ScaleY;   // 判断是否在缩放控制柄Y上
	else                                                                                                            editMode = ChElementEditMode::None;     // 不在任何控制柄上
}

void ChElementSelect::moveHandle(const QPointF& mouse)
{
    if (editMode == ChElementEditMode::None) return;

    PointVectorLayer& pointLayer = *(glVertReference->pointLayer);
    if (editMode == ChElementEditMode::Move) {
        QPointF toMove = glVertReference->gl->GLViewProjMatrixInvert(mouse) - glVertReference->gl->GLViewProjMatrixInvert(lastPos);
        for (int i = 0;i < selectedPoints->size();i++) {
            pointLayer.set_point(false, (*selectedPoints)[i], selectedPoints->getVert(i) + toMove);
        }
    }
    else if(editMode == ChElementEditMode::MoveX) {
        QPointF toMove = glVertReference->gl->GLViewProjMatrixInvert(mouse.x(), 0.f) - glVertReference->gl->GLViewProjMatrixInvert(lastPos.x(), 0.f);
        for (int i = 0; i < selectedPoints->size(); i++) {
            pointLayer.set_point(false, (*selectedPoints)[i], selectedPoints->getVert(i) + toMove);
        }
    }
    else if (editMode == ChElementEditMode::MoveY) {
        QPointF toMove = glVertReference->gl->GLViewProjMatrixInvert(0.f, mouse.y()) - glVertReference->gl->GLViewProjMatrixInvert(0.f, lastPos.y());
        for (int i = 0; i < selectedPoints->size(); i++) {
            pointLayer.set_point(false, (*selectedPoints)[i], selectedPoints->getVert(i) + toMove);
        }
    }
    else if (editMode == ChElementEditMode::Rotate) {
        float angle = angleBetweenPoint(lastPos - lastDHandleCenterPoint, mouse - lastDHandleCenterPoint);
        QPointF toRot = glVertReference->gl->getInsProj().map(lastHandleCenterPoint);
        for (int i = 0; i < selectedPoints->size(); i++) {
            pointLayer.set_point(false, (*selectedPoints)[i], toRot + getRotatedPoint(glVertReference->gl->getInsProj().map(selectedPoints->getVert(i) - lastHandleCenterPoint), angle));
            pointLayer.set_point(false, (*selectedPoints)[i], glVertReference->gl->getProj().map(glVertReference->pointLayer->get_uv_point((*selectedPoints)[i])));
        }
    }
    else if (editMode == ChElementEditMode::Scale) {
        float scale = (mouse.x() + mouse.y() - dHandleCenterPoint.x() - dHandleCenterPoint.y()) / (ROTATEHANDLE_RADIUS + ROTATEHANDLE_RADIUS);
        QPointF toScale = lastHandleCenterPoint * (scale - 1);
        for (int i = 0; i < selectedPoints->size(); i++) {
            pointLayer.set_point(false, (*selectedPoints)[i], selectedPoints->getVert(i) * scale - toScale);
        }
    }
    else if (editMode == ChElementEditMode::ScaleX) {
        float scale = (mouse.x() - lastDHandleCenterPoint.x()) / ROTATEHANDLE_RADIUS;
        float scaleX = lastDHandleCenterPoint.x() * (1 - scale);
        for (int i = 0; i < selectedPoints->size(); i++) {
            QPointF mapOri = glVertReference->gl->mapViewProjMatrix(selectedPoints->getVert(i));
            pointLayer.set_point(false, (*selectedPoints)[i], glVertReference->gl->GLViewProjMatrixInvert(mapOri.x() * scale + scaleX, mapOri.y()));
        }
    }
    else if (editMode == ChElementEditMode::ScaleY) {
        float scale = (mouse.y() - lastDHandleCenterPoint.y()) / ROTATEHANDLE_RADIUS;
        float scaleY = lastDHandleCenterPoint.y() * (1 - scale);
        for (int i = 0; i < selectedPoints->size(); i++) {
            QPointF mapOri = glVertReference->gl->mapViewProjMatrix(selectedPoints->getVert(i));
            pointLayer.set_point(false, (*selectedPoints)[i], glVertReference->gl->GLViewProjMatrixInvert(mapOri.x(), mapOri.y() * scale + scaleY));
        }
	}
}

void ChElementSelect::affirmHandle()
{
    selectedPoints->affirmVert();
    lastHandleCenterPoint = handleCenterPoint;
    lastDHandleCenterPoint = dHandleCenterPoint;
}

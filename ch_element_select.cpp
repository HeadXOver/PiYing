#include "ch_element_select.h"
#include "piYingGL.h"
#include "SelectedPoints.h"
#include "static_handle_zone.h"
#include "point_vector_layer.h"
#include "KeyboardStateWin.h"

#include "enum_select_handle_mode.h"
#include "enum_edit_mode.h"
#include "cus_func_zone.h"
#include "cus_func_point2d.h"

#include <qpainter>

ChElementSelect::ChElementSelect() : 
    edit_skelen(PiYingGL::getInstance().editMode == EditMode::characterSkeleton),
    editMode(ChElementEditMode::None)
{
    selected_points = std::make_unique<SelectedPoints>(false, *PiYingGL::getInstance().currentLayer());
}

ChElementSelect::~ChElementSelect()
{
}

void ChElementSelect::escape() 
{ 
    if (selected_points->size() > 0) selected_points->removeLast(); 
}

void ChElementSelect::deleteElement()
{
    std::vector<unsigned int>& idx = *PiYingGL::getInstance().currentIndex();
    const size_t nVert = PiYingGL::getInstance().currentLayer()->size();
    const size_t nTri = idx.size() / 3;

    std::vector<bool> killVert(nVert, false);
    std::vector<bool> killTri(nTri, false);
    for (unsigned v : selected_points->index()) killVert[v] = true;

    std::vector<unsigned> refCount(nVert, 0);
    for (size_t t = 0; t < nTri; ++t) {
        unsigned v0 = idx[t * 3], v1 = idx[t * 3 + 1], v2 = idx[t * 3 + 2];
        if (killVert[v0] || killVert[v1] || killVert[v2]) {
            killTri[t] = true;
            continue;
        }

        refCount[v0]++;
        refCount[v1]++;
        refCount[v2]++;
    }

    for (size_t t = 0; t < nTri; ++t) {
        if (!killTri[t]) continue;

        const size_t t3 = t * 3;

        for (int i = 0; i < 3; ++i) {
            const unsigned v = idx[t3 + i];
            killVert[v] = (refCount[v] == 0);
        }
    }

    std::vector<unsigned> old2new(nVert, UINT_MAX);
    unsigned newVertCount = 0;
    for (unsigned old = 0; old < nVert; ++old) {
        if (killVert[old]) continue;

        PiYingGL::getInstance().currentLayer()->copy_from_to(old, newVertCount);
        old2new[old] = newVertCount++;
    }

    PiYingGL::getInstance().currentLayer()->resize(newVertCount);

    size_t outIdx = 0;
    for (size_t t = 0; t < nTri; ++t) {
        if (killTri[t]) continue;

        const size_t t3 = t * 3;

        for (int i = 0; i < 3; ++i) idx[outIdx++] = old2new[idx[t3 + i]];
    }

    idx.resize(outIdx);

    selected_points->clear();
    PiYingGL::getInstance().update_ch_verts();
}

void ChElementSelect::draw_handle_and_selected()
{
    if(selected_points->size() == 0) return;

    // 计算中心点
    PointVectorLayer& pointLayer = *PiYingGL::getInstance().currentLayer();
    handleCenterPoint = QPointF();
    for (unsigned int i : selected_points->index()) {
        handleCenterPoint += edit_skelen ? pointLayer[i] : pointLayer(i);
    }
    
    handleCenterPoint /= selected_points->size();

    dHandleCenterPoint = PiYingGL::getInstance().mapViewProjMatrix(handleCenterPoint);

/// 以下区域代码，绘制控制柄，只与 dHandleCenterPoint 有关，所以折叠
#pragma region [draw handle] 

    QPainter painter(&PiYingGL::getInstance());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0, 0, 0, 0));

	// 绘制圆
    painter.setPen(QPen(Qt::black, 4));
	painter.drawEllipse(dHandleCenterPoint, ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS);
    painter.setPen(QPen(Qt::yellow, 2));
    painter.drawEllipse(dHandleCenterPoint, ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS);

    // 绘制移动控制柄
	painter.setPen(QPen(Qt::black, 6));
    painter.drawLine(dHandleCenterPoint.x() - MOVEHANDLE_LENTH, dHandleCenterPoint.y(), dHandleCenterPoint.x() + MOVEHANDLE_LENTH, dHandleCenterPoint.y());
    painter.drawLine(dHandleCenterPoint.x(), dHandleCenterPoint.y() - MOVEHANDLE_LENTH, dHandleCenterPoint.x(), dHandleCenterPoint.y() + MOVEHANDLE_LENTH);
    painter.setPen(QPen(Qt::green, 4));
    painter.drawLine(dHandleCenterPoint.x() - MOVEHANDLE_LENTH, dHandleCenterPoint.y(), dHandleCenterPoint.x() + MOVEHANDLE_LENTH, dHandleCenterPoint.y());
    painter.setPen(QPen(Qt::red, 4));
    painter.drawLine(dHandleCenterPoint.x(), dHandleCenterPoint.y() - MOVEHANDLE_LENTH, dHandleCenterPoint.x(), dHandleCenterPoint.y() + MOVEHANDLE_LENTH);

    // 绘制中心点
    painter.setPen(QPen(Qt::black, 12));
    painter.drawPoint(dHandleCenterPoint);
    painter.setPen(QPen(Qt::yellow, HANDLE_ZONE));
    painter.drawPoint(dHandleCenterPoint);

	// 绘制缩放控制柄
    painter.setPen(QPen(Qt::black, 12));
    painter.drawPoint(dHandleCenterPoint.x(), dHandleCenterPoint.y() + SCALEHANDLE_DISTANCE);
    painter.drawPoint(dHandleCenterPoint.x() + SCALEHANDLE_DISTANCE, dHandleCenterPoint.y());
    painter.drawPoint(dHandleCenterPoint.x() + ROTATEHANDLE_RADIUS, dHandleCenterPoint.y() + ROTATEHANDLE_RADIUS);
    painter.setPen(QPen(Qt::yellow, HANDLE_ZONE));
    painter.drawPoint(dHandleCenterPoint.x(), dHandleCenterPoint.y() + SCALEHANDLE_DISTANCE);
    painter.drawPoint(dHandleCenterPoint.x() + SCALEHANDLE_DISTANCE, dHandleCenterPoint.y());
    painter.drawPoint(dHandleCenterPoint.x() + ROTATEHANDLE_RADIUS, dHandleCenterPoint.y() + ROTATEHANDLE_RADIUS);

#pragma endregion 

    // 绘制选中点
    PiYingGL::getInstance().draw_selected_points(selected_points->size());
}

void ChElementSelect::changeEditMode()
{
    if (selected_points->size() == 0) {
        editMode = ChElementEditMode::None;
        return;
    }

    qreal length = QLineF(dHandleCenterPoint, lastPos).length();

    if (length <= ROTATEHANDLE_RADIUS + HANDLE_ZONE && length >= ROTATEHANDLE_RADIUS - HANDLE_ZONE)                     editMode = ChElementEditMode::Rotate;   // 判断是否在旋转控制柄上
    else if (isInRect(lastPos, dHandleCenterPoint, HANDLE_ZONE))                                                        editMode = ChElementEditMode::Move;     // 判断是否在中心点上
    else if (isInRect(lastPos, dHandleCenterPoint, MOVEHANDLE_LENTH * 2, HANDLE_ZONE))                                  editMode = ChElementEditMode::MoveX;    // 判断是否在移动控制柄X上
    else if (isInRect(lastPos, dHandleCenterPoint, HANDLE_ZONE, MOVEHANDLE_LENTH * 2))                                  editMode = ChElementEditMode::MoveY;    // 判断是否在移动控制柄Y上
    else if (isInRect(lastPos, dHandleCenterPoint + QPoint(ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS), HANDLE_ZONE))     editMode = ChElementEditMode::Scale;    // 判断是否在缩放控制柄上
    else if (isInRect(lastPos, dHandleCenterPoint + QPoint(SCALEHANDLE_DISTANCE, 0), HANDLE_ZONE))                      editMode = ChElementEditMode::ScaleX;   // 判断是否在缩放控制柄X上
    else if (isInRect(lastPos, dHandleCenterPoint + QPoint(0, SCALEHANDLE_DISTANCE), HANDLE_ZONE))                      editMode = ChElementEditMode::ScaleY;   // 判断是否在缩放控制柄Y上
	else                                                                                                                editMode = ChElementEditMode::None;     // 不在任何控制柄上
}

void ChElementSelect::moveHandle(const QPointF& mouse)
{
    if (editMode == ChElementEditMode::None) return;

    PointVectorLayer& pointLayer = *PiYingGL::getInstance().currentLayer();

    switch (editMode) {
    case ChElementEditMode::Move: {
        QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse) - PiYingGL::getInstance().GLViewProjMatrixInvert(lastPos);
        for (int i = 0; i < selected_points->size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_points)[i], selected_points->getVert(i) + toMove);
        }
    }break;
    case ChElementEditMode::MoveX: {
        QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse.x(), 0.f) - PiYingGL::getInstance().GLViewProjMatrixInvert(lastPos.x(), 0.f);
        for (int i = 0; i < selected_points->size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_points)[i], selected_points->getVert(i) + toMove);
        }
    }break;
    case ChElementEditMode::MoveY: {
        QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(0.f, mouse.y()) - PiYingGL::getInstance().GLViewProjMatrixInvert(0.f, lastPos.y());
        for (int i = 0; i < selected_points->size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_points)[i], selected_points->getVert(i) + toMove);
        }
    }break;
    case ChElementEditMode::Rotate: {
        float angle = angleBetweenPoint(lastPos - lastDHandleCenterPoint, mouse - lastDHandleCenterPoint);
        QPointF toRot = PiYingGL::getInstance().getInsProj().map(lastHandleCenterPoint);
        for (int i = 0; i < selected_points->size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_points)[i], toRot + getRotatedPoint(PiYingGL::getInstance().getInsProj().map(selected_points->getVert(i) - lastHandleCenterPoint), angle));
            pointLayer.set_point(edit_skelen, (*selected_points)[i], PiYingGL::getInstance().getProj().map(
                edit_skelen ?
                pointLayer[(*selected_points)[i]] :
                pointLayer((*selected_points)[i]))
            );
        }
    }break;
    case ChElementEditMode::Scale: {
        float scale = (mouse.x() + mouse.y() - dHandleCenterPoint.x() - dHandleCenterPoint.y()) / (ROTATEHANDLE_RADIUS + ROTATEHANDLE_RADIUS);
        QPointF toScale = lastHandleCenterPoint * (scale - 1);
        for (int i = 0; i < selected_points->size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_points)[i], selected_points->getVert(i) * scale - toScale);
        }
    }break;
    case ChElementEditMode::ScaleX: {
        float scale = (mouse.x() - lastDHandleCenterPoint.x()) / ROTATEHANDLE_RADIUS;
        float scaleX = lastDHandleCenterPoint.x() * (1 - scale);
        for (int i = 0; i < selected_points->size(); i++) {
            QPointF mapOri = PiYingGL::getInstance().mapViewProjMatrix(selected_points->getVert(i));
            pointLayer.set_point(edit_skelen, (*selected_points)[i], PiYingGL::getInstance().GLViewProjMatrixInvert(mapOri.x() * scale + scaleX, mapOri.y()));
        }
    }break;
    case ChElementEditMode::ScaleY: {
        float scale = (mouse.y() - lastDHandleCenterPoint.y()) / ROTATEHANDLE_RADIUS;
        float scaleY = lastDHandleCenterPoint.y() * (1 - scale);
        for (int i = 0; i < selected_points->size(); i++) {
            QPointF mapOri = PiYingGL::getInstance().mapViewProjMatrix(selected_points->getVert(i));
            pointLayer.set_point(edit_skelen, (*selected_points)[i], PiYingGL::getInstance().GLViewProjMatrixInvert(mapOri.x(), mapOri.y() * scale + scaleY));
        }
    }break;
    }

    update_selected_to_draw();
    PiYingGL::getInstance().update_ch_verts();
}

void ChElementSelect::affirmHandle()
{
    selected_points->affirmVert(edit_skelen);
    lastHandleCenterPoint = handleCenterPoint;
    lastDHandleCenterPoint = dHandleCenterPoint;
}

void ChElementSelect::click_select(const QPointF& mouse)
{
    const PointVectorLayer& pointVector = *PiYingGL::getInstance().currentLayer();

    for (unsigned int i = 0; i < pointVector.size(); i++) {
        if (QLineF(edit_skelen ? pointVector[i] : pointVector(i), mouse).length() < 0.02f / PiYingGL::getInstance().viewScale.value()) {
            if (selected_points->contains(i)) return;

            if (!KeyboardStateWin::isCtrlHeld()) selected_points->clear(); 

            selected_points->append(i);
            update_selected_to_draw();

            return;
        }
    }

    if (!KeyboardStateWin::isCtrlHeld()) selected_points->clear();
}

void ChElementSelect::update_selected_to_draw()
{
    const SelectedPoints& selectedPoints = *selected_points;

    std::vector<float> selectedPointsFloat;
    selectedPointsFloat.reserve(selectedPoints.size() * 2);

    for (int i = 0; i < selectedPoints.size(); i++) {
        const QPointF& selectPoint = PiYingGL::getInstance().currentLayer()->get(selectedPoints[i], edit_skelen);

        selectedPointsFloat.push_back(selectPoint.x());
        selectedPointsFloat.push_back(selectPoint.y());
    }

    PiYingGL::getInstance().update_selected_verts(selectedPointsFloat);
}

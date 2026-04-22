#include "tool_part_vert_select.h"

#include "selected_points.h"
#include "piYingGL.h"
#include "part.h"
#include "time_line_gl.h"
#include "point_vector.h"
#include "KeyboardStateWin.h"

#include "cus_func_zone.h"
#include "cus_func_point2d.h"

#include "enum_select_handle_mode.h"

#include "static_handle_zone.h"

#include <qpainter>

piying::tool::part::VertSelect::VertSelect() :
	_edit_mode(HandleControlMode::None)
{
	selected_points = std::make_unique<piying::SelectedPoints>();
}

piying::tool::part::VertSelect::~VertSelect() {}

void piying::tool::part::VertSelect::change_edit_mode_by_setting_last_pos(const QPointF& mouse)
{
    lastPos = mouse;

    if (selected_points->isEmpty()) {
        _edit_mode = HandleControlMode::None;
        return;
    }

    _edit_mode = controlModeByMousePos(mouse, dHandleCenterPoint);
}

void piying::tool::part::VertSelect::click_select(const QPointF& mouse)
{
    const PointVectorLayer* pointVector = TimelineGl::getInstance().currentLayer();
    if (!pointVector) return;

    for (unsigned int i = 0; i < pointVector->element_size(); i++) {
        if (QLineF((*pointVector)[i], mouse).length() < 0.02f / PiYingGL::getInstance().viewScale.value()) {
            if (selected_points->contains(i)) return;
            if (TimelineGl::getInstance().get_current_part()->is_point_traced(i)) continue;

            if (!KeyboardStateWin::isCtrlHeld()) selected_points->clear();

            selected_points->append(i);
            update_selected_to_draw();

            return;
        }
    }

    if (!KeyboardStateWin::isCtrlHeld()) selected_points->clear();
}

void piying::tool::part::VertSelect::update_selected_to_draw()
{
    std::vector<float> selectedPointsFloat;
    selectedPointsFloat.reserve(selected_points->point_size() * 2);

    for (int i = 0; i < selected_points->point_size(); i++) {
        const QPointF& selectPoint = TimelineGl::getInstance().currentLayerOrigin()->get((*selected_points)[i], true);

        selectedPointsFloat.push_back(selectPoint.x());
        selectedPointsFloat.push_back(selectPoint.y());
    }

    PiYingGL::getInstance().update_selected_verts(selectedPointsFloat);
}

void piying::tool::part::VertSelect::draw_handle_and_selected()
{
    if (selected_points->point_size() == 0) return;

    // 计算中心点
    PointVectorLayer& pointLayer = *TimelineGl::getInstance().currentLayer();
    handleCenterPoint = QPointF();
    for (unsigned int i : selected_points->index()) {
        handleCenterPoint += pointLayer[i];
    }

    handleCenterPoint /= selected_points->point_size();

    dHandleCenterPoint = PiYingGL::getInstance().mapViewProjMatrix(handleCenterPoint);

    drawHandleZone(&PiYingGL::getInstance(), dHandleCenterPoint);

    // 绘制选中点
    PiYingGL::getInstance().draw_selected_points(selected_points->point_size());
}

void piying::tool::part::VertSelect::move_handle(const QPointF& mouse)
{
    if (_edit_mode == HandleControlMode::None) return;

    PointVectorLayer& pointLayer = *TimelineGl::getInstance().currentLayer();
    PointVectorLayer& pointLayerOrigin = *TimelineGl::getInstance().currentLayerOrigin();

    switch (_edit_mode) {
    case HandleControlMode::Move: {
        const QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse) - PiYingGL::getInstance().GLViewProjMatrixInvert(lastPos);
        QPointF toSet;
        for (int i = 0; i < selected_points->point_size(); i++) {
            toSet = selected_points->getVert(i) + toMove;
            pointLayer.set_point(true, (*selected_points)[i], toSet);
            pointLayerOrigin.set_point(true, (*selected_points)[i], toSet);
        }
    }break;
    case HandleControlMode::MoveX: {
        const QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse.x(), 0.f) - PiYingGL::getInstance().GLViewProjMatrixInvert(lastPos.x(), 0.f);
        QPointF toSet;
        for (int i = 0; i < selected_points->point_size(); i++) {
            toSet = selected_points->getVert(i) + toMove;
            pointLayer.set_point(true, (*selected_points)[i], toSet);
            pointLayerOrigin.set_point(true, (*selected_points)[i], toSet);
        }
    }break;
    case HandleControlMode::MoveY: {
        const QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(0.f, mouse.y()) - PiYingGL::getInstance().GLViewProjMatrixInvert(0.f, lastPos.y());
        QPointF toSet;
        for (int i = 0; i < selected_points->point_size(); i++) {
            toSet = selected_points->getVert(i) + toMove;
            pointLayer.set_point(true, (*selected_points)[i], toSet);
            pointLayerOrigin.set_point(true, (*selected_points)[i], toSet);
        }
    }break;
    case HandleControlMode::Rotate: {
        using namespace piying;

        QMatrix4x4 rotation = PiYingGL::getInstance().getProj();
        rotation.rotate(angleBetweenPointDegree(lastPos - lastDHandleCenterPoint, mouse - lastDHandleCenterPoint), 0, 0, 1);
        rotation *= PiYingGL::getInstance().getInsProj();

        QPointF toSet;
        const QPointF mappedLastHandleCenterPoint = lastHandleCenterPoint - rotation.map(lastHandleCenterPoint);
        for (int i = 0; i < selected_points->point_size(); i++) {
            toSet = rotation.map(selected_points->getVert(i)) + mappedLastHandleCenterPoint;
            pointLayerOrigin.set_point(true, (*selected_points)[i], toSet);
            pointLayer.set_point(true, (*selected_points)[i], toSet);
        }
    }break;
    case HandleControlMode::Scale: {
        float scale = (mouse.x() + mouse.y() - dHandleCenterPoint.x() - dHandleCenterPoint.y()) / (ROTATEHANDLE_RADIUS + ROTATEHANDLE_RADIUS);
        const QPointF toScale = lastHandleCenterPoint * (scale - 1);
        QPointF toSet;
        for (int i = 0; i < selected_points->point_size(); i++) {
            toSet = selected_points->getVert(i) * scale - toScale;
            pointLayer.set_point(true, (*selected_points)[i], toSet);
            pointLayerOrigin.set_point(true, (*selected_points)[i], toSet);
        }
    }break;
    case HandleControlMode::ScaleX: {
        float scale = (mouse.x() - lastDHandleCenterPoint.x()) / ROTATEHANDLE_RADIUS;
        float scaleX = lastDHandleCenterPoint.x() * (1 - scale);
        QPointF toSet;
        for (int i = 0; i < selected_points->point_size(); i++) {
            toSet = PiYingGL::getInstance().mapViewProjMatrix(selected_points->getVert(i));
            toSet = PiYingGL::getInstance().GLViewProjMatrixInvert(toSet.x() * scale + scaleX, toSet.y());
            pointLayer.set_point(true, (*selected_points)[i], toSet);
            pointLayerOrigin.set_point(true, (*selected_points)[i], toSet);
        }
    }break;
    case HandleControlMode::ScaleY: {
        float scale = (mouse.y() - lastDHandleCenterPoint.y()) / ROTATEHANDLE_RADIUS;
        float scaleY = lastDHandleCenterPoint.y() * (1 - scale);
        QPointF toSet;
        for (int i = 0; i < selected_points->point_size(); i++) {
            toSet = PiYingGL::getInstance().mapViewProjMatrix(selected_points->getVert(i));
            toSet = PiYingGL::getInstance().GLViewProjMatrixInvert(toSet.x(), toSet.y() * scale + scaleY);
            pointLayer.set_point(true, (*selected_points)[i], toSet);
            pointLayerOrigin.set_point(true, (*selected_points)[i], toSet);
        }
    }break;
    }

    update_selected_to_draw();
    TimelineGl::getInstance().get_current_part()->update_vbo();
}

void piying::tool::part::VertSelect::clear()
{
    selected_points->clear();
}

void piying::tool::part::VertSelect::affirm_handle()
{
    selected_points->affirmVert(*TimelineGl::getInstance().currentLayerOrigin(), true);
    lastHandleCenterPoint = handleCenterPoint;
    lastDHandleCenterPoint = dHandleCenterPoint;
}

void piying::tool::part::VertSelect::delete_element()
{
    std::vector<unsigned int>& idx = *TimelineGl::getInstance().get_current_indices();
    const size_t nVert = TimelineGl::getInstance().currentLayer()->element_size();
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

        TimelineGl::getInstance().currentLayer()->copy_from_to(old, newVertCount);
        old2new[old] = newVertCount++;
    }

    TimelineGl::getInstance().currentLayer()->resize(newVertCount);

    size_t outIdx = 0;
    for (size_t t = 0; t < nTri; ++t) {
        if (killTri[t]) continue;

        const size_t t3 = t * 3;

        for (int i = 0; i < 3; ++i) idx[outIdx++] = old2new[idx[t3 + i]];
    }

    idx.resize(outIdx);
    selected_points->clear();

    if (idx.size() > 0) {
        TimelineGl::getInstance().get_current_part()->update_vbo();
    }
    else {
        TimelineGl::getInstance().delete_part_all_vert();
    }

    TimelineGl::getInstance().update();
}

void piying::tool::part::VertSelect::append(unsigned int index)
{
    selected_points->append(index);
}

bool piying::tool::part::VertSelect::contains(unsigned int index) const noexcept
{
    return selected_points->contains(index);
}

void piying::tool::part::VertSelect::escape()
{
    if (selected_points->point_size() > 0) selected_points->removeLast();
}

#include "tool_vert_select.h"
#include "piYingGL.h"
#include "selected_points.h"
#include "static_handle_zone.h"
#include "point_vector.h"
#include "KeyboardStateWin.h"

#include "enum_select_handle_mode.h"
#include "enum_edit_mode.h"
#include "cus_func_zone.h"
#include "cus_func_point2d.h"

#include <qpainter>

piying::tool::texture::VertSelect::VertSelect() :
    _edit_shape(PiYingGL::getInstance().editMode != EditMode::characterTexture),
    editMode(HandleControlMode::None)
{
    selected_points = std::make_unique<SelectedPoints>();
}

piying::tool::texture::VertSelect::~VertSelect() {}

void piying::tool::texture::VertSelect::escape()
{ 
    if (selected_points->point_size() > 0) selected_points->removeLast(); 
}

void piying::tool::texture::VertSelect::deleteElement()
{
    std::vector<unsigned int>& idx = *PiYingGL::getInstance().currentIndex();
    const size_t nVert = PiYingGL::getInstance().currentLayer()->element_size();
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

void piying::tool::texture::VertSelect::draw_handle_and_selected()
{
    if(selected_points->point_size() == 0) return;

    // 计算中心点
    PointVectorLayer& pointLayer = *PiYingGL::getInstance().currentLayer();
    handleCenterPoint = QPointF();
    for (unsigned int i : selected_points->index()) {
        handleCenterPoint += _edit_shape ? pointLayer[i] : pointLayer(i);
    }
    
    handleCenterPoint /= selected_points->point_size();

    dHandleCenterPoint = PiYingGL::getInstance().mapViewProjMatrix(handleCenterPoint);

    drawHandleZone(&PiYingGL::getInstance(), dHandleCenterPoint);

    // 绘制选中点
    PiYingGL::getInstance().draw_selected_points(selected_points->point_size());
}

void piying::tool::texture::VertSelect::change_edit_mode_by_setting_last_pos(const QPointF& mouse)
{
    lastPos = mouse;

    if (selected_points->point_size() == 0) {
        editMode = HandleControlMode::None;
        return;
    }

    editMode = controlModeByMousePos(mouse, dHandleCenterPoint);
}

void piying::tool::texture::VertSelect::moveHandle(const QPointF& mouse)
{
    if (editMode == HandleControlMode::None) return;

    PointVectorLayer& pointLayer = *PiYingGL::getInstance().currentLayer();

    switch (editMode) {
    case HandleControlMode::Move: {
        QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse) - PiYingGL::getInstance().GLViewProjMatrixInvert(lastPos);
        for (int i = 0; i < selected_points->point_size(); i++) {
            pointLayer.set_point(_edit_shape, (*selected_points)[i], selected_points->getVert(i) + toMove);
        }
    }break;
    case HandleControlMode::MoveX: {
        QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse.x(), 0.f) - PiYingGL::getInstance().GLViewProjMatrixInvert(lastPos.x(), 0.f);
        for (int i = 0; i < selected_points->point_size(); i++) {
            pointLayer.set_point(_edit_shape, (*selected_points)[i], selected_points->getVert(i) + toMove);
        }
    }break;
    case HandleControlMode::MoveY: {
        QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(0.f, mouse.y()) - PiYingGL::getInstance().GLViewProjMatrixInvert(0.f, lastPos.y());
        for (int i = 0; i < selected_points->point_size(); i++) {
            pointLayer.set_point(_edit_shape, (*selected_points)[i], selected_points->getVert(i) + toMove);
        }
    }break;
    case HandleControlMode::Rotate: {
        using namespace piying;

        QMatrix4x4 rotation = PiYingGL::getInstance().getProj();
        rotation.rotate(angleBetweenPointDegree(lastPos - lastDHandleCenterPoint, mouse - lastDHandleCenterPoint), 0, 0, 1);
        rotation *= PiYingGL::getInstance().getInsProj();

        const QPointF mappedLastHandleCenterPoint = lastHandleCenterPoint - rotation.map(lastHandleCenterPoint);
        for (int i = 0; i < selected_points->point_size(); i++) {
            pointLayer.set_point(_edit_shape, (*selected_points)[i], rotation.map(selected_points->getVert(i)) + mappedLastHandleCenterPoint);
        }
    }break;
    case HandleControlMode::Scale: {
        float scale = (mouse.x() + mouse.y() - dHandleCenterPoint.x() - dHandleCenterPoint.y()) / (ROTATEHANDLE_RADIUS + ROTATEHANDLE_RADIUS);
        QPointF toScale = lastHandleCenterPoint * (scale - 1);
        for (int i = 0; i < selected_points->point_size(); i++) {
            pointLayer.set_point(_edit_shape, (*selected_points)[i], selected_points->getVert(i) * scale - toScale);
        }
    }break;
    case HandleControlMode::ScaleX: {
        float scale = (mouse.x() - lastDHandleCenterPoint.x()) / ROTATEHANDLE_RADIUS;
        float scaleX = lastDHandleCenterPoint.x() * (1 - scale);
        for (int i = 0; i < selected_points->point_size(); i++) {
            QPointF mapOri = PiYingGL::getInstance().mapViewProjMatrix(selected_points->getVert(i));
            pointLayer.set_point(_edit_shape, (*selected_points)[i], PiYingGL::getInstance().GLViewProjMatrixInvert(mapOri.x() * scale + scaleX, mapOri.y()));
        }
    }break;
    case HandleControlMode::ScaleY: {
        float scale = (mouse.y() - lastDHandleCenterPoint.y()) / ROTATEHANDLE_RADIUS;
        float scaleY = lastDHandleCenterPoint.y() * (1 - scale);
        for (int i = 0; i < selected_points->point_size(); i++) {
            QPointF mapOri = PiYingGL::getInstance().mapViewProjMatrix(selected_points->getVert(i));
            pointLayer.set_point(_edit_shape, (*selected_points)[i], PiYingGL::getInstance().GLViewProjMatrixInvert(mapOri.x(), mapOri.y() * scale + scaleY));
        }
    }break;
    }

    update_selected_to_draw();
    PiYingGL::getInstance().update_sub_ch_verts();
}

void piying::tool::texture::VertSelect::affirmHandle()
{
    selected_points->affirmVert(*PiYingGL::getInstance().currentLayer(), _edit_shape);
    lastHandleCenterPoint = handleCenterPoint;
    lastDHandleCenterPoint = dHandleCenterPoint;
}

void piying::tool::texture::VertSelect::click_select(const QPointF& mouse)
{
    const PointVectorLayer& pointVector = *PiYingGL::getInstance().currentLayer();

    for (unsigned int i = 0; i < pointVector.element_size(); i++) {
        if (QLineF(_edit_shape ? pointVector[i] : pointVector(i), mouse).length() < 0.02f / PiYingGL::getInstance().viewScale.value()) {
            if (selected_points->contains(i)) return;

            if (!KeyboardStateWin::isCtrlHeld()) selected_points->clear(); 

            selected_points->append(i);
            update_selected_to_draw();

            return;
        }
    }

    if (!KeyboardStateWin::isCtrlHeld()) selected_points->clear();
}

void piying::tool::texture::VertSelect::update_selected_to_draw()
{
    std::vector<float> selectedPointsFloat;
    selectedPointsFloat.reserve(selected_points->point_size() * 2);

    for (int i = 0; i < selected_points->point_size(); i++) {
        const QPointF& selectPoint = PiYingGL::getInstance().currentLayer()->get((*selected_points)[i], _edit_shape);

        selectedPointsFloat.push_back(selectPoint.x());
        selectedPointsFloat.push_back(selectPoint.y());
    }

    PiYingGL::getInstance().update_selected_verts(selectedPointsFloat);
}

void piying::tool::texture::VertSelect::clear()
{
    selected_points->clear();
}

bool piying::tool::texture::VertSelect::contains(unsigned int index) const noexcept
{
    return selected_points->contains(index);
}

void piying::tool::texture::VertSelect::push_back(unsigned int index)
{
    selected_points->append(index);
}

void piying::tool::texture::VertSelect::append(unsigned int index)
{
    selected_points->append(index);
}
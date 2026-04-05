#include "tool_triangle_select.h"

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
#include <array>

piying::tool::texture::TriangleSelect::TriangleSelect() :
    edit_skelen(PiYingGL::getInstance().editMode == EditMode::characterSkeleton),
    editMode(ToolHandleControlMode::None)
{
    selected_trangle = std::make_unique<SelectedTriangle>();
}

piying::tool::texture::TriangleSelect::~TriangleSelect()
{
}

void piying::tool::texture::TriangleSelect::escape()
{
    if (selected_trangle->isEmpty()) return;
    selected_trangle->remove_last_triangle();
}

void piying::tool::texture::TriangleSelect::enter()
{
    if (selected_trangle->isEmpty()) return;

    PiYingGL::getInstance().add_part(selected_trangle->index());
}

void piying::tool::texture::TriangleSelect::deleteElement()
{
    std::vector<unsigned int>& idx = *PiYingGL::getInstance().currentIndex();
    PointVectorLayer& pointLayer = *PiYingGL::getInstance().currentLayer();
    const size_t nVert = pointLayer.element_size();
    const size_t nTri = idx.size() / 3;

    std::vector<bool> killVert(nVert, false);
    std::vector<bool> killTri(nTri, false);
    for (unsigned v : selected_trangle->index()) killVert[v] = true;

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

        pointLayer.copy_from_to(old, newVertCount);
        old2new[old] = newVertCount++;
    }

    pointLayer.resize(newVertCount);

    size_t outIdx = 0;
    for (size_t t = 0; t < nTri; ++t) {
        if (killTri[t]) continue;

        const size_t t3 = t * 3;

        for (int i = 0; i < 3; ++i) idx[outIdx++] = old2new[idx[t3 + i]];
    }

    idx.resize(outIdx);

    selected_trangle->clear();
    PiYingGL::getInstance().update_ch_verts();
}

void piying::tool::texture::TriangleSelect::draw_handle_and_selected()
{
    if (selected_trangle->isEmpty()) return;

    // 计算中心点
    PointVectorLayer& pointLayer = *PiYingGL::getInstance().currentLayer();
    _gl_handle_center = QPointF();
    for (unsigned int i : selected_trangle->index()) {
        _gl_handle_center += edit_skelen ? pointLayer[i] : pointLayer(i);
    }

    _gl_handle_center /= selected_trangle->point_size();

    _widget_handle_center = PiYingGL::getInstance().mapViewProjMatrix(_gl_handle_center);

    drawHandleZone(&PiYingGL::getInstance(), _widget_handle_center);

    // 绘制选中点
    PiYingGL::getInstance().draw_selected_triangle(selected_trangle->n_triangle());
}

void piying::tool::texture::TriangleSelect::changeEditMode()
{
    if (selected_trangle->isEmpty()) {
        editMode = ToolHandleControlMode::None;
        return;
    }

    editMode = controlModeByMousePos(lastPos, _widget_handle_center); 
}

void piying::tool::texture::TriangleSelect::moveHandle(const QPointF& mouse)
{
    if (editMode == ToolHandleControlMode::None) return;

    PointVectorLayer& pointLayer = *PiYingGL::getInstance().currentLayer();

    /// 根据 editMode 进行变换
    switch (editMode) {
    case ToolHandleControlMode::Move: {
        QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse) - PiYingGL::getInstance().GLViewProjMatrixInvert(lastPos);
        for (int i = 0; i < selected_trangle->point_size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_trangle)[i], selected_trangle->getVert(i) + toMove);
        }
    }break;
    case ToolHandleControlMode::MoveX: {
        QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse.x(), 0.f) - PiYingGL::getInstance().GLViewProjMatrixInvert(lastPos.x(), 0.f);
        for (int i = 0; i < selected_trangle->point_size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_trangle)[i], selected_trangle->getVert(i) + toMove);
        }
    }break;
    case ToolHandleControlMode::MoveY: {
        QPointF toMove = PiYingGL::getInstance().GLViewProjMatrixInvert(0.f, mouse.y()) - PiYingGL::getInstance().GLViewProjMatrixInvert(0.f, lastPos.y());
        for (int i = 0; i < selected_trangle->point_size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_trangle)[i], selected_trangle->getVert(i) + toMove);
        }
    }break;
    case ToolHandleControlMode::Rotate: {
        using namespace piying;

        QMatrix4x4 rotation = PiYingGL::getInstance().getProj();
        rotation.rotate(angleBetweenPointDegree(lastPos - lastDHandleCenterPoint, mouse - lastDHandleCenterPoint), 0, 0, 1);
        rotation *= PiYingGL::getInstance().getInsProj();

        const QPointF mappedLastHandleCenterPoint = lastHandleCenterPoint - rotation.map(lastHandleCenterPoint);
        for (int i = 0; i < selected_trangle->point_size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_trangle)[i], rotation.map(selected_trangle->getVert(i)) + mappedLastHandleCenterPoint);
        }
    }break;
    case ToolHandleControlMode::Scale: {
        float scale = (mouse.x() + mouse.y() - _widget_handle_center.x() - _widget_handle_center.y()) / (ROTATEHANDLE_RADIUS + ROTATEHANDLE_RADIUS);
        QPointF toScale = lastHandleCenterPoint * (scale - 1);
        for (int i = 0; i < selected_trangle->point_size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_trangle)[i], selected_trangle->getVert(i) * scale - toScale);
        }
    }break;
    case ToolHandleControlMode::ScaleX: {
        const float scale = (mouse.x() - lastDHandleCenterPoint.x()) / ROTATEHANDLE_RADIUS;
        const float scaleX = lastDHandleCenterPoint.x() * (1 - scale);

        QMatrix4x4 trans = PiYingGL::getInstance().getViewProjMatrixInvert() * PiYingGL::getInstance().get_map_to_gl_matrix();
        trans.translate(scaleX, 0.f);
        trans.scale(scale, 1.f);

        trans *= PiYingGL::getInstance().get_gl_to_map_matrix() * PiYingGL::getInstance().getViewProjMatrix();

        for (int i = 0; i < selected_trangle->point_size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_trangle)[i], trans.map(selected_trangle->getVert(i)));
        }
    }break;
    case ToolHandleControlMode::ScaleY: {
        const float scale = (mouse.y() - lastDHandleCenterPoint.y()) / ROTATEHANDLE_RADIUS;
        const float scaleY = lastDHandleCenterPoint.y() * (1 - scale);

        QMatrix4x4 trans = PiYingGL::getInstance().getViewProjMatrixInvert() * PiYingGL::getInstance().get_map_to_gl_matrix();
        trans.translate(0.f, scaleY);
        trans.scale(1.f, scale);

        trans *= PiYingGL::getInstance().get_gl_to_map_matrix() * PiYingGL::getInstance().getViewProjMatrix();

        for (int i = 0; i < selected_trangle->point_size(); i++) {
            pointLayer.set_point(edit_skelen, (*selected_trangle)[i], trans.map(selected_trangle->getVert(i)));
        }
    }break;
    }

    update_selected_to_draw();
    PiYingGL::getInstance().update_sub_ch_verts();
}

void piying::tool::texture::TriangleSelect::affirmHandle()
{
    selected_trangle->affirmVert(*PiYingGL::getInstance().currentLayer(), edit_skelen);
    lastHandleCenterPoint = _gl_handle_center;
    lastDHandleCenterPoint = _widget_handle_center;
}

void piying::tool::texture::TriangleSelect::mouse_press(const QPointF& mouse)
{
    lastPos = mouse;

    changeEditMode();

    if (editMode != ToolHandleControlMode::None) {
        affirmHandle();
    }
}

void piying::tool::texture::TriangleSelect::click_select(const QPointF& mouse)
{
    const PointVectorLayer& pointVector = *PiYingGL::getInstance().currentLayer();
    const std::vector<unsigned int>& triangleIndices = *PiYingGL::getInstance().currentIndex();

    QPointF eachTriangle[3];
    std::array<unsigned int, 3> triangle;
    for (unsigned int i = 0; i < triangleIndices.size(); i += 3) {
        for (int j = 0; j < 3; ++j) eachTriangle[j] = pointVector.get(triangleIndices[i + j], edit_skelen);

        triangle = { triangleIndices[i], triangleIndices[i + 1], triangleIndices[i + 2] };
        if (piying::isPointInTriangle(mouse, eachTriangle)) {
            if (selected_trangle->contains(triangle)) return;

            if (!KeyboardStateWin::isCtrlHeld()) selected_trangle->clear();

            selected_trangle->append(triangle);
            update_selected_to_draw();

            return;
        }
    }

    if (!KeyboardStateWin::isCtrlHeld()) selected_trangle->clear();
}

void piying::tool::texture::TriangleSelect::update_selected_to_draw()
{
    PiYingGL::getInstance().update_selected_triangle(selected_trangle->index_data(), selected_trangle->point_size());
    PiYingGL::getInstance().update_selected_triangle_texture();
}

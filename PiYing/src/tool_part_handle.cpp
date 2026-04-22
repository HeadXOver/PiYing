#include "tool_part_handle.h"

#include "point_vector.h"
#include "time_line_gl.h"
#include "piYingGL.h"
#include "PiYing.h"
#include "part.h"

#include "cus_func_zone.h"
#include "cus_func_point2d.h"

#include "enum_select_handle_mode.h"

#include "static_handle_zone.h"

#include <qpainter>

piying::tool::part::Handle::Handle():
    _edit_mode(HandleControlMode::None)
{
}

piying::tool::part::Handle::~Handle()
{
}

void piying::tool::part::Handle::draw()
{
    // 计算中心点
    PointVectorLayer& pointLayer = *TimelineGl::getInstance().currentLayer();
    handleCenterPoint = QPointF();
    for (size_t i = 0; i < pointLayer.element_size(); i++) {
        handleCenterPoint += pointLayer[i];
    }

    handleCenterPoint /= pointLayer.element_size();

    dHandleCenterPoint = PiYingGL::getInstance().mapViewProjMatrix(handleCenterPoint);

    drawHandleZone(&PiYingGL::getInstance(), dHandleCenterPoint);
}

void piying::tool::part::Handle::click(const QPointF& mouse)
{
	_is_press = true;
	_last_pos = mouse;

	_edit_mode = controlModeByMousePos(mouse, dHandleCenterPoint);

    lastHandleCenterPoint = handleCenterPoint;
    lastDHandleCenterPoint = dHandleCenterPoint;
}

void piying::tool::part::Handle::mouse_move(const QPointF& mouse)
{
	if (!_is_press) return;

    if (_edit_mode == HandleControlMode::None) return;

    PointVectorLayer& pointLayer = *TimelineGl::getInstance().currentLayer();
    PointVectorLayer& pointLayerOrigin = *TimelineGl::getInstance().currentLayerOrigin();

    switch (_edit_mode) {
    case HandleControlMode::Move: {
        pointLayer.skelen_add_move(
            PiYingGL::getInstance().GLViewProjMatrixInvert(mouse) - PiYingGL::getInstance().GLViewProjMatrixInvert(_last_pos),
            pointLayerOrigin
        );
    }break;
    case HandleControlMode::MoveX: {
        pointLayer.skelen_add_move(
            PiYingGL::getInstance().GLViewProjMatrixInvert(mouse.x(), 0.f) - PiYingGL::getInstance().GLViewProjMatrixInvert(_last_pos.x(), 0.f),
            pointLayerOrigin
        );
    }break;
    case HandleControlMode::MoveY: {
        pointLayer.skelen_add_move(
            PiYingGL::getInstance().GLViewProjMatrixInvert(0.f, mouse.y()) - PiYingGL::getInstance().GLViewProjMatrixInvert(0.f, _last_pos.y()),
            pointLayerOrigin
        );
    }break;
    case HandleControlMode::Rotate: {
        using namespace piying;

        QMatrix4x4 rotation = PiYingGL::getInstance().getProj();
        rotation.rotate(angleBetweenPointDegree(_last_pos - lastDHandleCenterPoint, mouse - lastDHandleCenterPoint), 0, 0, 1);
        rotation *= PiYingGL::getInstance().getInsProj();

        QPointF toSet;
        const QPointF mappedLastHandleCenterPoint = lastHandleCenterPoint - rotation.map(lastHandleCenterPoint);
        for (int i = 0; i < pointLayer.element_size(); i++) {
            toSet = rotation.map(pointLayerOrigin[i]) + mappedLastHandleCenterPoint;
            pointLayer.set_point(true, i, toSet);
        }
    }break;
    case HandleControlMode::Scale: {
        float scale = (mouse.x() + mouse.y() - dHandleCenterPoint.x() - dHandleCenterPoint.y()) / (ROTATEHANDLE_RADIUS + ROTATEHANDLE_RADIUS);
        const QPointF toScale = lastHandleCenterPoint * (scale - 1);
        QPointF toSet;
        for (int i = 0; i < pointLayer.element_size(); i++) {
            pointLayer.set_point(true, i, pointLayerOrigin[i] * scale - toScale);
        }
    }break;
    case HandleControlMode::ScaleX: {
        float scale = (mouse.x() - lastDHandleCenterPoint.x()) / ROTATEHANDLE_RADIUS;
        float scaleX = lastDHandleCenterPoint.x() * (1 - scale);
        QPointF toSet;
        for (int i = 0; i < pointLayer.element_size(); i++) {
            toSet = PiYingGL::getInstance().mapViewProjMatrix(pointLayerOrigin[i]);
            toSet = PiYingGL::getInstance().GLViewProjMatrixInvert(toSet.x() * scale + scaleX, toSet.y());
            pointLayer.set_point(true, i, toSet);
        }
    }break;
    case HandleControlMode::ScaleY: {
        float scale = (mouse.y() - lastDHandleCenterPoint.y()) / ROTATEHANDLE_RADIUS;
        float scaleY = lastDHandleCenterPoint.y() * (1 - scale);
        QPointF toSet;
        for (int i = 0; i < pointLayer.element_size(); i++) {
            toSet = PiYingGL::getInstance().mapViewProjMatrix(pointLayerOrigin[i]);
            toSet = PiYingGL::getInstance().GLViewProjMatrixInvert(toSet.x(), toSet.y() * scale + scaleY);
            pointLayer.set_point(true, i, toSet);
        }
    }break;
    }

    TimelineGl::getInstance().get_current_part()->update_vbo();
}

void piying::tool::part::Handle::release(const QPointF& mouse)
{
	_is_press = false;

    PointVectorLayer& pointLayer = *TimelineGl::getInstance().currentLayer();
    PointVectorLayer& pointLayerOrigin = *TimelineGl::getInstance().currentLayerOrigin();

    pointLayerOrigin = pointLayer;
}

void piying::tool::part::Handle::delete_element()
{
    TimelineGl::getInstance().part_delete();
    PiYing::getInstance().update_part_slider();
    PiYingGL::getInstance().update();
    TimelineGl::getInstance().update();
}

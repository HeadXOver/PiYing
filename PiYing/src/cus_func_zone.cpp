#include "cus_func_zone.h"

#include "cus_func_point2d.h"

#include "enum_select_handle_mode.h"

#include "static_handle_zone.h"

#include <QRectF>
#include <QLineF>
#include <qpainter>
#include <qwidget>

bool piying::isInRect(const QPointF& mouse, const QPointF& pos, int zone)
{
    QRectF centerRect(pos.x() - zone / 2, pos.y() - zone / 2, zone, zone);
    return centerRect.contains(mouse);
}

bool piying::isInRect(const QPointF& mouse, const QPointF& pos, int width, int height)
{
    QRectF centerRect(pos.x() - width / 2, pos.y() - height / 2, width, height);
    return  centerRect.contains(mouse);
}

bool piying::isPointInTriangle(const QPointF& point, const QPointF* const triangle) noexcept
{
    using namespace piying;

    double d1 = crossProduct(point, triangle[0], triangle[1]);
    double d2 = crossProduct(point, triangle[1], triangle[2]);
    double d3 = crossProduct(point, triangle[2], triangle[0]);

    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    // 如果三个叉积同号（全正或全负），则点在三角形内（或在边上）
    return !(hasNeg && hasPos);
}

bool piying::isRectIntersectTriangle(const QRectF& rect, const QPointF* const triangle)
{
    for (int i = 0; i < 3; ++i) {
        if (rect.contains(triangle[i])) return true;
    }

    QPointF rectVertices[4] = { rect.topLeft(), rect.topRight(), rect.bottomLeft(), rect.bottomRight() };

    for (int i = 0; i < 4; ++i) {
        if (isPointInTriangle(rectVertices[i], triangle)) return true;
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (piying::isCross(triangle[i], triangle[(i + 1) % 3], rectVertices[j], rectVertices[(j + 1) % 4])) return true;
        }
    }

    return false;
}

bool piying::isInsideSquare(const QPointF& point, float side) noexcept
{
    return (point.x() >= -side / 2.f && point.x() <= side / 2.f && point.y() >= -side / 2.f && point.y() <= side / 2.f);
}

piying::tool::HandleControlMode piying::controlModeByMousePos(const QPointF& mouse, const QPointF& center)
{
    const qreal length = QLineF(center, mouse).length();

    return
        (length <= ROTATEHANDLE_RADIUS + HANDLE_ZONE && length >= ROTATEHANDLE_RADIUS - HANDLE_ZONE) ?
        tool::HandleControlMode::Rotate :     // 判断是否在旋转控制柄上
        (piying::isInRect(mouse, center, HANDLE_ZONE)) ?
        tool::HandleControlMode::Move :       // 判断是否在中心点上
        (piying::isInRect(mouse, center, MOVEHANDLE_LENTH * 2, HANDLE_ZONE)) ?
        tool::HandleControlMode::MoveX :      // 判断是否在移动控制柄X上
        (piying::isInRect(mouse, center, HANDLE_ZONE, MOVEHANDLE_LENTH * 2)) ?
        tool::HandleControlMode::MoveY :      // 判断是否在移动控制柄Y上
        (piying::isInRect(mouse, center + QPoint(ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS), HANDLE_ZONE)) ?
        tool::HandleControlMode::Scale :      // 判断是否在缩放控制柄上
        (piying::isInRect(mouse, center + QPoint(SCALEHANDLE_DISTANCE, 0), HANDLE_ZONE)) ?
        tool::HandleControlMode::ScaleX :     // 判断是否在缩放控制柄X上
        (piying::isInRect(mouse, center + QPoint(0, SCALEHANDLE_DISTANCE), HANDLE_ZONE)) ?
        tool::HandleControlMode::ScaleY :     // 判断是否在缩放控制柄Y上
        tool::HandleControlMode::None;        // 不在任何控制柄上
}

void piying::drawHandleZone(QWidget* widget, const QPointF& position)
{
    QPainter painter(widget);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0, 0, 0, 0));

    // 绘制圆
    painter.setPen(QPen(Qt::black, 4));
    painter.drawEllipse(position, ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS);
    painter.setPen(QPen(Qt::yellow, 2));
    painter.drawEllipse(position, ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS);

    // 绘制移动控制柄
    painter.setPen(QPen(Qt::black, 6));
    painter.drawLine(position.x() - MOVEHANDLE_LENTH, position.y(), position.x() + MOVEHANDLE_LENTH, position.y());
    painter.drawLine(position.x(), position.y() - MOVEHANDLE_LENTH, position.x(), position.y() + MOVEHANDLE_LENTH);
    painter.setPen(QPen(Qt::green, 4));
    painter.drawLine(position.x() - MOVEHANDLE_LENTH, position.y(), position.x() + MOVEHANDLE_LENTH, position.y());
    painter.setPen(QPen(Qt::red, 4));
    painter.drawLine(position.x(), position.y() - MOVEHANDLE_LENTH, position.x(), position.y() + MOVEHANDLE_LENTH);

    // 绘制中心点
    painter.setPen(QPen(Qt::black, 12));
    painter.drawPoint(position);
    painter.setPen(QPen(Qt::yellow, HANDLE_ZONE));
    painter.drawPoint(position);

    // 绘制缩放控制柄
    painter.setPen(QPen(Qt::black, 12));
    painter.drawPoint(position.x(), position.y() + SCALEHANDLE_DISTANCE);
    painter.drawPoint(position.x() + SCALEHANDLE_DISTANCE, position.y());
    painter.drawPoint(position.x() + ROTATEHANDLE_RADIUS, position.y() + ROTATEHANDLE_RADIUS);
    painter.setPen(QPen(Qt::yellow, HANDLE_ZONE));
    painter.drawPoint(position.x(), position.y() + SCALEHANDLE_DISTANCE);
    painter.drawPoint(position.x() + SCALEHANDLE_DISTANCE, position.y());
    painter.drawPoint(position.x() + ROTATEHANDLE_RADIUS, position.y() + ROTATEHANDLE_RADIUS);
}

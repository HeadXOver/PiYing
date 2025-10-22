#pragma once

class QPainter;
class QPointF;

struct ClickBehavior {
    virtual void click(const QPointF& mouse) = 0;
    virtual ~ClickBehavior() = default;
};

struct MouseMoveBehavior {
    virtual void mouseMove(const QPointF& mouse) = 0;
    virtual ~MouseMoveBehavior() = default;
};

struct EscapeBehavior {
    virtual void escape() = 0;
    virtual ~EscapeBehavior() = default;
};

struct EnterBehavior {
    virtual void enter() = 0;
    virtual ~EnterBehavior() = default;
};

struct DeleteElementBehavior {
    virtual void deleteElement() = 0;
    virtual ~DeleteElementBehavior() = default;
};

struct ReleaseBehavior {
    virtual void release(const QPointF& mouse) = 0;
    virtual ~ReleaseBehavior() = default;
};

struct DrawBehavior {
    virtual void draw(QPainter& painter) = 0;
    virtual ~DrawBehavior() = default;
};
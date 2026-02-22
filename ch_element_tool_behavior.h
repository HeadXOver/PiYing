#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_CLICK_BEHAVIOR(ClassName, DataName) \
class ClassName final : public ClickBehavior \
{ \
public: \
    ClassName(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act(const QPointF& mouse) override { _data->click(mouse); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_MOVEMOUSE_BEHAVIOR(ClassName, DataName) \
class ClassName final : public MouseMoveBehavior \
{ \
public: \
    ClassName(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act(const QPointF& mouse) override { _data->mouse_move(mouse); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_ESCAPE_BEHAVIOR(ClassName, DataName) \
class ClassName final : public EscapeBehavior \
{ \
public: \
    ClassName(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act() override { _data->escape(); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_ENTER_BEHAVIOR(ClassName, DataName) \
class ClassName final : public EnterBehavior \
{ \
public: \
    ClassName(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act() override { _data->enter(); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_DELETE_BEHAVIOR(ClassName, DataName) \
class ClassName final : public DeleteElementBehavior \
{ \
public: \
    ClassName(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act() override { _data->delete_element(); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_DRAW_BEHAVIOR(ClassName, DataName) \
class ClassName final : public DrawBehavior \
{ \
public: \
    ClassName(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act() override { _data->draw(); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_RELEASE_BEHAVIOR(ClassName, DataName) \
class ClassName final : public ReleaseBehavior \
{ \
public: \
    ClassName(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act(const QPointF& mouse) override { _data->release(mouse); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

class QPointF;

struct ClickBehavior {
    virtual void act(const QPointF& mouse) = 0;
    virtual ~ClickBehavior() = default;
};

struct MouseMoveBehavior {
    virtual void act(const QPointF& mouse) = 0;
    virtual ~MouseMoveBehavior() = default;
};

struct EscapeBehavior {
    virtual void act() = 0;
    virtual ~EscapeBehavior() = default;
};

struct EnterBehavior {
    virtual void act() = 0;
    virtual ~EnterBehavior() = default;
};

struct DeleteElementBehavior {
    virtual void act() = 0;
    virtual ~DeleteElementBehavior() = default;
};

struct ReleaseBehavior {
    virtual void act(const QPointF& mouse) = 0;
    virtual ~ReleaseBehavior() = default;
};

struct DrawBehavior {
    virtual void act() = 0;
    virtual ~DrawBehavior() = default;
};

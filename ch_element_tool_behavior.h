#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_CLICK_BEHAVIOR(DataName) \
class DataName ## Click final : public ClickBehavior \
{ \
public: \
    DataName ## Click(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act(const QPointF& mouse) override { _data->click(mouse); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_MOVEMOUSE_BEHAVIOR(DataName) \
class DataName ## MoveMouse final : public MouseMoveBehavior \
{ \
public: \
    DataName ## MoveMouse(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act(const QPointF& mouse) override { _data->mouse_move(mouse); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_ESCAPE_BEHAVIOR(DataName) \
class DataName ## Escape final : public EscapeBehavior \
{ \
public: \
    DataName ## Escape(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act() override { _data->escape(); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_ENTER_BEHAVIOR(DataName) \
class DataName ## Enter final : public EnterBehavior \
{ \
public: \
    DataName ## Enter(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act() override { _data->enter(); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_DELETE_BEHAVIOR(DataName) \
class DataName ## Delete final : public DeleteElementBehavior \
{ \
public: \
    DataName ## Delete(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act() override { _data->delete_element(); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_DRAW_BEHAVIOR(DataName) \
class DataName ## Draw final : public DrawBehavior \
{ \
public: \
    DataName ## Draw(const std::shared_ptr<DataName>& data) : _data(data) {} \
    virtual void act() override { _data->draw(); } \
private: \
    const std::shared_ptr<DataName> _data; \
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIYING_RELEASE_BEHAVIOR(DataName) \
class DataName ## Release final : public ReleaseBehavior \
{ \
public: \
    DataName ## Release(const std::shared_ptr<DataName>& data) : _data(data) {} \
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

#pragma once

#include <memory>

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


struct PiYingVoidClick final : public ClickBehavior
{
    virtual void act(const QPointF& mouse) override {};
};

struct PiYingVoidMove final : public MouseMoveBehavior
{
    virtual void act(const QPointF& mouse) override {};
};

struct PiYingVoidRelease final : public ReleaseBehavior
{
    virtual void act(const QPointF& mouse) override {};
};

struct PiYingVoidEscape final : public EscapeBehavior
{
    virtual void act() override {};
};

struct PiYingVoidDelete final : public DeleteElementBehavior
{
    virtual void act() override {};
};

struct PiYingVoidEnter final : public EnterBehavior
{
    virtual void act() override {};
};

struct PiYingVoidDraw final : public DrawBehavior
{
    virtual void act() override {};
};

template<typename DataName>
class PiYingToolClick final : public ClickBehavior
{
public:
    PiYingToolClick(const std::shared_ptr<DataName>& data) : _data(data) {}
    virtual void act(const QPointF& mouse) override { _data->click(mouse); }
private:
    const std::shared_ptr<DataName> _data;
};

template<typename DataName>
class PiYingToolMouseMove final : public MouseMoveBehavior
{
public:
    PiYingToolMouseMove(const std::shared_ptr<DataName>& data) : _data(data) {}
    virtual void act(const QPointF& mouse) override { _data->mouse_move(mouse); }
private:
    const std::shared_ptr<DataName> _data;
};

template<typename DataName>
class PiYingToolRelease final : public ReleaseBehavior
{
public:
    PiYingToolRelease(const std::shared_ptr<DataName>& data) : _data(data) {}
    virtual void act(const QPointF& mouse) override { _data->release(mouse); }
private:
    const std::shared_ptr<DataName> _data;
};

template<typename DataName>
class PiYingToolEscape final : public EscapeBehavior
{
public:
    PiYingToolEscape(const std::shared_ptr<DataName>& data) : _data(data) {}
    virtual void act() override { _data->escape(); }
private:
    const std::shared_ptr<DataName> _data;
};

template<typename DataName>
class PiYingToolEnter final : public EnterBehavior
{
public:
    PiYingToolEnter(const std::shared_ptr<DataName>& data) : _data(data) {}
    virtual void act() override { _data->enter(); }
private:
    const std::shared_ptr<DataName> _data;
};

template<typename DataName>
class PiYingToolDelete final : public DeleteElementBehavior
{
public:
    PiYingToolDelete(const std::shared_ptr<DataName>& data) : _data(data) {}
    virtual void act() override { _data->delete_element(); }
private:
    const std::shared_ptr<DataName> _data;
};

template<typename DataName>
class PiYingToolDraw final : public DrawBehavior
{
public:
    PiYingToolDraw(const std::shared_ptr<DataName>& data) : _data(data) {}
    virtual void act() override { _data->draw(); }
private:
    const std::shared_ptr<DataName> _data;
};
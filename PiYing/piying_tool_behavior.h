#pragma once

#include <memory>

class QPointF;

enum class PosBehaviorType {
    VOID,
    CLICK,
    MOUSE_MOVE,
    RELEASE,
};

enum class ActBehaviorType {
    VOID,
    ENTER,
    ESCAPE,
    DELETE,
    DRAW,
};

namespace piying::tool {

    struct PosBehavior {
        virtual void act(const QPointF& mouse) = 0;
        virtual ~PosBehavior() = default;
    };

    struct ActBehavior {
        virtual void act() = 0;
        virtual ~ActBehavior() = default;
    };


    struct VoidPosBehavior final : public PosBehavior
    {
        virtual void act(const QPointF& mouse) override {};
    };

    struct VoidActBehavior final : public ActBehavior
    {
        virtual void act() override {};
    };

    template<typename DataName>
    class Click final : public PosBehavior
    {
    public:
        Click(const std::shared_ptr<DataName>& data) : _data(data) {}
        virtual void act(const QPointF& mouse) override { _data->click(mouse); }
    private:
        const std::shared_ptr<DataName> _data;
    };

    template<typename DataName>
    class MouseMove final : public PosBehavior
    {
    public:
        MouseMove(const std::shared_ptr<DataName>& data) : _data(data) {}
        virtual void act(const QPointF& mouse) override { _data->mouse_move(mouse); }
    private:
        const std::shared_ptr<DataName> _data;
    };

    template<typename DataName>
    class Release final : public PosBehavior
    {
    public:
        Release(const std::shared_ptr<DataName>& data) : _data(data) {}
        virtual void act(const QPointF& mouse) override { _data->release(mouse); }
    private:
        const std::shared_ptr<DataName> _data;
    };

    template<typename DataName>
    class Escape final : public ActBehavior
    {
    public:
        Escape(const std::shared_ptr<DataName>& data) : _data(data) {}
        virtual void act() override { _data->escape(); }
    private:
        const std::shared_ptr<DataName> _data;
    };

    template<typename DataName>
    class Enter final : public ActBehavior
    {
    public:
        Enter(const std::shared_ptr<DataName>& data) : _data(data) {}
        virtual void act() override { _data->enter(); }
    private:
        const std::shared_ptr<DataName> _data;
    };

    template<typename DataName>
    class Delete final : public ActBehavior
    {
    public:
        Delete(const std::shared_ptr<DataName>& data) : _data(data) {}
        virtual void act() override { _data->delete_element(); }
    private:
        const std::shared_ptr<DataName> _data;
    };

    template<typename DataName>
    class Draw final : public ActBehavior
    {
    public:
        Draw(const std::shared_ptr<DataName>& data) : _data(data) {}
        virtual void act() override { _data->draw(); }
    private:
        const std::shared_ptr<DataName> _data;
    };

}

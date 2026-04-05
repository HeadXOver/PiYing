#pragma once

#include <qobject>

class ViewData : public QObject
{
    Q_OBJECT
        Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

public:
    explicit ViewData(QObject* parent = nullptr) : QObject(parent), m_value(0.f) {}
    explicit ViewData(float value, QObject* parent = nullptr) : QObject(parent), m_value(value) {}
    inline float value() const noexcept { return m_value; }
    void setValue(float v);

signals:
    void valueChanged(float newValue);

private:
    float m_value;
};


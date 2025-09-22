#pragma once

#include<QObject>

class DataObject : public QObject
{
    Q_OBJECT
        Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

public:
    explicit DataObject(QObject* parent = nullptr) : QObject(parent), m_value(0.f) {}
    inline float value() const { return m_value; }
    void setValue(float v) { if (m_value != v) { m_value = v; emit valueChanged(v); } }

signals:
    void valueChanged(float newValue);

private:
    float m_value;
};


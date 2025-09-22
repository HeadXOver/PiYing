#pragma once

#include<QObject>

class ViewData : public QObject
{
    Q_OBJECT
        Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

public:
    explicit ViewData(QObject* parent = nullptr) : QObject(parent), m_value(0.f) {}
    inline float value() const { return m_value; }
    void setValue(float v) { if (m_value != v) { m_value = v; emit valueChanged(v); } }
    void setValue180(float v) {
        if (m_value != v) {
            int n = v;
            m_value = n % 360 + 
                v < 0.f ? (n - v) : (v - n);
            emit valueChanged(v);
        }
    }

signals:
    void valueChanged(float newValue);

private:
    float m_value;
};


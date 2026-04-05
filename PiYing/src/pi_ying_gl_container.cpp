#include "piYingGLContainer.h"

#include <QVBoxLayout>

PiYingGLContainer::PiYingGLContainer(QWidget& content, double ratio, QWidget* parent) : QWidget(parent), m_content(content), m_ratio(ratio)
{
    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->addWidget(&content);
    setLayout(lay);
}

void PiYingGLContainer::resizeEvent(QResizeEvent*)
{
    QRect r = rect();
    if (r.width() / static_cast<double>(r.height()) > m_ratio) {
        r.setWidth(static_cast<int>(r.height() * m_ratio));
    }
    else {
        r.setHeight(static_cast<int>(r.width() / m_ratio));
    }
    // 居中
    r.moveCenter(rect().center());

    m_content.setGeometry(r);
}

void PiYingGLContainer::setRatio(double r) {
    m_ratio = r; 
    resizeEvent(nullptr);
}
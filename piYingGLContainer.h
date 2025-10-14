#pragma once

#include <QWidget>

class PiYingGLContainer : public QWidget
{
    Q_OBJECT
public:
    PiYingGLContainer(QWidget* content, double ratio, QWidget* parent = nullptr);

    void setRatio(double r);

protected:
    void resizeEvent(QResizeEvent*) override;

private:
    QWidget* m_content;
    double m_ratio;
};
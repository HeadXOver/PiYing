#pragma once

#include <QDialog>

class QDoubleSpinBox;

class AskBgTransformDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AskBgTransformDialog(
        const QString& title,
        const float d[5],
        QWidget* parent = nullptr);

    bool getValues(float& v1, float& v2, float& v3, float& v4, float& v5);

private:
    QDoubleSpinBox* m_spin[5];
};
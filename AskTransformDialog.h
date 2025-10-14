#pragma once

#include <QDialog>

class QDoubleSpinBox;

class Ask3DoublesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit Ask3DoublesDialog(
        const QString& title,
        const QString s[5],
        const float d[5],
        QWidget* parent = nullptr);

    bool getValues(float& v1, float& v2, float& v3, float& v4, float& v5);

private:
    QDoubleSpinBox* m_spin[5];
};
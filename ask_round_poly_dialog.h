#pragma once

#include <QDialog>

class QSpinBox;

class AskRoundPolyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AskRoundPolyDialog(
        const QString& title,
        const float d[3],
        QWidget* parent = nullptr);

    bool getValues(float& radius, float& edge_count, float& initial_angle);

private:
    QSpinBox* m_spin[3];
};
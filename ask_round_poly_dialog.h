#pragma once

#include <QDialog>

class QSpinBox;

class AskRoundPolyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AskRoundPolyDialog(
        const QString& title,
        const int init[3],
        QWidget* parent = nullptr
    );

    bool getValues(int& radius, int& edge_count, int& initial_angle);

private:
    QSpinBox* m_spin[3];
};
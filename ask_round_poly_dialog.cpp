#include "ask_round_poly_dialog.h"

#include <qdialogbuttonbox>
#include <qspinbox>
#include <QFormLayout>

AskRoundPolyDialog::AskRoundPolyDialog(const QString& title, const int initial[3], QWidget* parent) : QDialog(parent)
{
    setWindowTitle(title);
    setModal(true);

    auto* lay = new QFormLayout(this);

    QString s[3] = { QString("半径"), QString("边数"), QString("初始角度") };

    for (int i = 0; i < 3; ++i) {
        m_spin[i] = new QSpinBox(this);
        m_spin[i]->setSingleStep(1);
        m_spin[i]->setValue(initial[i]);
        lay->addRow(s[i], m_spin[i]);
    }

    m_spin[0]->setRange(0, 99999);
    m_spin[1]->setRange(3, 100);
    m_spin[2]->setRange(-180, 180);

    auto* bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(bbox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(bbox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    lay->addRow(bbox);
}

bool AskRoundPolyDialog::getValues(int& radius, int& edge_count, int& initial_angle)
{
    if (exec() != QDialog::Accepted) return false;
    radius = m_spin[0]->value();
    edge_count = m_spin[1]->value();
    initial_angle = m_spin[2]->value();

    return true;
}
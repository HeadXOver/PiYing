#include "AskTransformDialog.h"

#include <qdialogbuttonbox>
#include <qdoublespinbox>
#include <QFormLayout>

Ask3DoublesDialog::Ask3DoublesDialog(const QString& title, const float d[5], QWidget* parent) : QDialog(parent)
{
    setWindowTitle(title);
    setModal(true);

    auto* lay = new QFormLayout(this);

    const QString s[5] = { QString("X位移"), QString("Y位移"), QString("旋转"), QString("X缩放"), QString("Y缩放") };
    for (int i = 0; i < 5; ++i) {
        m_spin[i] = new QDoubleSpinBox(this);
        m_spin[i]->setRange(-9999999, 9999999);
        m_spin[i]->setDecimals(6);
        m_spin[i]->setSingleStep(0.01);
        m_spin[i]->setValue(d[i]);
        lay->addRow(s[i], m_spin[i]);
    }

    auto* bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(bbox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(bbox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    lay->addRow(bbox);
}

bool Ask3DoublesDialog::getValues(float& v1, float& v2, float& v3, float& v4, float& v5)
{
    if (exec() != QDialog::Accepted) return false;
    v1 = (float)m_spin[0]->value();
    v2 = (float)m_spin[1]->value();
    v3 = (float)m_spin[2]->value();
    v4 = (float)m_spin[3]->value();
    v5 = (float)m_spin[4]->value();

    return true;
}
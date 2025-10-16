#include "RatioDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QButtonGroup>

RatioDialog::RatioDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("设置长宽比"));
    buildUi();
}

void RatioDialog::buildUi()
{
    auto* lay = new QVBoxLayout(this);

    /* 1. 预设 */
    m_presetGroup = new QButtonGroup(this);
    const QList<QPair<QString, Ratio>> presets = {
        {"16:9", {16,9}}, {"4:3", {4,3}},
        {"1:1", {1,1}},   {tr("自定义"), {0,0}}
    };
    auto* presetLay = new QHBoxLayout(this);
    for (int i = 0; i < presets.size(); ++i) {
        auto* rb = new QRadioButton(presets[i].first);
        m_presetGroup->addButton(rb, i);
        presetLay->addWidget(rb);
        if (i == 0) rb->setChecked(true);
    }
    lay->addLayout(presetLay);

    /* 2. 自定义输入 */
    auto* customLay = new QHBoxLayout(this);
    customLay->addWidget(new QLabel(tr("自定义")));
    m_customW = new QSpinBox(this);
    m_customH = new QSpinBox(this);
    m_customW->setRange(1, 9999); m_customW->setValue(16);
    m_customH->setRange(1, 9999); m_customH->setValue(9);
    customLay->addWidget(m_customW);
    customLay->addWidget(new QLabel(":", this));
    customLay->addWidget(m_customH);
    customLay->addStretch();
    lay->addLayout(customLay);

    /* 3. 按钮 */
    m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);
    lay->addWidget(m_buttons);

    /* 4. 信号 */
    connect(m_presetGroup,
        QOverload<int, bool>::of(&QButtonGroup::idToggled),
        this,
        &RatioDialog::onPresetToggled);
    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    onPresetToggled(0, true); // 初始化
}

void RatioDialog::onPresetToggled(int id, bool checked)
{
    if (!checked) return;
    bool custom = id == 3;
    m_customW->setEnabled(custom);
    m_customH->setEnabled(custom);
}

Ratio RatioDialog::ratio() const
{
    int idx = m_presetGroup->checkedId();
    if (idx < 3) { // 预设
        const QList<Ratio> presetRatios = { {16,9}, {4,3}, {1,1} };
        return presetRatios[idx];
    }
    return { m_customW->value(), m_customH->value() };
}

void RatioDialog::setRatio(const Ratio& r)
{
    m_customW->setValue(r.w);
    m_customH->setValue(r.h);
    /* 根据值回选单选按钮，略 */
}
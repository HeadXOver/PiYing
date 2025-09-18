#pragma once

#include <QDialog>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
class QSpinBox;
class QDialogButtonBox;
QT_END_NAMESPACE

struct Ratio {
    int w = 16;
    int h = 9;
};

class RatioDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RatioDialog(QWidget* parent = nullptr);

    Ratio ratio() const;          // 外界取结果
    void setRatio(const Ratio&); // 外界回显

private slots:
    void onPresetToggled(int id, bool checked);

private:
    void buildUi();

    QButtonGroup* m_presetGroup;
    QSpinBox* m_customW;
    QSpinBox* m_customH;
    QDialogButtonBox* m_buttons;
};
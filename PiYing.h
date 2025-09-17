#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PiYing.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QTreeWidget>
#include <QHBoxLayout>

#include "piYingGL.h"

class PiYing : public QMainWindow
{
    Q_OBJECT

public:
    PiYing(QWidget *parent = nullptr);
    ~PiYing();

private:
    Ui::PiYingClass ui;

    QMenu* menuFile;

    QAction* actionExit;

    PiYingGL* piYingGL = nullptr;
};


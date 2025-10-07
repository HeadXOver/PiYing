#pragma once

#include <QOpenGLTexture>
#include <vector>
#include <QIcon>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QStatusBar>
#include <QList>
#include <QSet>
#include <QMatrix4x4>
#include <QSplitter>

#include "ctrlSlideWidget.h"

enum class ChTexToolState {
    None,
    AddTriangle,
    RectSelectVert,
	AddPoly,
};

enum class MousePos {
	Inside,
	LeftTop,
	LeftBottom,
	RightTop,
	RightBottom,
	LeftEdge,
	RightEdge,
	TopEdge,
	BottomEdge,
	OutSide
};

enum class EditMode {
	Default,
	BackGround,
	characterTexture,
	characterSkeleton,
	controlSlide,
};
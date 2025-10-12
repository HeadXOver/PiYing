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

static const int MOVEHANDLE_LENTH = 50;
static const int ROTATEHANDLE_RADIUS = 100;
static const int SCALEHANDLE_DISTANCE = 120;
static const int HANDLE_ZONE = 10;

enum class ChTexToolState {
    None,
    AddTriangle,
	MoveVert,
    RectSelectVert,
	LibreSelectVert,
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

enum class ChElementEditMode {
	None,
	Move,
	MoveX,
	MoveY,
	Rotate,
	Scale,
	ScaleX,
	ScaleY,
};
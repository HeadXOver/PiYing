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
#include <QLabel>
#include <QList>
#include <QMatrix4x4>
#include <QSplitter>

enum class First2VertState {
    None,
    HalfPoint,
    HalfSelect,
    Full2Point,
    Full2Select,
    FullSelectPoint,
    FullPointSelect,
};

enum class ChTexToolState {
    None,
    AddTriangle,
    SelectVert,
	AddVert,
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
};
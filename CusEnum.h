#pragma once

#include <QOpenGLTexture>
#include <QMatrix4x4>
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
#include <QListWidget>
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

enum class ToolState {
    None,
    AddVert,
    SelectVert,
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
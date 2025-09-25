#pragma once

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
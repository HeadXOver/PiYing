#include "control_slider_toolbar.h"

#include <qtoolbar>

#include "tool_button.h"
#include "enum_character_texture_tool_state.h"
#include "piYingGL.h"

ControlSliderToolbar::ControlSliderToolbar() :
	_select(new ToolButton(":/PiYing/selectRectChVert_S.png", ":/PiYing/selectRectChVert.png", "selectRectChVert", CharacterToolState::RectSelectVertPart)),
	_add_vert_trace(new ToolButton(":/PiYing/addVertTrace_S.png", ":/PiYing/addVertTrace.png", "addVertTrace", CharacterToolState::AddVertTrace))
{
	connect(_select->action(), &QAction::triggered, this, [this] { click_select(); }); 
	connect(_add_vert_trace->action(), &QAction::triggered, this, [this] { click_add_vert_trace(); });
}

void ControlSliderToolbar::set_to_toolbar(QToolBar* toolbar)
{
	toolbar->clear();

	toolbar->addAction(_select->action());
	toolbar->addAction(_add_vert_trace->action());
}

void ControlSliderToolbar::remember_last_tool()
{
	if (_select->isSelect()) {
		PiYingGL::getInstance().setChTool(_select->toolState());
	}
	else if (_add_vert_trace->isSelect()) {
		PiYingGL::getInstance().setChTool(CharacterToolState::AddVertTrace);
	}
	else {
		PiYingGL::getInstance().setChTool(CharacterToolState::None);
	}
}

void ControlSliderToolbar::press_1()
{
	switch (PiYingGL::getInstance().ch_tool_state()) {
	case CharacterToolState::RectSelectTrianglePart: {
		_select->set_selected(":/PiYing/selectRectChVert_S.png");
		_select->set_unselected(":/PiYing/selectRectChVert.png");
		_select->set_toolState(CharacterToolState::RectSelectVertPart);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::RectSelectVertPart);
	}return;
	case CharacterToolState::LibreSelectTrianglePart: {
		_select->set_selected(":/PiYing/selectLibreChVert_S.png");
		_select->set_unselected(":/PiYing/selectLibreChVert.png");
		_select->set_toolState(CharacterToolState::LibreSelectVertPart);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectVertPart);
	}return;
	case CharacterToolState::LibreSelectVertPart:
	case CharacterToolState::RectSelectVertPart: return;
	default: {
		if (_select->toolState() == CharacterToolState::RectSelectTrianglePart) {
			_select->set_selected(":/PiYing/selectRectChVert_S.png");
			_select->set_unselected(":/PiYing/selectRectChVert.png");
			_select->set_toolState(CharacterToolState::RectSelectVertPart);
		}
		else if (_select->toolState() == CharacterToolState::LibreSelectTrianglePart) {
			_select->set_selected(":/PiYing/selectLibreChVert_S.png");
			_select->set_unselected(":/PiYing/selectLibreChVert.png");
			_select->set_toolState(CharacterToolState::LibreSelectVertPart);
		}

		_select->select();
		PiYingGL::getInstance().setChTool(_select->toolState());
	}
	}
}

void ControlSliderToolbar::press_3()
{
	switch (PiYingGL::getInstance().ch_tool_state()) {
	case CharacterToolState::RectSelectVertPart: {
		_select->set_selected(":/PiYing/rect_triangle_select_S.png");
		_select->set_unselected(":/PiYing/rect_triangle_select.png");
		_select->set_toolState(CharacterToolState::RectSelectTrianglePart);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::RectSelectTrianglePart);
	}break;
	case CharacterToolState::LibreSelectVertPart: {
		_select->set_selected(":/PiYing/libre_triangle_select_S.png");
		_select->set_unselected(":/PiYing/libre_triangle_select.png");
		_select->set_toolState(CharacterToolState::LibreSelectTrianglePart);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectTrianglePart);
	}break;
	case CharacterToolState::LibreSelectTrianglePart:
	case CharacterToolState::RectSelectTrianglePart: return;
	default: {
		if (_select->toolState() == CharacterToolState::RectSelectVertPart) {
			_select->set_selected(":/PiYing/rect_triangle_select_S.png");
			_select->set_unselected(":/PiYing/rect_triangle_select.png");
			_select->set_toolState(CharacterToolState::RectSelectTrianglePart);
		}
		else if (_select->toolState() == CharacterToolState::LibreSelectVertPart) {
			_select->set_selected(":/PiYing/libre_triangle_select_S.png");
			_select->set_unselected(":/PiYing/libre_triangle_select.png");
			_select->set_toolState(CharacterToolState::LibreSelectTrianglePart);
		}

		_select->select();
		PiYingGL::getInstance().setChTool(_select->toolState());
	}
	}
}

void ControlSliderToolbar::click_select()
{
	if (_select->isSelect()) {
		switch (_select->toolState()) {
		case CharacterToolState::RectSelectVertPart: {
			_select->set_selected(":/PiYing/selectLibreChVert_S.png");
			_select->set_unselected(":/PiYing/selectLibreChVert.png");
			_select->set_toolState(CharacterToolState::LibreSelectVertPart);
			_select->update();
			PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectVertPart);
		}return;
		case CharacterToolState::LibreSelectVertPart: {
			_select->set_selected(":/PiYing/selectRectChVert_S.png");
			_select->set_unselected(":/PiYing/selectRectChVert.png");
			_select->set_toolState(CharacterToolState::RectSelectVertPart);
			_select->unSelect();
			PiYingGL::getInstance().setChTool(CharacterToolState::None);
		}return;
		case CharacterToolState::RectSelectTrianglePart: {
			_select->set_selected(":/PiYing/libre_triangle_select_S.png");
			_select->set_unselected(":/PiYing/libre_triangle_select.png");
			_select->set_toolState(CharacterToolState::LibreSelectTrianglePart);
			_select->update();
			PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectTrianglePart);
		}return;
		case CharacterToolState::LibreSelectTrianglePart: {
			_select->set_selected(":/PiYing/rect_triangle_select_S.png");
			_select->set_unselected(":/PiYing/rect_triangle_select.png");
			_select->set_toolState(CharacterToolState::RectSelectTrianglePart);
			_select->unSelect();
			PiYingGL::getInstance().setChTool(CharacterToolState::None);
		}return;
		}

		assert(false);
	}

	_select->select();
	PiYingGL::getInstance().setChTool(_select->toolState());
}

void ControlSliderToolbar::click_add_vert_trace()
{
	if (_add_vert_trace->isSelect()) {
		_add_vert_trace->unSelect();
		PiYingGL::getInstance().setChTool(CharacterToolState::None);
		return;
	}

	_select->unSelect();
	_add_vert_trace->select();
	PiYingGL::getInstance().setChTool(CharacterToolState::AddVertTrace);
}

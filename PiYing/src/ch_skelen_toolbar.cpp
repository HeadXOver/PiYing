#include "ch_skelen_toolbar.h"

#include <qtoolbar>

#include "tool_button.h"
#include "enum_character_texture_tool_state.h"
#include "piYingGL.h"

ChSkelenToolbar::ChSkelenToolbar():
	_select(new ToolButton(":/PiYing/piying_icons/selectRectChVert_S.png", ":/PiYing/piying_icons/selectRectChVert.png", "selectRectChVert", CharacterToolState::RectSelectVert))
{
	connect(_select->action(), &QAction::triggered, this, [this] { click_select(); });
}

void ChSkelenToolbar::set_to_toolbar(QToolBar* toolbar)
{
	toolbar->clear();

	toolbar->addAction(_select->action());
}

void ChSkelenToolbar::remember_last_tool()
{
	if (_select->isSelect()) {
		PiYingGL::getInstance().setChTool(_select->toolState());
	}
	else {
		PiYingGL::getInstance().setChTool(CharacterToolState::None);
	}
}

void ChSkelenToolbar::press_1()
{
	switch (PiYingGL::getInstance().ch_tool_state()) {
	case CharacterToolState::RectSelectTriangle: {
		_select->set_selected(":/PiYing/piying_icons/selectRectChVert_S.png");
		_select->set_unselected(":/PiYing/piying_icons/selectRectChVert.png");
		_select->set_toolState(CharacterToolState::RectSelectVert);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::RectSelectVert);
	}return;
	case CharacterToolState::LibreSelectTriangle: {
		_select->set_selected(":/PiYing/piying_icons/selectLibreChVert_S.png");
		_select->set_unselected(":/PiYing/piying_icons/selectLibreChVert.png");
		_select->set_toolState(CharacterToolState::LibreSelectVert);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectVert);
	}return;
	case CharacterToolState::LibreSelectVert:
	case CharacterToolState::RectSelectVert: return;
	default: {
		if (_select->toolState() == CharacterToolState::RectSelectTriangle) {
			_select->set_selected(":/PiYing/piying_icons/selectRectChVert_S.png");
			_select->set_unselected(":/PiYing/piying_icons/selectRectChVert.png");
			_select->set_toolState(CharacterToolState::RectSelectVert);
		}
		else if (_select->toolState() == CharacterToolState::LibreSelectTriangle) {
			_select->set_selected(":/PiYing/piying_icons/selectLibreChVert_S.png");
			_select->set_unselected(":/PiYing/piying_icons/selectLibreChVert.png");
			_select->set_toolState(CharacterToolState::LibreSelectVert);
		}

		_select->select();
		PiYingGL::getInstance().setChTool(_select->toolState());
	}
	}
}

void ChSkelenToolbar::press_3()
{
	switch (PiYingGL::getInstance().ch_tool_state()) {
	case CharacterToolState::RectSelectVert: {
		_select->set_selected(":/PiYing/piying_icons/rect_triangle_select_S.png");
		_select->set_unselected(":/PiYing/piying_icons/rect_triangle_select.png");
		_select->set_toolState(CharacterToolState::RectSelectTriangle);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::RectSelectTriangle);
	}break;
	case CharacterToolState::LibreSelectVert: {
		_select->set_selected(":/PiYing/piying_icons/libre_triangle_select_S.png");
		_select->set_unselected(":/PiYing/piying_icons/libre_triangle_select.png");
		_select->set_toolState(CharacterToolState::LibreSelectTriangle);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectTriangle);
	}break;
	case CharacterToolState::LibreSelectTriangle:
	case CharacterToolState::RectSelectTriangle: return;
	default: {
		if (_select->toolState() == CharacterToolState::RectSelectVert) {
			_select->set_selected(":/PiYing/piying_icons/rect_triangle_select_S.png");
			_select->set_unselected(":/PiYing/piying_icons/rect_triangle_select.png");
			_select->set_toolState(CharacterToolState::RectSelectTriangle);
		}
		else if (_select->toolState() == CharacterToolState::LibreSelectVert) {
			_select->set_selected(":/PiYing/piying_icons/libre_triangle_select_S.png");
			_select->set_unselected(":/PiYing/piying_icons/libre_triangle_select.png");
			_select->set_toolState(CharacterToolState::LibreSelectTriangle);
		}

		_select->select();
		PiYingGL::getInstance().setChTool(_select->toolState());
	}
	}
}

void ChSkelenToolbar::click_select()
{
	if (_select->isSelect()) {
		switch (_select->toolState()) {
		case CharacterToolState::RectSelectVert: {
			_select->set_selected(":/PiYing/piying_icons/selectLibreChVert_S.png");
			_select->set_unselected(":/PiYing/piying_icons/selectLibreChVert.png");
			_select->set_toolState(CharacterToolState::LibreSelectVert);
			_select->update();
			PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectVert);
		}return;
		case CharacterToolState::LibreSelectVert: {
			_select->set_selected(":/PiYing/piying_icons/selectRectChVert_S.png");
			_select->set_unselected(":/PiYing/piying_icons/selectRectChVert.png");
			_select->set_toolState(CharacterToolState::RectSelectVert);
			_select->unSelect();
			PiYingGL::getInstance().setChTool(CharacterToolState::None);
		}return;
		case CharacterToolState::RectSelectTriangle: {
			_select->set_selected(":/PiYing/piying_icons/libre_triangle_select_S.png");
			_select->set_unselected(":/PiYing/piying_icons/libre_triangle_select.png");
			_select->set_toolState(CharacterToolState::LibreSelectTriangle);
			_select->update();
			PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectTriangle);
		}return;
		case CharacterToolState::LibreSelectTriangle: {
			_select->set_selected(":/PiYing/piying_icons/rect_triangle_select_S.png");
			_select->set_unselected(":/PiYing/piying_icons/rect_triangle_select.png");
			_select->set_toolState(CharacterToolState::RectSelectTriangle);
			_select->unSelect();
			PiYingGL::getInstance().setChTool(CharacterToolState::None);
		}return;
		}

		assert(false);
	}

	_select->select();
	PiYingGL::getInstance().setChTool(_select->toolState());
}
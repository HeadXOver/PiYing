#include "ch_texture_toolbar.h"

#include <qtoolbar>

#include "tool_button.h"
#include "enum_character_texture_tool_state.h"
#include "piYingGL.h"

ChTextureToolbar::ChTextureToolbar() :
	_select(new ToolButton(":/PiYing/selectRectChVert_S.png", ":/PiYing/selectRectChVert.png", "selectRectChVert", CharacterToolState::RectSelectVert)),
	_add_triangle(new ToolButton(":/PiYing/addChVert_S.png", ":/PiYing/addChVert.png", "addChVert", CharacterToolState::AddTriangle)),
	_add_poly(new ToolButton(":/PiYing/chAddPoly_S.png", ":/PiYing/chAddPoly.png", "chAddPoly", CharacterToolState::AddPoly)),
	_add_round(new ToolButton(":/PiYing/chAddRound_S.png", ":/PiYing/chAddRound.png", "chAddRound", CharacterToolState::AddRound))
{
	connect(_select->action(), &QAction::triggered, this, [this] { click_select(); });
	connect(_add_triangle->action(), &QAction::triggered, this, [this] { click_add_triangle(); });
	connect(_add_poly->action(), &QAction::triggered, this, [this] { click_add_poly(); });
	connect(_add_round->action(), &QAction::triggered, this, [this] { click_add_round(); });
}

void ChTextureToolbar::set_to_toolbar(QToolBar* toolbar)
{
	toolbar->clear();

	toolbar->addAction(_select->action());
	toolbar->addAction(_add_triangle->action());
	toolbar->addAction(_add_poly->action());
	toolbar->addAction(_add_round->action());
}

void ChTextureToolbar::remember_last_tool()
{
	if (_select->isSelect()) {
		PiYingGL::getInstance().setChTool(_select->toolState());
	}
	else if (_add_triangle->isSelect()) {
		PiYingGL::getInstance().setChTool(CharacterToolState::AddTriangle);
	}
	else if (_add_poly->isSelect()) {
		PiYingGL::getInstance().setChTool(CharacterToolState::AddPoly);
	}
	else if (_add_round->isSelect()) {
		PiYingGL::getInstance().setChTool(CharacterToolState::AddRound);
	}
	else {
		PiYingGL::getInstance().setChTool(CharacterToolState::None);
	}
}

void ChTextureToolbar::press_1()
{
	switch (PiYingGL::getInstance().ch_tool_state()) {
	case CharacterToolState::RectSelectTriangle: {
		_select->set_selected(":/PiYing/selectRectChVert_S.png");
		_select->set_unselected(":/PiYing/selectRectChVert.png");
		_select->set_toolState(CharacterToolState::RectSelectVert);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::RectSelectVert);
	}return;
	case CharacterToolState::LibreSelectTriangle: {
		_select->set_selected(":/PiYing/selectLibreChVert_S.png");
		_select->set_unselected(":/PiYing/selectLibreChVert.png");
		_select->set_toolState(CharacterToolState::LibreSelectVert);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectVert);
	}return;
	case CharacterToolState::LibreSelectVert:
	case CharacterToolState::RectSelectVert: return;
	default: {
		if (_select->toolState() == CharacterToolState::RectSelectTriangle) {
			_select->set_selected(":/PiYing/selectRectChVert_S.png");
			_select->set_unselected(":/PiYing/selectRectChVert.png");
			_select->set_toolState(CharacterToolState::RectSelectVert);
		}
		else if (_select->toolState() == CharacterToolState::LibreSelectTriangle) {
			_select->set_selected(":/PiYing/selectLibreChVert_S.png");
			_select->set_unselected(":/PiYing/selectLibreChVert.png");
			_select->set_toolState(CharacterToolState::LibreSelectVert);
		}

		_add_triangle->unSelect();
		_add_poly->unSelect();
		_add_round->unSelect();
		_select->select();
		PiYingGL::getInstance().setChTool(_select->toolState());
	}
	}
}

void ChTextureToolbar::press_3()
{
	switch (PiYingGL::getInstance().ch_tool_state()) {
	case CharacterToolState::RectSelectVert: {
		_select->set_selected(":/PiYing/rect_triangle_select_S.png");
		_select->set_unselected(":/PiYing/rect_triangle_select.png");
		_select->set_toolState(CharacterToolState::RectSelectTriangle);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::RectSelectTriangle);
	}break;
	case CharacterToolState::LibreSelectVert: {
		_select->set_selected(":/PiYing/libre_triangle_select_S.png");
		_select->set_unselected(":/PiYing/libre_triangle_select.png");
		_select->set_toolState(CharacterToolState::LibreSelectTriangle);
		_select->update();
		PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectTriangle);
	}break;
	case CharacterToolState::LibreSelectTriangle:
	case CharacterToolState::RectSelectTriangle: return;
	default: {
		if (_select->toolState() == CharacterToolState::RectSelectVert) {
			_select->set_selected(":/PiYing/rect_triangle_select_S.png");
			_select->set_unselected(":/PiYing/rect_triangle_select.png");
			_select->set_toolState(CharacterToolState::RectSelectTriangle);
		}
		else if (_select->toolState() == CharacterToolState::LibreSelectVert) {
			_select->set_selected(":/PiYing/libre_triangle_select_S.png");
			_select->set_unselected(":/PiYing/libre_triangle_select.png");
			_select->set_toolState(CharacterToolState::LibreSelectTriangle);
		}

		_add_triangle->unSelect();
		_add_poly->unSelect();
		_add_round->unSelect();
		_select->select();
		PiYingGL::getInstance().setChTool(_select->toolState());
	}
	}
}

void ChTextureToolbar::click_select()
{
	if (_select->isSelect()) {
		switch (_select->toolState()) {
		case CharacterToolState::RectSelectVert: {
			_select->set_selected(":/PiYing/selectLibreChVert_S.png");
			_select->set_unselected(":/PiYing/selectLibreChVert.png");
			_select->set_toolState(CharacterToolState::LibreSelectVert);
			_select->update();
			PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectVert);
		}return;
		case CharacterToolState::LibreSelectVert: {
			_select->set_selected(":/PiYing/selectRectChVert_S.png");
			_select->set_unselected(":/PiYing/selectRectChVert.png");
			_select->set_toolState(CharacterToolState::RectSelectVert);
			_select->unSelect();
			PiYingGL::getInstance().setChTool(CharacterToolState::None);
		}return;
		case CharacterToolState::RectSelectTriangle: {
			_select->set_selected(":/PiYing/libre_triangle_select_S.png");
			_select->set_unselected(":/PiYing/libre_triangle_select.png");
			_select->set_toolState(CharacterToolState::LibreSelectTriangle);
			_select->update();
			PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectTriangle);
		}return;
		case CharacterToolState::LibreSelectTriangle: {
			_select->set_selected(":/PiYing/rect_triangle_select_S.png");
			_select->set_unselected(":/PiYing/rect_triangle_select.png");
			_select->set_toolState(CharacterToolState::RectSelectTriangle);
			_select->unSelect();
			PiYingGL::getInstance().setChTool(CharacterToolState::None);
		}return;
		}

		assert(false);
	}

	_add_triangle->unSelect();
	_add_poly->unSelect();
	_add_round->unSelect();
	_select->select();
	PiYingGL::getInstance().setChTool(_select->toolState());
}

void ChTextureToolbar::click_add_triangle()
{
	if (_add_triangle->isSelect()) {
		_add_triangle->unSelect();
		PiYingGL::getInstance().setChTool(CharacterToolState::None);
		return;
	}

	_select->unSelect();
	_add_poly->unSelect();
	_add_round->unSelect();
	_add_triangle->select();
	PiYingGL::getInstance().setChTool(CharacterToolState::AddTriangle);
}

void ChTextureToolbar::click_add_poly()
{
	if (_add_poly->isSelect()) {
		_add_poly->unSelect();
		PiYingGL::getInstance().setChTool(CharacterToolState::None);
		return;
	}

	_select->unSelect();
	_add_triangle->unSelect();
	_add_round->unSelect();
	_add_poly->select();
	PiYingGL::getInstance().setChTool(CharacterToolState::AddPoly);
}

void ChTextureToolbar::click_add_round()
{
	if (_add_round->isSelect()) {
		_add_round->unSelect();
		PiYingGL::getInstance().setChTool(CharacterToolState::None);
		return;
	}

	_select->unSelect();
	_add_triangle->unSelect();
	_add_poly->unSelect();
	_add_round->select();
	PiYingGL::getInstance().setChTool(CharacterToolState::AddRound);
}

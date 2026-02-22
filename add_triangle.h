#pragma once

#include "ch_element_tool_behavior.h"

#include <memory>
#include <qpointf>

/// <summary>
/// @brief 添加一个三角形
/// @details 点击三次，添加一个三角形
/// </summary>
struct AddTriangle final
{
	bool addVert(unsigned int i) noexcept;
	bool checkPointRepeat(const QPointF& point) const noexcept;

	void addVert(const QPointF& point) noexcept;
	void reduceOne() noexcept;
	void click(const QPointF& mouse);
	void draw();

	QPointF first;
	QPointF second;

	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;

	bool vertThenInd = false;

	int numVert = 0;
	int numInd = 0;
};

////////////////////////////////////////////////////////

class AddTriangleEscape final : public EscapeBehavior
{
public:
	AddTriangleEscape(const std::shared_ptr<AddTriangle>& add) : addTriangle(add) {}
	virtual void escape() override;

private:
	const std::shared_ptr<AddTriangle> addTriangle;
};

////////////////////////////////////////////////////////

class AddTriangleDelete final : public DeleteElementBehavior
{
public:
	AddTriangleDelete(const std::shared_ptr<AddTriangle>& add) : addTriangle(add) {}
	virtual void delete_element() override;

private:
	const std::shared_ptr<AddTriangle> addTriangle;
};

////////////////////////////////////////////////////////

class AddTriangleClick final : public ClickBehavior
{
public:
	AddTriangleClick(const std::shared_ptr<AddTriangle>& add) : addTriangle(add) {}
	virtual void click(const QPointF& mouse) override;

private:
	const std::shared_ptr<AddTriangle> addTriangle;
};

////////////////////////////////////////////////////////

class AddTriangleDraw final : public DrawBehavior
{
public:
	AddTriangleDraw(const std::shared_ptr<AddTriangle>& add) : addTriangle(add) {}
	virtual void draw() override;

private:
	const std::shared_ptr<AddTriangle> addTriangle;
};
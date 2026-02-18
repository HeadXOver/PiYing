#pragma once

struct ScaleTrans 
{
	ScaleTrans() = default;
	ScaleTrans(const ScaleTrans& other) = default;
	ScaleTrans& operator=(const ScaleTrans& other) = default;

	float scale_height{ 1.f };
	float scale_lenth{ 1.f };
	float trans_x{ 0.f };
	float trans_y{ 0.f };
};
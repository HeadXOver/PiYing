#pragma once

struct ScaleTrans 
{
	void operator=(const ScaleTrans& other);

	float scale_height{ 1.f };
	float scale_lenth{ 1.f };
	float trans_x{ 0.f };
	float trans_y{ 0.f };
};
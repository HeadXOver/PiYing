#pragma once

#include <vector>
#include <memory>

class PiYingGL;
class PointVectorLayer;
class TimelineGl;

inline PiYingGL* piYingGL = nullptr;
inline TimelineGl* timelineGl = nullptr;

inline PointVectorLayer* currentLayer = nullptr;
inline std::vector<unsigned int>* currentIndex = nullptr;
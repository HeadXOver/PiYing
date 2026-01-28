#pragma once

#include <vector>
#include <memory>

class PiYingGL;
class PiYing;
class PointVectorLayer;
class Part;
class TimelineGl;

class Parts;

inline PiYingGL* piYingGL = nullptr;
inline TimelineGl* timelineGl = nullptr;

inline PointVectorLayer* currentLayer = nullptr;
inline std::vector<unsigned int>* currentIndex = nullptr;
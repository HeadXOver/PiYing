#pragma once

#include <vector>

class PiYingGL;
class PointVectorLayer;
class Part;
class TimelineGl;

inline PointVectorLayer* currentLayer = nullptr;

inline PiYingGL* piYingGL = nullptr;

inline TimelineGl* timelineGl = nullptr;

inline std::vector<unsigned int>* currentIndex = nullptr;

inline std::vector<Part*> parts;
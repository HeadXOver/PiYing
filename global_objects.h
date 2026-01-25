#pragma once

#include <vector>
#include <memory>

class PiYingGL;
class PiYing;
class PointVectorLayer;
class Part;
class TimelineGl;

class Parts;

void g_init();

inline PiYingGL* piYingGL = nullptr;
inline PiYing* piYing = nullptr;
inline TimelineGl* timelineGl = nullptr;

inline PointVectorLayer* currentLayer = nullptr;
inline std::vector<unsigned int>* currentIndex = nullptr;

inline Parts* parts;
#pragma once

#include <vector>
#include <memory>

class PiYingGL;
class PointVectorLayer;

inline PiYingGL* piYingGL = nullptr;

inline PointVectorLayer* currentLayer = nullptr;
inline std::vector<unsigned int>* currentIndex = nullptr;
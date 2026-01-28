#pragma once

#include <vector>
#include <memory>

class PointVectorLayer;

inline PointVectorLayer* currentLayer = nullptr;
inline std::vector<unsigned int>* currentIndex = nullptr;
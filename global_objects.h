#pragma once

#include <vector>

class PiYingGL;
class PointVectorLayer;
class Part;

inline PointVectorLayer* currentLayer = nullptr;

inline PiYingGL* piYingGL = nullptr;

inline std::vector<unsigned int>* currentIndex = nullptr;

inline std::vector<Part*> parts;
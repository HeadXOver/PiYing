#include "ChElementTool.h"
#include "piYing.h"

ChElementTool::ChElementTool(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v, PiYingGL* pygl) :glIndex(ind), glVert(v), gl(pygl)
{
	currentVector = gl->getChCurrentRow();
}

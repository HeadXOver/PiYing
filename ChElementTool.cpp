#include "ChElementTool.h"
#include "piYing.h"

ChElementTool::ChElementTool(int current, PiYingGL* pygl) :
	glIndex(pygl->ref_characterTriangleIndices()[current]),
	glVert(pygl->ref_characterVerts()[current]),
	sVert(pygl->ref_characterDrawVerts()[current]),
	gl(pygl)
{
}

void ChElementTool::addPointToVert(const QPointF& p)
{
	glVert.push_back(p.x());
	glVert.push_back(p.y());
	sVert.push_back(p);
}

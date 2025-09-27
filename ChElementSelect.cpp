#include "ChElementSelect.h"

void ChElementSelect::deleteElement(int currentVector)
{
    std::vector<unsigned int>& idx = glIndex[currentVector];
    std::vector <float>& vert = glVert[currentVector];
    const size_t nVert = vert.size() / 2;
    const size_t nTri = idx.size() / 3;

    std::vector<bool> killVert(nVert, false);
    std::vector<bool> killTri(nTri, false);
    for (unsigned v : index) killVert[v] = true;

    std::vector<unsigned> refCount(nVert, 0);
    for (size_t t = 0; t < nTri; ++t) {
        unsigned v0 = idx[t * 3], v1 = idx[t * 3 + 1], v2 = idx[t * 3 + 2];
        if (killVert[v0] || killVert[v1] || killVert[v2]) {
            killTri[t] = true;
        }
        else {
            refCount[v0]++;
            refCount[v1]++;
            refCount[v2]++;
        }
    }

    for (size_t t = 0; t < nTri; ++t) {
        unsigned v0 = idx[t * 3], v1 = idx[t * 3 + 1], v2 = idx[t * 3 + 2];
        if (killTri[t] == true) {
            if (!killVert[v1] && refCount[v1] == 0) killVert[v1] = true;
            if (!killVert[v2] && refCount[v2] == 0) killVert[v2] = true;
            if (!killVert[v0] && refCount[v0] == 0) killVert[v0] = true;
        }
    }

    std::vector<unsigned> old2new(nVert, UINT_MAX);
    unsigned newVertCount = 0;
    for (unsigned old = 0; old < nVert; ++old) {
        if (!killVert[old]) {
            old2new[old] = newVertCount++;
            vert[newVertCount * 2 - 2] = vert[old * 2];
            vert[newVertCount * 2 - 1] = vert[old * 2 + 1];
        }
    }
    if (newVertCount == 0) vert.clear();
    else vert.resize(newVertCount * 2);

    size_t outIdx = 0;
    for (size_t t = 0; t < nTri; ++t) {
        if (killTri[t]) continue;
        unsigned v0 = idx[t * 3], v1 = idx[t * 3 + 1], v2 = idx[t * 3 + 2];
        idx[outIdx++] = old2new[v0];
        idx[outIdx++] = old2new[v1];
        idx[outIdx++] = old2new[v2];
    }
    if (outIdx != 0) idx.resize(outIdx);
    else idx.clear();

    index.clear();
}

void ChElementSelect::clickPos(const QPointF& mouse, float viewScale, int currentVector)
{
	for (unsigned int i = 0; i < glVert[currentVector].size() / 2; i++) {
		QPointF readyPoint(glVert[currentVector][i + i], glVert[currentVector][i + i + 1]);
		if (QLineF(readyPoint, mouse).length() < 0.02f / viewScale && !index.contains(i)) {
			index.append(i);
			return;
		}
	}

	index.clear();
}

QList<QPointF> ChElementSelect::getToDrawVert(int currentVector)
{
	QList<QPointF> vert;
	for (int i = 0; i < index.size(); i++) {
		vert.append(QPointF(glVert[currentVector][index[i] + index[i]], glVert[currentVector][index[i] + index[i] + 1]));
	}
	return vert;
}

#include "ChElementSelect.h"
#include "KeyboardStateWin.h"
#include "piYingGL.h"

void ChElementSelect::deleteElement()
{
    std::vector<unsigned int>& idx = glIndex;
    std::vector <float>& vert = glVert;
    QList<QPointF>& s_Vert = sVert;
    const size_t nVert = s_Vert.size();
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
            vert[newVertCount + newVertCount - 2] = vert[old + old];
            vert[newVertCount + newVertCount - 1] = vert[old + old + 1];
            s_Vert[newVertCount - 1] = s_Vert[old];
        }
    }
    if (newVertCount == 0) {
        vert.clear();
        s_Vert.clear();
    }
    else {
        vert.resize(newVertCount * 2);
        s_Vert.resize(newVertCount);
    }

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

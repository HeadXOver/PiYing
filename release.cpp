#include "piYingGL.h"

PiYingGL::~PiYingGL()
{
	makeCurrent();

	////////////////////////////////////////

	glDeleteBuffers(1, &bgVBO);
	glDeleteVertexArrays(1, &bgVAO);
	glDeleteBuffers(1, &bgEBO);

	////////////////////////////////////////

	doneCurrent();

	for (ImageTexture* ch : characterTextures) delete ch;
	for (ImageTexture* bg : backGrounds) delete bg;
	for (PointVector* pv : characterVerts) delete pv;
}
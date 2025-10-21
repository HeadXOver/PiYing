#include "piYingGL.h"

#include <QOpenGLShaderProgram.h>

void PiYingGL::initializeGL()
{
	initializeOpenGLFunctions();

	//////////////initialize background///////////////////////

	glGenVertexArrays(1, &bgVAO);
	glGenBuffers(1, &bgVBO);
	glGenBuffers(1, &bgEBO);

	/////////////////////////////////////////////

	glBindVertexArray(bgVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(RECTANGLE_VERT), RECTANGLE_VERT, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDECES), RECTANGLE_INDECES, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	bgShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/bgshapes.vert");
	bgShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/bgshapes.frag");
	bgShaderProgram->link();
	bgShaderProgram->setUniformValue("texture1", 0);

	/////////////////////////////////////////////

	////////////////initialize character editer///////////////////////

	glGenVertexArrays(1, &chVAO);
	glGenBuffers(1, &chVBO);
	glGenBuffers(1, &chEBO);

	glBindVertexArray(chVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	/////////////////////////////////////////////

	chShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/chEditershapes.vert");
	chShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/chEditershapes.frag");
	chShaderProgram->link();
	chShaderProgram->setUniformValue("texture1", 0);

	/////////////////////////////////////////////

	// global setting

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
}

void PiYingGL::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);
	if (editMode == EditMode::BackGround || editMode == EditMode::OverView)	paintBackgrounds();
	else if (editMode == EditMode::characterTexture)						paintCharacterTexture();
	else if (editMode == EditMode::characterSkeleton) {
		paint_applied_texture();
		if (ch_tool_state_ != CharacterToolState::LibreSelectVert && ch_tool_state_ != CharacterToolState::RectSelectVert) return;
		draw_ch_applied_vert();
	}
	else if (editMode == EditMode::controlSlide) {
		paint_applied_texture();
		if (ch_tool_state_ != CharacterToolState::AddVertTrace) return;
		draw_ch_applied_vert();
	}
}

void PiYingGL::resizeGL(int w, int h) {

}

void PiYingGL::currentUpdate()
{
	makeCurrent();
	paintBackgrounds();
	update();
	doneCurrent();
}
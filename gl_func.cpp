#include "piYingGL.h"
#include "ch_element_tool.h"
#include "enum_edit_mode.h"
#include "point_vector.h"
#include "static_gl_const.h"

#include <QOpenGLShaderProgram.h>

void PiYingGL::initializeGL()
{
	initializeOpenGLFunctions();

	//////////////initialize rectangle///////////////////////

	glGenVertexArrays(1, &rtVAO);
	glGenBuffers(1, &rtVBO);

	/////////////////////////////////////////////

	glBindVertexArray(rtVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rtVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(RECTANGLE_VERT), RECTANGLE_VERT, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/////////////////////////////////////////////

	_rectangle_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/rectangle.vert");
	_rectangle_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/rectangle.frag");
	_rectangle_shader_program->link();

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

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOAT2, (void*)0);
	glEnableVertexAttribArray(0);

	/////////////////////////////////////////////

	bgShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/bgshapes.vert");
	bgShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/bgshapes.frag");
	bgShaderProgram->link();
	bgShaderProgram->setUniformValue("texture1", 0);

	////////////////initialize character editer///////////////////////

	glGenVertexArrays(1, &chVAO);
	glGenBuffers(1, &chVBO);
	glGenBuffers(1, &chEBO);

	glBindVertexArray(chVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOAT4, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, FLOAT4, DEBU_FLOAT2);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	/////////////////////////////////////////////

	chShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/chEditershapes.vert");
	chShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/chEditershapes.frag");
	chShaderProgram->link();

	//////////////initialize texture triangles///////////////////////

	glGenVertexArrays(1, &ttVAO);

	glGenBuffers(1, &ttEBO);

	glBindVertexArray(ttVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glLineWidth(3.0f);

	glBindVertexArray(0);

	/////////////////////////////////////////////

	_texture_tri_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/texture_tri.vert");
	_texture_tri_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/texture_tri.frag");
	_texture_tri_shader_program->link();

	/////////////////////////////////////////////

	//////////////initialize selected points///////////////////////

	glGenVertexArrays(1, &svVAO);
	glGenBuffers(1, &svVBO);

	glBindVertexArray(svVAO);
	glBindBuffer(GL_ARRAY_BUFFER, svVBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	_selected_vert_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/selected_vert.vert");
	_selected_vert_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/selected_vert.frag");
	_selected_vert_shader_program->link();

	/////////////////////////////////////////////

	// global setting
	chShaderProgram->bind();
	chShaderProgram->setUniformValue("trc", QMatrix4x4());
	_selected_vert_shader_program->bind();
	_selected_vert_shader_program->setUniformValue("trc", QMatrix4x4());
	_texture_tri_shader_program->bind();
	_texture_tri_shader_program->setUniformValue("trc", QMatrix4x4());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_DEPTH_TEST);   // 不需要深度测试
	glDisable(GL_CULL_FACE);    // 不需要背面剔除
}

void PiYingGL::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);
	if (editMode == EditMode::BackGround || editMode == EditMode::OverView)	paintBackgrounds();
	else if (editMode == EditMode::characterTexture)						paintCharacterTexture();
	else if (editMode == EditMode::characterSkeleton) {
		paint_applied_texture();
		if (_ch_tool_state != CharacterToolState::LibreSelectVert && 
			_ch_tool_state != CharacterToolState::RectSelectVert &&
			_ch_tool_state != CharacterToolState::RectSelectTriangle&&
			_ch_tool_state != CharacterToolState::LibreSelectTriangle
			) return;
		draw_triangle_frame(true);
		if (ch_element_tool_) ch_element_tool_->draw();
	}
	else if (editMode == EditMode::controlSlide) {
		paint_selected_part();
		if (ch_element_tool_) ch_element_tool_->draw();
	}
	else return;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void PiYingGL::generate_ebo(std::vector<unsigned int>& indices, unsigned int& ebo)
{
	makeCurrent();

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	doneCurrent();
}

void PiYingGL::generate_vao(unsigned int& vao, unsigned int vbo, unsigned int ebo)
{
	makeCurrent();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
	doneCurrent();
}
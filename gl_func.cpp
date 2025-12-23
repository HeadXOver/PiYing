#include "piYingGL.h"
#include "ch_element_tool.h"
#include "enum_edit_mode.h"
#include "point_vector.h"
#include "static_gl_const.h"

#include <QOpenGLShaderProgram.h>

void PiYingGL::initializeGL()
{
	initializeOpenGLFunctions();

	/// programmes
	_rectangle_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/bgshapes.vert");
	_rectangle_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/color_shape.frag");
	_rectangle_shader_program->link();

	_texture_color_shader_programme->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/bgshapes.vert");
	_texture_color_shader_programme->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/texture_color_shape.frag");
	_texture_color_shader_programme->link();

	chShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/chEditershapes.vert");
	chShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/texture_color_shape.frag");
	chShaderProgram->link();

	_texture_tri_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/texture_tri.vert");
	_texture_tri_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/color_shape.frag");
	_texture_tri_shader_program->link();

	_selected_vert_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/selected_vert.vert");
	_selected_vert_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/color_shape.frag");
	_selected_vert_shader_program->link();

	/// generate VAO
	glGenVertexArrays(1, &RECTANGLE_TEXTURE_VAO);
	glGenVertexArrays(1, &chVAO);
	glGenVertexArrays(1, &ttVAO);
	glGenVertexArrays(1, &svVAO);

	/// generate VBO
	glGenBuffers(1, &RECTANGLE_TEXTURE_VBO);
	glGenBuffers(1, &chVBO);
	glGenBuffers(1, &svVBO);

	/// generate EBO
	glGenBuffers(1, &RECTANGLE_TEXTURE_EBO);
	glGenBuffers(1, &chEBO);
	glGenBuffers(1, &ttEBO);

	/// buffer data vbo
	glBindBuffer(GL_ARRAY_BUFFER, RECTANGLE_TEXTURE_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RECTANGLE_VERT), RECTANGLE_VERT, GL_STATIC_DRAW);

	/// buffer data ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RECTANGLE_TEXTURE_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDECES), RECTANGLE_INDECES, GL_STATIC_DRAW);

	//////////////initialize rect texture///////////////////////

	glBindVertexArray(RECTANGLE_TEXTURE_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, RECTANGLE_TEXTURE_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RECTANGLE_TEXTURE_EBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOAT2, (void*)0);
	glEnableVertexAttribArray(0);

	////////////////initialize character editer///////////////////////

	glBindVertexArray(chVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOAT4, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, FLOAT4, DEBU_FLOAT2);
	glEnableVertexAttribArray(1);

	//////////////initialize texture triangles///////////////////////

	glBindVertexArray(ttVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOAT4, DEBU_FLOAT2);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, FLOAT4, (void*)0);
	glEnableVertexAttribArray(1);

	//////////////initialize selected points///////////////////////

	glBindVertexArray(svVAO);
	glBindBuffer(GL_ARRAY_BUFFER, svVBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOAT2, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); ///< end initialize

	/////////////////////global setting////////////////////////

	chShaderProgram->bind();
	chShaderProgram->setUniformValue("trc", QMatrix4x4());
	_selected_vert_shader_program->bind();
	_selected_vert_shader_program->setUniformValue("trc", QMatrix4x4());
	_texture_tri_shader_program->bind();
	_texture_tri_shader_program->setUniformValue("trc", QMatrix4x4());
	_rectangle_shader_program->bind();
	_rectangle_shader_program->setUniformValue("aColor", QVector4D(0.2f, 0.2f, 1.0f, 1.0f)); 


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glLineWidth(3.0f);
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
#include "time_line_gl.h"

#include "static_gl_const.h"
#include "point_vector.h"
#include "piYingGL.h"
#include "global_objects.h"

#include <QOpenGLShaderProgram>
#include <qimage>
#include <qopengltexture>

void TimelineGl::initializeGL()
{
	if (_texture) return;

	initializeOpenGLFunctions();
	glClearColor(0.1f, 0.1f, 0.2f, 0.4f);
	_texture = std::make_unique<QOpenGLTexture>(QImage(":/PiYing/timeline.png").flipped());
	_texture->setWrapMode(QOpenGLTexture::Repeat);

	//////////////initialize timeline rectangle///////////////////////

	glGenVertexArrays(1, &tlVAO);
	glGenBuffers(1, &tlVBO);
	glGenBuffers(1, &rectEBO);

	//////////////initialize simple rectangle///////////////////////

	glGenVertexArrays(1, &sVAO);
	glGenBuffers(1, &sVBO);

	//////////////initialize simple with trans rectangle///////////////////////

	glGenVertexArrays(1, &tVAO);
	glGenBuffers(1, &tVBO);

	/////////////////////////////////////////////

	glBindVertexArray(tlVAO);
	glBindBuffer(GL_ARRAY_BUFFER, tlVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(RECTANGLE_VERT), RECTANGLE_VERT, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDECES), RECTANGLE_INDECES, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOAT2, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	/////////////////////////////////////////////

	constexpr float insert_bar_vert[] = {
		0.005f, 0.2f,
		0.005f, -0.2f,
		-0.005f, -0.2f,
		-0.005f, 0.2f
	};

	glBindVertexArray(tVAO);
	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(insert_bar_vert), insert_bar_vert, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOAT2, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	/////////////////////////////////////////////

	constexpr float scrollbar_vert[] = {
		1.0f,  1.0f,
		1.0f, -1.0f,
		1.0f - spTimelineGL::scroll_width, -1.0f,
		1.0f - spTimelineGL::scroll_width, 1.0f
	};

	glBindVertexArray(sVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(scrollbar_vert), scrollbar_vert, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOAT2, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	//////////////////////////////////////////////

	_timeline_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/timeline_rect_shape.vert");
	_timeline_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/texture_color_shape.frag");
	_timeline_shader_program->link();
	_timeline_shader_program->bind();
	_timeline_shader_program->setUniformValue("aColor", QVector4D(1.f, 1.f, 1.f, 1.f));

	//////////////////////////////////////////////

	_rect_select_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/part_select_rect.vert");
	_rect_select_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/color_shape.frag");
	_rect_select_program->link();
	_rect_select_program->bind();
	_rect_select_program->setUniformValue("aColor", QVector4D(0.6f, 0.3f, 0.3f, 1.0f));

	//////////////////////////////////////////////

	_part_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/square_icon.vert");
	_part_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/texture_color_shape.frag");
	_part_shader_program->link();
	_part_shader_program->bind();
	_rect_select_program->setUniformValue("aColor", QVector4D(1.f, 1.f, 1.f, 1.f));

	//////////////////////////////////////////////

	_simple_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/simple_triangle.vert");
	_simple_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/color_shape.frag");
	_simple_shader_program->link();
	_simple_shader_program->bind();
	_simple_shader_program->setUniformValue("aColor", QVector4D(0.2f, 0.6f, 0.6f, 0.8f));

	//////////////////////////////////////////////

	_simple_scroll_block_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/scroll_block.vert");
	_simple_scroll_block_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/color_shape.frag");
	_simple_scroll_block_program->link();
	_simple_scroll_block_program->bind();
	_simple_scroll_block_program->setUniformValue("aColor", QVector4D(0.3f, 0.8f, 0.6f, 0.8f));

	//////////////////////////////////////////////

	_simple_with_trans_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/vert_with_trans.vert");
	_simple_with_trans_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/color_shape.frag");
	_simple_with_trans_shader_program->link();
	_simple_with_trans_shader_program->bind();
	_simple_with_trans_shader_program->setUniformValue("aColor", QVector4D(0.7f, 0.7f, 0.3f, 1.f));

	// global setting

	glActiveTexture(GL_TEXTURE0);
}

void TimelineGl::resizeGL(int w, int h)
{
	_ratio = (width() * PiYingGL::getInstance().height()) / (float)(PiYingGL::getInstance().width() * height());
	_rect_select_program->bind();
	_rect_select_program->setUniformValue("ratio", _ratio);
	_part_shader_program->bind();
	_part_shader_program->setUniformValue("ratio", _ratio);
	_simple_with_trans_shader_program->bind();
	_simple_with_trans_shader_program->setUniformValue("ratio", _ratio);

	_part_cursor.update_after_resize();
}

void TimelineGl::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (_ui_type) {
	case spTimelineGL::UiType::Timeline: paint_timeline(); break;
	case spTimelineGL::UiType::Part: paint_parts(); break;
	}
}

void TimelineGl::generate_vbo(const PointVector& pointVector, unsigned int& vbo)
{
	makeCurrent();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pointVector.float_size() * sizeof(float), pointVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	doneCurrent();
}

void TimelineGl::update_vbo(const PointVector& pointVector, unsigned int vbo)
{
	makeCurrent();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pointVector.float_size() * sizeof(float), pointVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	doneCurrent();
}

void TimelineGl::update_ebo(const std::vector<unsigned int>& indices, const unsigned int ebo)
{
	makeCurrent();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	doneCurrent();
}

void TimelineGl::update_buffers(const PointVector& pointVector, const std::vector<unsigned int>& indices, unsigned int vbo, unsigned int ebo)
{
	makeCurrent();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pointVector.float_size() * sizeof(float), pointVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	doneCurrent();
}

void TimelineGl::generate_ebo(const std::vector<unsigned int>& indices, unsigned int& ebo)
{
	makeCurrent();

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	doneCurrent();
}

void TimelineGl::generate_vao(unsigned int& vao, unsigned int vbo, unsigned int ebo)
{
	makeCurrent();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOAT4, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, FLOAT4, DEBU_FLOAT2);
	glBindVertexArray(0);
	doneCurrent();
}
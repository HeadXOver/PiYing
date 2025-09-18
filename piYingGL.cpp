#include "piYingGL.h"

#include <QMessageBox>

unsigned int VAO = 0, VBO = 0, EBO = 0;

PiYingGL::PiYingGL(QWidget* parent) : QOpenGLWidget(parent)
{
	setAutoFillBackground(false);
}

PiYingGL::~PiYingGL()
{
	makeCurrent();

	////////////////////////////////////////

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	////////////////////////////////////////

	doneCurrent();
}

void PiYingGL::initializeGL()
{
	initializeOpenGLFunctions();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	/////////////////////////////////////////////

	shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/shapes.vert");
	shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/shapes.frag");
	shaderProgram.link();
	shaderProgram.bind();

	/////////////////////////////////////////////

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glClearColor(0.2f, 0.1f, 0.067f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PiYingGL::resizeGL(int w, int h){

}

void PiYingGL::paintGL(){
	glClear(GL_COLOR_BUFFER_BIT);
	for (ImageTexture &it : backGrounds) {
		shaderProgram.bind();
		glActiveTexture(GL_TEXTURE0);
		it.tex->bind();
		shaderProgram.setUniformValue("texture1", 0);
		shaderProgram.setUniformValue("trans", 0);
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(RECTANGLE_VERT), RECTANGLE_VERT, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDECES), RECTANGLE_INDECES, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void PiYingGL::addBackground(QString& imageName){
	QImage img;
	if (!img.load(imageName)) {
		QMessageBox::warning(this, "Warning", "Failed to load image: " + imageName);
		return;
	}
	backGrounds.append(img);
	update();
}

#include "piYingGL.h"

unsigned int VAO = 0, VBO = 0, EBO = 0;

PiYingGL::PiYingGL(QWidget* parent) : QOpenGLWidget(parent)
{
	setAutoFillBackground(false);
}

PiYingGL::~PiYingGL()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	if (cusTexture)
	{
		delete cusTexture;
		cusTexture = nullptr;
	}
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

	if (backGrounds.size() > 0) {
		if (cusTexture) {
			delete cusTexture;
			cusTexture = nullptr;
		}
		cusTexture = new QOpenGLTexture(backGrounds[0].flipped());
		cusTexture->setMinificationFilter(QOpenGLTexture::Linear);
		cusTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
		cusTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
		shaderProgram.bind();
		glActiveTexture(GL_TEXTURE0);
		cusTexture->bind();
		shaderProgram.setUniformValue("texture1", 0);
		float vertices[] = {
			// positions          // colors           // texture coords
			 1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
			 1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
			-1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left 
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void PiYingGL::addBackground(const QImage& img){
	backGrounds.append(img);
	update();
}

///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016 F4EXB                                                      //
// written by Edouard Griffiths                                                  //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QImage>
#include <QMatrix4x4>
#include <QVector4D>
#include <QDebug>

#include "gui/glshadertextured.h"

GLShaderTextured::GLShaderTextured() :
	m_program(0),
	m_textureId(0),
	m_matrixLoc(0),
	m_textureLoc(0)
{ }

GLShaderTextured::~GLShaderTextured()
{
	cleanup();
}

void GLShaderTextured::initializeGL()
{
	initializeOpenGLFunctions();

	m_program = new QOpenGLShaderProgram;

	if (!m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_vertexShaderSourceTextured)) {
		qDebug() << "GLShaderTextured::initializeGL: error in vertex shader: " << m_program->log();
	}

	if (!m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_fragmentShaderSourceTextured)) {
		qDebug() << "GLShaderTextured::initializeGL: error in fragment shader: " << m_program->log();
	}

	m_program->bindAttributeLocation("vertex", 0);
	m_program->bindAttributeLocation("texCoord", 1);

	if (!m_program->link()) {
		qDebug() << "GLShaderTextured::initializeGL: error linking shader: " << m_program->log();
	}

	m_program->bind();
	m_matrixLoc = m_program->uniformLocation("uMatrix");
	m_textureLoc = m_program->uniformLocation("uTexture");
	m_program->release();
}

void GLShaderTextured::initTexture(const QImage& image, QOpenGLTexture::WrapMode wrapMode)
{
	if (m_textureId) {
		glDeleteTextures(1, &m_textureId);
		m_textureId = 0;
	}

	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
		image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.constScanLine(0));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}

void GLShaderTextured::subTexture(int xOffset, int yOffset, int width, int height, const void *pixels)
{
	if (!m_textureId) {
		qDebug("GLShaderTextured::subTexture: no texture defined. Doing nothing");
		return;
	}

	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void GLShaderTextured::drawSurface(const QMatrix4x4& transformMatrix, GLfloat *textureCoords, GLfloat *vertices, int nbVertices)
{
	draw(GL_TRIANGLE_FAN, transformMatrix, textureCoords, vertices, nbVertices);
}

void GLShaderTextured::draw(unsigned int mode, const QMatrix4x4& transformMatrix, GLfloat *textureCoords, GLfloat *vertices, int nbVertices)
{
	if (!m_textureId) {
		qDebug("GLShaderTextured::draw: no texture defined. Doing nothing");
		return;
	}

	m_program->bind();
	m_program->setUniformValue(m_matrixLoc, transformMatrix);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	m_program->setUniformValue(m_textureLoc, 0); // Use texture unit 0 which magically contains our texture
	glEnableVertexAttribArray(0); // vertex
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(1); // texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, textureCoords);
	glDrawArrays(mode, 0, nbVertices);
	glDisableVertexAttribArray(0);
	m_program->release();
}

void GLShaderTextured::cleanup()
{
	if (m_program)	{
		delete m_program;
		m_program = 0;
	}

	if (m_textureId) {
		glDeleteTextures(1, &m_textureId);
		m_textureId = 0;
	}
}

const QString GLShaderTextured::m_vertexShaderSourceTextured = QString(
		"uniform highp mat4 uMatrix;\n"
		"attribute highp vec4 vertex;\n"
		"attribute highp vec2 texCoord;\n"
		"varying mediump vec2 texCoordVar;\n"
		"void main() {\n"
		"    gl_Position = uMatrix * vertex;\n"
		"    texCoordVar = texCoord;\n"
		"}\n"
		);

const QString GLShaderTextured::m_fragmentShaderSourceTextured = QString(
		"uniform lowp sampler2D uTexture;\n"
		"varying mediump vec2 texCoordVar;\n"
		"void main() {\n"
		"    gl_FragColor = texture2D(uTexture, texCoordVar);\n"
		"}\n"
		);

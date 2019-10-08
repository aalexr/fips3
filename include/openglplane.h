/*
 *  Copyright (C) 2017  Matwey V. Kornilov <matwey.kornilov@gmail.com>
 *                      Konstantin Malanchev <hombit@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _OPENGLPLANE_H
#define _OPENGLPLANE_H

#include <QObject>
#include <QSize>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <array>
#include <cmath>

#include "openglshaderprogram.h"

class OpenGLPlane: public QObject {
private:
	QOpenGLBuffer vertex_buffer_;
	QOpenGLBuffer UV_buffer_;
	QOpenGLBuffer index_buffer_;

	QSize image_size_;
	qreal scale_;

	static constexpr const int VERTICES_PER_SIDE = 32;

	std::vector<float> mesh_;
	std::vector<float> uv_;
	std::vector<unsigned int> indices_;

	void updateVertexArray(const QRectF& p, int nx, int ny);
	void updateScale();
	void updateUV(int nx, int ny);
	void setImageSize(const QSize& image_size);

	void updateIndices(int width, int height);

	bool initializeBufferHelper(QOpenGLBuffer& buffer, const void* data, int count, GLuint index);
	bool initializeVertexBuffer();
	bool initializeUVBuffer();
	bool initializeIndexBuffer();

public:
	OpenGLPlane(const QSize& image_size, QObject* parent = Q_NULLPTR);
	virtual ~OpenGLPlane() override;

	inline const qreal& scale() const { return scale_; }

	QRectF planeRect() const;
	QRectF borderRect(float angle) const;

	inline const QOpenGLBuffer& vertexCoordBuffer() const { return vertex_buffer_; }
	inline QOpenGLBuffer&       vertexCoordBuffer()       { return vertex_buffer_; }
	inline const QOpenGLBuffer& vertexUVBuffer() const { return UV_buffer_; }
	inline QOpenGLBuffer&       vertexUVBuffer()       { return UV_buffer_; }
	inline QOpenGLBuffer& indexBuffer() { return index_buffer_; }
	inline const QOpenGLBuffer& indexBuffer() const { return index_buffer_; }

	bool initialize();
};

#endif // _OPENGLPLANE_H

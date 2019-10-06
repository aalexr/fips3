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

#include <algorithm>

#include <QMatrix4x4>
#include <QRectF>

#include <openglplane.h>

OpenGLPlane::OpenGLPlane(const QSize& image_size, QObject* parent):
	QObject(parent),
	mesh_(),
	uv_(),
	indices_(),
	scale_{0},
	vertex_buffer_(QOpenGLBuffer::VertexBuffer),
	UV_buffer_(QOpenGLBuffer::VertexBuffer),
	index_buffer_(QOpenGLBuffer::IndexBuffer) {

	setImageSize(image_size);
}

OpenGLPlane::~OpenGLPlane() = default;

void OpenGLPlane::updateScale() {
	const auto w = image_size_.width();
	const auto h = image_size_.height();
	const qreal scale = static_cast<qreal>(1) / std::max(w,h);

	if (scale_ == scale) return;

	scale_ = scale;
}

void OpenGLPlane::updateVertexArray(const QRectF& p, int nx, int ny) {
	mesh_.clear();
	qreal dx = p.width() / (nx - 1), dy = p.height() / (ny - 1);
	for (auto i = 0; i < ny; ++i) {
		for (auto j = 0; j < nx; ++j) {
			mesh_.push_back(p.x() + i * dx);
			mesh_.push_back(p.y() + j * dy);
		}
	}
}

void OpenGLPlane::setImageSize(const QSize& image_size) {
	if (image_size_ == image_size) return;

	image_size_ = image_size;

	const auto p = planeRect();
	qreal w = p.width();
	qreal h = p.height();
	qreal delta = std::max(w, h) / static_cast<qreal>(VERTICES_PER_SIDE - 1);
	int nx = std::ceil(w / delta + 1);
	int ny = std::ceil(h / delta + 1);

	updateScale();
	updateVertexArray(p, nx, ny);
	updateUV(nx, ny);
	updateIndices(nx, ny);
}

QRectF OpenGLPlane::planeRect() const {
	const auto w = image_size_.width();
	const auto h = image_size_.height();
	const auto x = (w < h ? static_cast<qreal>(w)/h : static_cast<qreal>(1));
	const auto y = (w < h ? static_cast<qreal>(1) : static_cast<qreal>(h)/w);
	const QRectF p{QPointF{-x,-y}, QPointF{x,y}};

	return p;
}

QRectF OpenGLPlane::borderRect(float angle) const {
	const auto p = planeRect();
	QMatrix4x4 rotation_matrix;
	// Rotation in viewrect coordinates is clockwise, but it doesn't matter in
	// the case of rectangle with the center in (0,0)
	rotation_matrix.rotate(-angle, 0, 0, 1);
	// Arguments are top left and bottom right corners in viewrect coordinates:
	return rotation_matrix.mapRect(p);
}

bool OpenGLPlane::initializeBufferHelper(QOpenGLBuffer& buffer, const void* data, int count, GLuint index) {
	Q_ASSERT(&buffer == &vertex_buffer_ || &buffer == &UV_buffer_ || &buffer == &index_buffer_);

	if (!buffer.create()) {
		return false;
	}

	buffer.bind();
	buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	buffer.allocate(data, count);
	buffer.release();

	return true;
}

bool OpenGLPlane::initializeVertexBuffer() {
	return initializeBufferHelper(vertex_buffer_, mesh_.data(), sizeof(float) * mesh_.size(), OpenGLShaderProgram::vertex_coord_index);
}

bool OpenGLPlane::initializeUVBuffer() {
	return initializeBufferHelper(UV_buffer_, OpenGLPlane::uv_.data(), sizeof(float) * OpenGLPlane::uv_.size(), OpenGLShaderProgram::vertex_UV_index);
}

bool OpenGLPlane::initializeIndexBuffer() {
	return initializeBufferHelper(index_buffer_, indices_.data(), sizeof(unsigned int) * indices_.size(), OpenGLShaderProgram::indices_index);
}

bool OpenGLPlane::initialize() {
	if (!initializeVertexBuffer()) {
		return false;
	}
	if (!initializeUVBuffer()) {
		return false;
	}

	if (!initializeIndexBuffer()) {
		return false;
	}

	return true;
}

void OpenGLPlane::updateUV(int nx, int ny) {
	uv_.clear();
	float dx = 1.f / (nx - 1), dy = 1.f / (nx - 1);

	for (float x = 0.f; x <= 1.f; x += dx) {
		for (float y = 0.f; y <= 1.f; y += dy) {
			uv_.push_back(x);
			uv_.push_back(y);
		}
	}
}

void OpenGLPlane::updateIndices(int width, int height) {
	indices_.clear();

	for (auto row = 0U; row < height - 1; ++row) {
		// Repeat first element of the next row to create degenerate triangle
		// Do not do this for the first row

		if (row != 0U) {
			indices_.push_back(row * width);
		}

		for (auto column = 0U; column < width; ++column) {
			indices_.push_back(column + row * width);
			indices_.push_back(column + (row + 1) * width);
		}

		// Repeat last element of the current row to create degenerate triangle
		// Do not do this for the last row

		if (row != height - 2) {
			indices_.push_back(*(indices_.end() - 1));
		}
	}

}

constexpr const int OpenGLPlane::VERTICES_PER_SIDE;

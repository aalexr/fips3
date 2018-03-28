/*
 *  Copyright (C) 2018  Matwey V. Kornilov <matwey.kornilov@gmail.com>
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

#include <abstractopenglplan.h>

AbstractOpenGLPlan::AbstractOpenGLPlan(const QString& name, QObject* parent):
	QObject(parent),
	name_(name) {
}

AbstractOpenGLPlan::~AbstractOpenGLPlan() = default;

QString AbstractOpenGLPlan::vertexShaderSourceCode() {
	static const QString source = R"(
		attribute vec2 vertexCoord;
		attribute vec2 VertexUV;
		varying vec2 UV;
		uniform mat4 MVP;

		void main() {
			gl_Position = MVP * vec4(vertexCoord,0,1);
			UV = VertexUV;
		}
	)";

	return source;
}

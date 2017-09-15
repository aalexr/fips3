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

#ifndef _SCROLLZOOMAREA_H
#define _SCROLLZOOMAREA_H

#include <QAbstractScrollArea>

#include <fits.h>
#include <openglwidget.h>

class ScrollZoomArea: public QAbstractScrollArea {
	Q_OBJECT
public:
	ScrollZoomArea(QWidget *parent, const FITS::HeaderDataUnit& hdu);

	void zoomViewport(double zoom_factor);
	void zoomViewport(double zoom_factor, const QPoint& fixed_point);
	void fitToViewport();
	inline OpenGLWidget* viewport() const { return static_cast<OpenGLWidget*>(QAbstractScrollArea::viewport()); }
private slots:
	inline void translateScrollRectX(int x) { translateScrollRect(x, viewport()->viewrect().scroll().top());  }
	inline void translateScrollRectY(int y) { translateScrollRect(viewport()->viewrect().scroll().left(), y); }
	void updateBars();
protected:
	virtual void wheelEvent(QWheelEvent* event) override;
	virtual bool viewportEvent(QEvent* event) override;
private:
	void translateScrollRect(int x, int y);
};

#endif //_SCROLLZOOMAREA_H

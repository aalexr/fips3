#ifndef _OPENGLWIDGET_H_
#define _OPENGLWIDGET_H_

#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

#include <fits.h>

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions {
public:
	template<class T> class OpenGLDeleter {
	private:
		QOpenGLWidget *openGL_widget_;
	public:
		OpenGLDeleter(QOpenGLWidget *openGL_widget):
				openGL_widget_(openGL_widget){}
		void operator() (T* ptr) {
			openGL_widget_->makeCurrent();
			delete ptr;
			openGL_widget_->doneCurrent();
		};
	};

	template<class T> using openGL_unique_ptr = std::unique_ptr<T, OpenGLDeleter<T>>;

public:
	OpenGLWidget(QWidget *parent, FITS* fits);
	~OpenGLWidget();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	std::unique_ptr<FITS> fits_;
	OpenGLDeleter<QOpenGLTexture> texture_deleter_;
	openGL_unique_ptr<QOpenGLTexture> texture_;
	OpenGLDeleter<QOpenGLShaderProgram> program_deleter_;
	openGL_unique_ptr<QOpenGLShaderProgram> program_;
	QOpenGLBuffer vbo_;
	static const int program_vertex_coord_attribute = 0;
	static const int program_vertex_uv_attribute    = 1;
	static const int program_texture_uniform = 0;

	static constexpr GLfloat vbo_data[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	};
};


#endif // _OPENGLWIDGET_H_

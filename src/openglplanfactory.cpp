#include <openglplanfactory.h>
#include <openglplan.h>
#include <openglplanresolver.h>

OpenGLPlanFactory::OpenGLPlanFactory(const QOpenGLContext& opengl_context):
	features_{opengl_context} {
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<quint8>>& hdu) const {
	return resolveOpenGLPlan<Uint8OpenGL33Plan, Uint8OpenGL30Plan, Uint8OpenGLPlan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint16>>& hdu) const {
	return resolveOpenGLPlan<Int16OpenGL33Plan, Int16OpenGL30Plan, Int16OpenGLPlan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint32>>& hdu) const {
	return resolveOpenGLPlan<Int32OpenGL33Plan, Int32OpenGL30Plan, Int32OpenGLPlan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<qint64>>& hdu) const {
	return resolveOpenGLPlan<Int64OpenGL33Plan, Int64OpenGLPlan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<float>>& hdu) const {
	return resolveOpenGLPlan<FloatOpenGL33Plan, FloatOpenGL30Plan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::DataUnit<double>>& hdu) const {
	return resolveOpenGLPlan<DoubleOpenGL33Plan, DoubleOpenGL30Plan>(features_, hdu);
}

AbstractOpenGLPlan* OpenGLPlanFactory::operator() (const FITS::HeaderDataUnit<FITS::EmptyDataUnit>&) const {
	return Q_NULLPTR;
}

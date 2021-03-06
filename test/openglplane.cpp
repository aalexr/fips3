#include <QtTest/QtTest>

#include <openglplane.h>

class TestOpenGLPlane: public QObject
{
Q_OBJECT
private slots:
	void test_plane1x1();
	void test_plane1x2();
	void test_plane2x1();
	void test_plane16x16();
	void test_plane16x1024();
};

void TestOpenGLPlane::test_plane1x1() {
	OpenGLPlane pl{QSize{1,1}};
	auto p = pl.planeRect();
	QCOMPARE(p.left(), -1.);
	QCOMPARE(p.top(), -1.);
	QCOMPARE(p.right(), 1.);
	QCOMPARE(p.bottom(), 1.);
	QCOMPARE(pl.scale(), 1.);
}

void TestOpenGLPlane::test_plane1x2() {
	OpenGLPlane pl{QSize{1,2}};
	auto p = pl.planeRect();
	QCOMPARE(p.left(), -0.5);
	QCOMPARE(p.top(), -1.);
	QCOMPARE(p.right(), 0.5);
	QCOMPARE(p.bottom(), 1.);
	QCOMPARE(pl.scale(), 0.5);
}

void TestOpenGLPlane::test_plane2x1() {
	OpenGLPlane pl{QSize{2,1}};
	auto p = pl.planeRect();
	QCOMPARE(p.left(), -1.);
	QCOMPARE(p.top(), -0.5);
	QCOMPARE(p.right(), 1.);
	QCOMPARE(p.bottom(), 0.5);
	QCOMPARE(pl.scale(), 0.5);
}

void TestOpenGLPlane::test_plane16x16() {
	OpenGLPlane pl{QSize{16,16}};
	auto p = pl.planeRect();
	QCOMPARE(p.left(), -1.);
	QCOMPARE(p.top(), -1.);
	QCOMPARE(p.right(), 1.);
	QCOMPARE(p.bottom(), 1.);
	QCOMPARE(pl.scale(), 1./16);
}

void TestOpenGLPlane::test_plane16x1024() {
	OpenGLPlane pl{QSize{16,1024}};
	auto p = pl.planeRect();
	QCOMPARE(p.left(), -1./64);
	QCOMPARE(p.top(), -1.);
	QCOMPARE(p.right(), 1./64);
	QCOMPARE(p.bottom(), 1.);
	QCOMPARE(pl.scale(), 1./1024);
}

QTEST_GUILESS_MAIN(TestOpenGLPlane)
#include "openglplane.moc"

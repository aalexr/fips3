#include <QtTest/QtTest>
#include <QFile>
#include <fits.h>

#define DATA_ROOT PROJECT_ROOT "/test/data"

class TestFits: public QObject
{
	Q_OBJECT
private slots:
	void parseHeaderUnit1();
	void parseHeaderUnit2();
	void parseHeaderUnit3();
	void parseHeaderUnit4();
	void parseDataUnitShape();
};

void TestFits::parseHeaderUnit1() {
	QFile* file = new QFile(DATA_ROOT "/header_unexpected_end.fits");
	file->open(QIODevice::ReadOnly);
	QVERIFY_EXCEPTION_THROWN(FITS fits(file), FITS::UnexpectedEnd);
}
void TestFits::parseHeaderUnit2() {
	QFile* file = new QFile(DATA_ROOT "/header_end.fits");
	file->open(QIODevice::ReadOnly);
	FITS fits(file);
	QTRY_COMPARE(fits.header_unit().header("SIMPLE"), QString("T"));
	QTRY_COMPARE(fits.header_unit().header("BITPIX"), QString("16"));
	QTRY_COMPARE(fits.header_unit().header("NAXIS"), QString("2"));
	QTRY_COMPARE(fits.header_unit().header("NAXIS1"), QString("10"));
	QTRY_COMPARE(fits.header_unit().header("NAXIS2"), QString("10"));
}
void TestFits::parseHeaderUnit3() {
	quint8 end[] = "END     ";

	AbstractFITSStorage::Page page_begin(end);
	AbstractFITSStorage::Page page_end(end + 2880);

	FITS::HeaderUnit hdr(page_begin, page_end);
	QTRY_COMPARE(page_begin, page_end);
}
void TestFits::parseHeaderUnit4() {
	quint8 end[] = "END     ";

	AbstractFITSStorage::Page page_begin(end);
	AbstractFITSStorage::Page header_end(end + 2880);
	AbstractFITSStorage::Page page_end(end + 2 * 2880);

	FITS::HeaderUnit hdr(page_begin, page_end);
	QTRY_COMPARE(page_begin, header_end);
}
void TestFits::parseDataUnitShape() {
	QFile* file = new QFile(DATA_ROOT "/sombrero.fits");
	file->open(QIODevice::ReadOnly);
	const FITS fits(file);
	QTRY_COMPARE(fits.data_unit().height(), static_cast<quint64>(448));
	QTRY_COMPARE(fits.data_unit().width (), static_cast<quint64>(800));
}

QTEST_MAIN(TestFits)
#include "fits.moc"
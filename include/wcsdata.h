#ifndef _WCSDATA_H_
#define _WCSDATA_H_

#include <QMatrix4x4>
#include <QString>

#include <array>
#include <cmath>

#include <fits.h>

class WcsData
{
public:
	enum class WcsState {
		Error,
		Disabled,
		Enabled,
	};

	WcsData(WcsState state = WcsState::Error);
	explicit WcsData(const FITS::HeaderUnit&);
	~WcsData() = default;
	WcsData& operator =(const WcsData& other) {
		matrix_ = other.matrix_;
		state_ = other.state_;
		return *this;
	}

	const QMatrix4x4& matrix() const noexcept;
	float rotationAngle() const noexcept;

	inline bool isUsingWcs() const { return state_ == WcsState::Enabled; }
	void setUseWcs(bool use);
private:
	QMatrix4x4 matrix_;
	static const QMatrix4x4 identity_;
	WcsState state_;

	const QString CRPIX1 = "CRPIX1";
	const QString CRPIX2 = "CRPIX2";
	
	const QString CD1_1  = "CD1_1";
	const QString CD1_2  = "CD1_2";
	const QString CD2_1  = "CD2_1";
	const QString CD2_2  = "CD2_2";
};
#endif

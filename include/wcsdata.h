#ifndef _WCSDATA_H_
#define _USE_MATH_DEFINES
#define _WCSDATA_H_

#include <QMatrix4x4>
#include <QString>
#include <QPair>

#include <array>
#include <cmath>

#include <fits.h>
#include <utils/optional.h>

class WcsData {
public:
    enum class WcsState {
        Error,
        Disabled,
        Enabled,
    };

    explicit WcsData(WcsState state = WcsState::Error);
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
	const QString& name() const;

	static Utils::Optional<QPair<QString, QString>> parse_type(const FITS::HeaderUnit& unit) noexcept;
private:
	QMatrix4x4 matrix_;
	static const QMatrix4x4 identity_;
	WcsState state_;
	QString name_;

    const QString CRPIX1 = "CRPIX1";
    const QString CRPIX2 = "CRPIX2";

    const QString CD1_1  = "CD1_1";
    const QString CD1_2  = "CD1_2";
    const QString CD2_1  = "CD2_1";
    const QString CD2_2  = "CD2_2";
};
#endif

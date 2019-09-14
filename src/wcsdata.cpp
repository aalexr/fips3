#include "wcsdata.h"

WcsData::WcsData(WcsState state) : matrix_{}, state_{ state } {}

WcsData::WcsData(const FITS::HeaderUnit& unit) : WcsData{ WcsState::Enabled } {
	try {
        float m11, m12, m21, m22;
        float ref_px_x = unit.header_as<float>(CRPIX1);
        float ref_px_y = unit.header_as<float>(CRPIX2);

        if (unit.count(CD1_1) > 0 && unit.count(CD1_2) > 0 &&
            unit.count(CD2_1) > 0 && unit.count(CD2_2) > 0) {
            m11 = unit.header_as<float>(CD1_1);
            m12 = unit.header_as<float>(CD1_2);
            m21 = unit.header_as<float>(CD2_1);
            m22 = unit.header_as<float>(CD2_2);
        } else if (unit.count("PC1_1") > 0 && unit.count("PC1_2") > 0 &&
                 unit.count("PC2_1") > 0 && unit.count("PC2_2") > 0) {

            auto c_delt1 = unit.header_as<float>("CDELT1");
            auto c_delt2 = unit.header_as<float>("CDELT2");

            m11 = unit.header_as<float>("PC1_1") * c_delt1;
            m12 = unit.header_as<float>("PC1_2") * c_delt1;
            m21 = unit.header_as<float>("PC2_1") * c_delt2;
            m22 = unit.header_as<float>("PC2_2") * c_delt2;
        } else {
			state_ = WcsState::Error;
            qDebug() << "WCS data is not present or unsupported. Using identity transformation.";
        }

        if (isUsingWcs()) {
            matrix_ = QMatrix4x4{
                m11, m12, 0.f, unit.header_as<float>("CRVAL1") - (m11 * ref_px_x + m12 * ref_px_y),
                m21, m22, 0.f, unit.header_as<float>("CRVAL2") - (m21 * ref_px_x + m22 * ref_px_y),
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 0.f, 1.f};
        }
    }
    catch (std::exception& e) {
		state_ = WcsState::Error;
        qDebug() << "Error occurred during processing WCS data. See details: " << e.what();
    }
}

const QMatrix4x4& WcsData::matrix() const noexcept {
    if (isUsingWcs())
        return matrix_;
    return identity_;
}

float WcsData::rotationAngle() const noexcept {
    if (matrix().isIdentity())
        return 0.f;
    const float* f = matrix_.constData();
    return static_cast<int>(atan(f[5] / f[0]) * 180 / 3.14159265f);
}

void WcsData::setUseWcs(bool use) {
	if (state_ == WcsState::Error)
		return;
	state_ = use
		? WcsState::Enabled
		: WcsState::Disabled;
}

const QMatrix4x4 WcsData::identity_ = QMatrix4x4{};


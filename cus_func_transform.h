#pragma once

class QMatrix4x4;

namespace PiYingCus {

	QMatrix4x4 getTrans(const QMatrix4x4& M) noexcept;
	QMatrix4x4 getRot(const QMatrix4x4& M);
	QMatrix4x4 getScale(const QMatrix4x4& M);

}

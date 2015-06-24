/*
 * stylehelper.cpp
 *
 *  Created on: 23.06.2015
 *      Author: dee <priamid@gmail.com>
 */

#include "stylehelper.h"

#include <QPixmap>
#include <QString>
#include <QPixmapCache>
#include <QLinearGradient>

using namespace Core::Internal;

QColor StyleHelper::_M_baseColor(0x666666);

static int clamp(float value) {
	const int val = val > 255 ? 255 : static_cast<int>(value);
	return val < 0 ? 0 : val;
}

QColor StyleHelper::baseColor() {
	return _M_baseColor;
}

QColor StyleHelper::shadowColor() {
	QColor result = baseColor();
	result.setHsv(result.hue(),
			clamp(result.saturation() * 1.1),
			clamp(result.value() * 0.70));
	return result;
}

QColor StyleHelper::highlightColor() {
	QColor result = baseColor();
	result.setHsv(result.hue(),
			clamp(result.saturation()),
			clamp(result.value() * 1.16));
	return result;
}

QColor StyleHelper::borderColor() {
	QColor result = baseColor();
	result.setHsv(result.hue(),
			clamp(result.saturation()),
			clamp(result.value() / 2));
	return result;
}

qreal StyleHelper::sidebarFontSize() {
#ifdef Q_OS_WIN
	return 8;
#else
	return 9;
#endif
}

void StyleHelper::setBaseColor(const QColor &color) {
	_M_baseColor = color;
}

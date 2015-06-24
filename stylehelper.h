/*
 * stylehelper.h
 *
 *  Created on: 23.06.2015
 *      Author: dee <priamid@gmail.com>
 */

#ifndef STYLEHELPER_H_
#define STYLEHELPER_H_

#include <QColor>
#include <QPainter>
#include <QRect>

namespace Core {
namespace Internal {
	class StyleHelper {
	public:
		static int navigationWidgetHeight() { return 24; }
		static void setBaseColor(const QColor &color);
		static bool usePixmapCache() { return true; }
		static QColor baseColor();
		static QColor highlightColor();
		static QColor shadowColor();
		static QColor borderColor();
		static qreal sidebarFontSize();
	private:
		static QColor _M_baseColor;
	};
}
}

#endif /* STYLEHELPER_H_ */

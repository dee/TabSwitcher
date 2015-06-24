/***************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
**
** Non-Open Source Usage
**
** Licensees may use this file in accordance with the Qt Beta Version
** License Agreement, Agreement version 2.2 provided with the Software or,
** alternatively, in accordance with the terms contained in a written
** agreement between you and Nokia.
**
** GNU General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the packaging
** of this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
**
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt GPL Exception
** version 1.3, included in the file GPL_EXCEPTION.txt in this package.
**
***************************************************************************/

#ifndef FANCYTABWIDGET_H
#define FANCYTABWIDGET_H

#include <QPushButton>
#include <QTabBar>
#include <QStyleOptionTabV2>
#include <QTimeLine>

QT_BEGIN_NAMESPACE
class QPainter;
class QStackedLayout;
class QStatusBar;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

class FancyTabBar : public QTabBar
{
	Q_OBJECT

public:
	FancyTabBar(QWidget *parent = 0);
	~FancyTabBar();

	QSize tabSizeHint(int index) const;
	void paintEvent(QPaintEvent *event);
	void paintTab(QPainter *painter, int tabIndex) const;
	void mouseMoveEvent(QMouseEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void tabInserted(int index);
	void tabRemoved(int index);

public slots:
	void updateHover();

private:
	static const int m_rounding;
	static const int m_textPadding;
	QTimeLine m_hoverControl;
	QRect m_hoverRect;
};

} // namespace Internal
} // namespace Core

#endif // FANCYTABWIDGET_H








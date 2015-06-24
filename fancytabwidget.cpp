
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

#include "fancytabwidget.h"
#include "stylehelper.h"

#include <QDebug>

#include <QColorDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
//#include <QWindowsStyle>
#include <QPainter>
#include <QSplitter>
#include <QStackedLayout>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QGradient>

using namespace Core;
using namespace Internal;

const int FancyTabBar::m_rounding = 22;
const int FancyTabBar::m_textPadding = 4;

FancyTabBar::FancyTabBar(QWidget *parent)
	: QTabBar(parent)
{
//	TODO: enable style
//	setStyle(new QWindowsStyle);
	setDrawBase(false);
	setElideMode(Qt::ElideNone);
	setMinimumWidth(qMax(2 * m_rounding, 40));
	setMinimumHeight(StyleHelper::navigationWidgetHeight());
	setAttribute(Qt::WA_Hover, true);
	setFocusPolicy(Qt::NoFocus);
	m_hoverControl.setFrameRange(0, 20);
	m_hoverControl.setDuration(130);
	m_hoverControl.setCurveShape(QTimeLine::EaseInCurve);
	connect(&m_hoverControl, SIGNAL(frameChanged(int)), this, SLOT(updateHover()));
	setMouseTracking(true); // Needed for hover events
	setExpanding(false);
}

FancyTabBar::~FancyTabBar()
{
	delete style();
}

QSize FancyTabBar::tabSizeHint(int index) const
{
	QFont boldFont(font());
	boldFont.setPointSize(10);
	boldFont.setBold(true);
	QFontMetrics fm(boldFont);
	int spacing = 4;
	int width = spacing + 2;
	QString currentTabText(tabText(index));
	if (currentTabText.length() != 0) {
		width += fm.averageCharWidth()*tabText(index).length() + 10;
	} else
		width += 60;
	int height = tabIcon(index).actualSize(QSize(64, 64)).height() + spacing + fm.height();
	if (height < StyleHelper::navigationWidgetHeight())
		height = StyleHelper::navigationWidgetHeight();

	return QSize(width, height);
}

void FancyTabBar::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	QPainter p(this);

	// paint background
	QRect rect = this->rect();
		p.fillRect(rect, StyleHelper::baseColor());

	for (int i = 0; i < count(); ++i)
		if (i != currentIndex())
			paintTab(&p, i);

	// paint active tab last, since it overlaps the neighbors
	paintTab(&p, currentIndex());
}

// Handle hover events for mouse fade ins
void FancyTabBar::mouseMoveEvent(QMouseEvent *e)
{
	if (!m_hoverRect.contains(e->pos())) {
		for (int i = 0; i < count(); ++i) {
			QRect area = tabRect(i);
			if (area.contains(e->pos())) {
				QRect oldHoverRect = m_hoverRect;
				m_hoverRect = area;
				update(oldHoverRect);
				m_hoverControl.stop();
				m_hoverControl.start();
				break;
			}
		}
	}
}

void FancyTabBar::updateHover()
{
	update(m_hoverRect);
}

// Resets hover animation on mouse enter
void FancyTabBar::enterEvent(QEvent *e)
{
	Q_UNUSED(e);
	m_hoverRect = QRect();
}

// Resets hover animation on mouse enter
void FancyTabBar::leaveEvent(QEvent *e)
{
	Q_UNUSED(e);

	m_hoverControl.stop();
	m_hoverControl.start();
}

void FancyTabBar::paintTab(QPainter *painter, int tabIndex) const
{
	QStyleOptionTabV2 tab;
	initStyleOption(&tab, tabIndex);
	QRect rect = tab.rect;
	painter->save();

	bool selected = tab.state & QStyle::State_Selected;
	bool hover = tab.state & QStyle::State_MouseOver;

#ifdef Q_WS_MAC
	hover = false; // Dont hover on Mac
#endif

	QColor hoverColor;

	if (hover) {
		hoverColor = QColor(255, 255, 255, m_hoverControl.currentFrame()*2);
	}

	QColor light = QColor(255, 255, 255, 40);
	QColor dark = QColor(0, 0, 0, 60);

	if (selected) {
		painter->fillRect(tab.rect, QColor(0x47, 0x47, 0x47));
		painter->setPen(QPen(light, 0));
		painter->drawLine(rect.topLeft(), rect.topRight());
		painter->setPen(QPen(dark, 0));
		painter->drawLine(rect.bottomLeft(), rect.bottomRight());
	} else {
		painter->fillRect(tab.rect, StyleHelper::baseColor());
		if (hover)
			painter->fillRect(tab.rect, hoverColor);
	}

	QString tabText(tab.text);
	QRect tabTextRect(tab.rect);
	QRect tabIconRect(tab.rect);
	QFont font(painter->font());
	font.setPointSizeF(StyleHelper::sidebarFontSize());
	font.setBold(false);
	painter->setFont(font);
	painter->setPen(Qt::white);
	int textFlags = Qt::AlignCenter | Qt::AlignCenter | Qt::ElideRight | Qt::TextWordWrap;
	painter->drawText(tabTextRect, textFlags, tabText);
	painter->setPen(selected ? QColor(60, 60, 60) : Qt::white);
	int textHeight = painter->fontMetrics().boundingRect(QRect(0, 0, width(), height()), Qt::TextWordWrap, tabText).height();
	tabIconRect.adjust(0, 4, 0, -textHeight);
	style()->drawItemPixmap(painter, tabIconRect, Qt::AlignCenter | Qt::AlignVCenter,
							tab.icon.pixmap(QSize(64, 64)));
//    painter->translate(0, -1);
//    painter->drawText(tabTextRect, textFlags, tabText);
	painter->restore();
}

void FancyTabBar::tabInserted(int index)
{
	Q_UNUSED(index)
}

void FancyTabBar::tabRemoved(int index)
{
	Q_UNUSED(index)
}

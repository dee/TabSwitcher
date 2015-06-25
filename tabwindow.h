/*
 * tabwindow.h
 *
 *  Created on: 23.06.2015
 *      Author: dee <priamid@gmail.com>
 */

#ifndef TABWINDOW_H_
#define TABWINDOW_H_

#include <QWidget>
#include <QList>
#include <QHash>
#include <coreplugin/icore.h>

class QMenu;

namespace Core {
	class IMode;
	class IEditor;
}

namespace TabSwitch
{
	class FancyTabBar;

	class TabWindow : public QWidget
	{
		Q_OBJECT
	public:
		TabWindow(Core::ICore *core, QWidget *parent = 0);
		~TabWindow();
		FancyTabBar *tabBar() { return m_tab; }
	public slots:
		void modeChanged(Core::IMode *);
		void editorCreated(Core::IEditor *editor, const QString &fileName);
		void currentEditorChanged(Core::IEditor *editor);
		void editorsClosed(QList<Core::IEditor *> editors);
		void currentChanged(int);
		void customContextMenu(const QPoint &);
		void closeOthers();
	private:
		Core::ICore *m_core;
		FancyTabBar *m_tab;
		QMenu *m_popup;
		QAction *m_closeOthers;
		bool m_initialised;

		QString getTabCaption(const QString &fileName);
		int findTab(Core::IEditor *editor);
	};
}

#endif /* TABWINDOW_H_ */

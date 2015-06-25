/*
 * tabwindow.cpp
 *
 * Created on: 23.06.2015
 * Author: dee <priamid@gmail.com>
 */

#include "tabwindow.h"
#include <coreplugin/imode.h>
#include <coreplugin/idocument.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/editmode.h>
#include <coreplugin/minisplitter.h>

#include <QTabBar>
#include <QLayout>
#include <QFileInfo>
#include <QDebug>
#include <QMenu>

#include "fancytabwidget.h"

using namespace TabSwitch;
using Core::EditorManagerPlaceHolder;
using Core::ActionManager;

TabWindow::TabWindow(Core::ICore *core, QWidget *parent) : QWidget(parent),
	m_core(core), m_initialised(false)
{
/*
	QVBoxLayout *vBox = new QVBoxLayout(this);

	vBox->setContentsMargins(0, 0, 0, 0);
	m_tab = new FancyTabBar(this);
	m_tab->setContextMenuPolicy(Qt::CustomContextMenu);
	vBox->addWidget(m_tab);

	connect(m_tab, SIGNAL(currentChanged(int)),
			this, SLOT(currentChanged(int)));
	connect(m_tab, SIGNAL(customContextMenuRequested(const QPoint &)),
			this, SLOT(customContextMenu(const QPoint &)));

	auto *am = ActionManager::instance();
	m_popup = new QMenu(this);

	// get first from manager, it is predefined
	Core::Command *cmd = am->command(Core::Constants::CLOSE);

	if (cmd != 0)
		m_popup->addAction(cmd->action());

	// register own action
	m_closeOthers = new QAction(tr("Close &Others"), this);
	Core::Context pContext(Core::Constants::C_GLOBAL);
	am->registerAction(m_closeOthers, Core::Id("TabBar.CloseOtherEditors"),
					   pContext);
	connect(m_closeOthers, SIGNAL(triggered()),
			this, SLOT(closeOthers()));
	m_popup->addAction(m_closeOthers);

	// get third from manager also
	cmd = am->command(Core::Constants::CLOSEALL);

	if (cmd != 0)
		m_popup->addAction(cmd->action());
*/
}

//TabWindow::initialise(QWidget* parent)
//{
//	m_tab = new FancyTabBar(parent);
//}

TabWindow::~TabWindow()
{
//	ActionManager::instance()->unregisterAction(m_closeOthers, Core::Id("TabBar.CloseOtherEditors"));
//	qDebug() << "Destroying TabWindow instance.";
}

void TabWindow::modeChanged(Core::IMode *mode)
{

}

QString TabWindow::getTabCaption(const QString& fileName)
{
	QFileInfo fi(fileName);

	return QString(fi.fileName());
}

void TabWindow::editorCreated(Core::IEditor *editor, const QString& fileName)
{
	Q_UNUSED(editor);
	qDebug() << "editorCreated()";
	int index = m_tab->addTab(getTabCaption(fileName));

	// QVariant::fromValue did not work =(
	qlonglong ptr = reinterpret_cast<qulonglong>(editor);
	m_tab->setTabData(index, ptr);
}

void TabWindow::currentEditorChanged(Core::IEditor *editor)
{
	qDebug() << "currentEditorChanged()";
	if (editor != 0)
	{
		int index = findTab(editor);

		if (index >= 0)
			m_tab->setCurrentIndex(index);
		else
		{
			qWarning() << "No tab found for editor : " <<
				editor->document()->displayName();
		}
	}
}

void TabWindow::editorsClosed(QList<Core::IEditor *>editors)
{
	Core::IEditor *editor = 0;
	int index;

	qDebug() << "editorsClosed()";
	foreach(editor, editors)
	{
		index = findTab(editor);

		if (index >= 0)
		{
			m_tab->removeTab(index);
		}
		else
		{
			qWarning() << "No tab found for editor : " <<
				editor->document()->displayName();
		}
	}
}

int TabWindow::findTab(Core::IEditor *editor)
{
	for (int i = 0; i < m_tab->count(); i++)
	{
		if (m_tab->tabData(i) == reinterpret_cast<qulonglong>(editor))
			return i;
	}
	return -1;
}

void TabWindow::currentChanged(int index)
{
	qDebug() << "currentChanged()";
	if (index >= 0)
	{
		Core::IEditor *editor =
			reinterpret_cast<Core::IEditor *>(m_tab->tabData(index).toLongLong());

		if (editor != 0)
			Core::EditorManager::instance()->activateEditor(editor);
	}
}

void TabWindow::customContextMenu(const QPoint& point)
{
	int index = m_tab->tabAt(point);

	if (index >= 0)
	{
		m_tab->setCurrentIndex(index);
	}
	m_popup->popup(m_tab->mapToGlobal(point));
}

void TabWindow::closeOthers()
{
	int index = m_tab->currentIndex();

	QList<Core::IEditor *> editors;
	if (index >= 0)
	{
		for (int i = 0; i < m_tab->count(); i++)
		{
			if (i != index)
				editors.append(reinterpret_cast<Core::IEditor *>(m_tab->tabData(i).toLongLong()));
		}
	}
	Core::EditorManager::instance()->closeEditors(editors, true);
}


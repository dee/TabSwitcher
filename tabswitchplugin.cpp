#include "tabswitchplugin.h"
#include "tabswitchconstants.h"
#include "fancytabwidget.h"

#include <coreplugin/icore.h>
#include <coreplugin/imode.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <extensionsystem/pluginmanager.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/editmode.h>
#include <coreplugin/minisplitter.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <QtPlugin>

using namespace TabSwitch::Internal;
using namespace TabSwitch;
using Core::EditorManagerPlaceHolder;
using Core::ActionManager;

TabSwitchPlugin::TabSwitchPlugin()
	: m_initialised(false)
{
	// Create your members
	qDebug() << "Creating TabSwitch plugin.";
}

TabSwitchPlugin::~TabSwitchPlugin()
{
	qDebug() << "Destroying TabSwitch plugin.";
	// Unregister objects from the plugin manager's object pool
	// Delete members
}

bool TabSwitchPlugin::initialize(const QStringList &arguments, QString *errorString)
{
	// Register objects in the plugin manager's object pool
	// Load settings
	// Add actions to menus
	// Connect to other plugins' signals
	// In the initialize function, a plugin can be sure that the plugins it
	// depends on have initialized their members.

	Q_UNUSED(arguments)
	Q_UNUSED(errorString)

//	auto pm = ExtensionSystem::PluginManager::instance();
//	m_core = pm->getObject<Core::ICore>();
//	m_wnd = new TabWindow(m_core);
	return true;
}

void TabSwitchPlugin::extensionsInitialized()
{
	// Retrieve objects from the plugin manager's object pool
	// In the extensionsInitialized function, a plugin can be sure that all
	// plugins that depend on it are completely initialized.
	connect(Core::ModeManager::instance(), SIGNAL(currentModeChanged(Core::IMode *)),
			this,
			SLOT(modeChanged(Core::IMode *)));
/*
	connect(Core::ModeManager::instance(), SIGNAL(currentModeChanged(Core::IMode *)),
			m_wnd,
			SLOT(modeChanged(Core::IMode *)));
	connect(Core::EditorManager::instance(),
			SIGNAL(editorCreated(Core::IEditor *, const QString &)),
			m_wnd,
			SLOT(editorCreated(Core::IEditor *, const QString &)));
	connect(Core::EditorManager::instance(),
			SIGNAL(currentEditorChanged(Core::IEditor *)),
			m_wnd,
			SLOT(currentEditorChanged(Core::IEditor *)));
	connect(Core::EditorManager::instance(),
			SIGNAL(editorsClosed(QList<Core::IEditor *>)),
			m_wnd,
			SLOT(editorsClosed(QList<Core::IEditor *>)));
*/
}

void TabSwitchPlugin::modeChanged(Core::IMode* mode)
{
	qDebug() << "Current mode is: " << mode->displayName();
	if (mode->id() == Core::Constants::MODE_EDIT)
	{
		qDebug() << "Edit mode active.";
		if (!m_initialised)
		{
			Core::Internal::EditMode *pEditMode =
				dynamic_cast<Core::Internal::EditMode *>(mode);
			Core::MiniSplitter *splitter =
				dynamic_cast<Core::MiniSplitter *>(pEditMode->widget());
			Q_ASSERT(splitter != NULL);

			// qDebug() << "Widget is: " << splitter->children().length();
			EditorManagerPlaceHolder *placeHolder =
				dynamic_cast<EditorManagerPlaceHolder *>(splitter->focusProxy());
			Q_ASSERT(placeHolder != NULL);

			// get layout and modify it
			m_placeHolderLayout =
				dynamic_cast<QVBoxLayout *>(placeHolder->layout());
			Q_ASSERT(m_placeHolderLayout != NULL);

			m_tab = new FancyTabBar(placeHolder);
			m_tab->setObjectName(QLatin1String("TabSwitcherWidget"));
			m_placeHolderLayout->setSpacing(0);
			m_placeHolderLayout->insertWidget(0, m_tab);

			m_initialised = true;
		}
		m_tab->show();
	}
	else
	{
		m_tab->setParent(0);
		m_tab->hide();
		qDebug() << "Edit mode inactive.\r\n";
	}
}

ExtensionSystem::IPlugin::ShutdownFlag TabSwitchPlugin::aboutToShutdown()
{
	// Save settings
	// Disconnect from signals that are not needed during shutdown
/*
	disconnect(m_wnd, SLOT(modeChanged(Core::IMode *)));
	disconnect(m_wnd, SLOT(editorCreated(Core::IEditor *, const QString &)));
	disconnect(m_wnd, SLOT(currentEditorChanged(Core::IEditor *)));
	disconnect(m_wnd, SLOT(editorsClosed(QList<Core::IEditor *>)));
*/
	//m_placeHolderLayout->removeWidget(0);
	// Hide UI (if you add UI that is not in the main window directly)
	m_tab->setParent(0);
	m_tab->hide();
	return SynchronousShutdown;
}


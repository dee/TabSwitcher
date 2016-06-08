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
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/idocument.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <QtPlugin>
#include <QFileInfo>

using namespace TabSwitch::Internal;
using namespace TabSwitch;
using namespace Core;

TabSwitchPlugin::TabSwitchPlugin()
    : m_initialised(false)
{
    // Create your members
    qDebug() << "Creating TabSwitch plugin.";

    // create tab
    m_tab = new FancyTabBar(0);
    m_tab->setObjectName(QLatin1String("TabSwitcherWidget"));
}

void TabSwitchPlugin::buildMenu()
{
    qDebug() << "Building menu.";
    // create popup
    m_popup = new QMenu(m_tab);

    ActionManager *am = Core::ActionManager::instance();
    // get first from manager, it is predefined
    Command *cmd = am->command(Core::Constants::CLOSE);

    Q_ASSERT(cmd != 0);
    m_popup->addAction(cmd->action());

    // register own action
    m_closeOthers = new QAction(tr("Close &Others"), this);
    Context pContext(Core::Constants::C_GLOBAL);
    am->registerAction(m_closeOthers, Id("TabBar.CloseOtherEditors"), pContext);

    connect(m_closeOthers, SIGNAL(triggered()),     this, SLOT(closeOthers()));
    m_popup->addAction(m_closeOthers);

    // get third from manager also
    cmd = am->command(Core::Constants::CLOSEALL);
    Q_ASSERT(cmd != 0);
    m_popup->addAction(cmd->action());
}

TabSwitchPlugin::~TabSwitchPlugin()
{
    qDebug() << "Destroying TabSwitch plugin.";
    // Unregister objects from the plugin manager's object pool
    auto am = ActionManager::instance();
    am->unregisterAction(m_closeOthers, Id("TabBar.CloseOtherEditors"));

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

    buildMenu();

    return true;
}

void TabSwitchPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
    qDebug() << "Connecting signals and slots.";

    connect(Core::ModeManager::instance(), SIGNAL(currentModeChanged(Core::IMode *)),
            this,
            SLOT(modeChanged(Core::IMode *)));
    connect(Core::ModeManager::instance(), SIGNAL(currentModeChanged(Core::IMode *)),
            this,
            SLOT(modeChanged(Core::IMode *)));
    connect(Core::EditorManager::instance(), SIGNAL(editorCreated(Core::IEditor *, const QString &)),
            this,
            SLOT(editorCreated(Core::IEditor *, const QString &)));
    connect(Core::EditorManager::instance(), SIGNAL(currentEditorChanged(Core::IEditor *)),
            this,
            SLOT(currentEditorChanged(Core::IEditor *)));
    connect(Core::EditorManager::instance(), SIGNAL(editorsClosed(QList<Core::IEditor *>)),
            this,
            SLOT(editorsClosed(QList<Core::IEditor *>)));

    // tab widget signals
    m_tab->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tab, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));
    connect(m_tab, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenu(const QPoint &)));
}

void TabSwitchPlugin::modeChanged(Core::IMode *mode)
{
    qDebug() << "Current mode is: " << mode->displayName();
    if (mode->id() == Core::Constants::MODE_EDIT)
    {
        Core::Internal::EditMode *pEditMode =
            dynamic_cast<Core::Internal::EditMode *>(mode);
        Core::MiniSplitter *splitter =
            dynamic_cast<Core::MiniSplitter *>(pEditMode->widget());
        Q_ASSERT(splitter != NULL);

        qDebug() << "Widget is: " << splitter->children().length();
        m_placeHolder =
            dynamic_cast<EditorManagerPlaceHolder *>(splitter->focusProxy());
        Q_ASSERT(m_placeHolder != NULL);

        // get layout and modify it
        m_placeHolderLayout =
            dynamic_cast<QVBoxLayout *>(m_placeHolder->layout());
        Q_ASSERT(m_placeHolderLayout != NULL);

        qDebug() << "Creating tab widget.";
        m_tab->setParent(m_placeHolder);
        m_placeHolderLayout->setSpacing(0);
        m_placeHolderLayout->insertWidget(0, m_tab);
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
    disconnect(this, SLOT(modeChanged(Core::IMode *)));
    disconnect(this, SLOT(editorCreated(Core::IEditor *, const QString &)));
    disconnect(this, SLOT(currentEditorChanged(Core::IEditor *)));
    disconnect(this, SLOT(editorsClosed(QList<Core::IEditor *>)));

    // Hide UI (if you add UI that is not in the main window directly)
    m_tab->setParent(0);
    m_tab->hide();
    return SynchronousShutdown;
}

void TabSwitchPlugin::editorCreated(Core::IEditor *editor, const QString &fileName)
{
    // add tab
    qDebug() << "editorCreated()";
    Q_ASSERT(m_tab != 0);
    int index = m_tab->addTab(getTabCaption(fileName));

    auto key = fileName;
    qDebug() << "Setting tab data to " << key;
    m_tab->setTabData(index, key);
}

QString TabSwitchPlugin::getTabCaption(const QString& fileName)
{
    QFileInfo fi(fileName);
    return QString(fi.fileName());
}

void TabSwitchPlugin::currentEditorChanged(Core::IEditor *editor)
{
    if (editor != 0)
    {
        int index = findTab(editor);

        if (index >= 0)
        {
            m_tab->setCurrentIndex(index);
        }
        else
        {
            qWarning() << "No tab found for editor : " <<
                editor->document()->displayName();
        }
    }
}

void TabSwitchPlugin::editorsClosed(QList<Core::IEditor *> editors)
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

void TabSwitchPlugin::currentChanged(int index)
{
    qDebug() << "currentChanged()";
    if (index >= 0)
    {
        QString fn = m_tab->tabData(index).toString();
        const QList<IEditor *> editors = DocumentModel::editorsForFilePath(fn);
        if (!editors.isEmpty()) {
            IEditor *editor = editors.first();
            Core::EditorManager::instance()->activateEditor(editor);
        }
    }
}

int TabSwitchPlugin::findTab(Core::IEditor *editor)
{
    qDebug() << "Find tab: " << editor->document()->filePath().toString();
    for (int i = 0; i < m_tab->count(); i++)
    {
        auto data = m_tab->tabData(i).toString();
        if (data == editor->document()->filePath().toString())
        {
            qDebug() << "Found, index is " << i;
            return i;
        }
    }
    return -1;
}

void TabSwitchPlugin::customContextMenu(const QPoint& point)
{
    qDebug() << "Custom context menu request.";
    int index = m_tab->tabAt(point);
    if (index >= 0)
    {
        m_tab->setCurrentIndex(index);
    }
    m_popup->popup(m_tab->mapToGlobal(point));
}

void TabSwitchPlugin::closeOthers()
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

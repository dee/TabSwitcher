#ifndef TABSWITCH_H
#define TABSWITCH_H

#include "tabswitch_global.h"
#include <extensionsystem/iplugin.h>

class QStringList;
class QString;
class QVBoxLayout;
class QMenu;
class QAction;

namespace Core
{
	class EditorManagerPlaceHolder;
	class IMode;
	class IEditor;
	class ICore;
}

namespace TabSwitch
{
	class FancyTabBar;

	namespace Internal
	{
		class TabSwitchPlugin : public ExtensionSystem::IPlugin
		{
			Q_OBJECT
			Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "TabSwitch.json")

		public:
			TabSwitchPlugin();
			~TabSwitchPlugin();

			bool initialize(const QStringList &arguments, QString *errorString);
			void extensionsInitialized();
			ShutdownFlag aboutToShutdown();
		public slots:
			void modeChanged(Core::IMode *mode);
			void editorCreated(Core::IEditor *editor, const QString &fileName);
			void currentEditorChanged(Core::IEditor *editor);
			void editorsClosed(QList<Core::IEditor *> editors);
			void currentChanged(int);
			void customContextMenu(const QPoint &);
			void closeOthers();
		private:
			FancyTabBar *m_tab;
			QVBoxLayout* m_placeHolderLayout;
			Core::EditorManagerPlaceHolder* m_placeHolder;
			Core::ICore *m_core;
			QMenu *m_popup;
			QAction *m_closeOthers;
			bool m_initialised;

			QString getTabCaption(const QString&);
			int findTab(Core::IEditor *editor);
			void buildMenu();
		};

	} // namespace Internal
} // namespace TabSwitch

#endif // TABSWITCH_H


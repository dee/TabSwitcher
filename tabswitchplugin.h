#ifndef TABSWITCH_H
#define TABSWITCH_H

#include "tabswitch_global.h"
#include <extensionsystem/iplugin.h>
#include "tabwindow.h"

class QStringList;
class QString;
class QVBoxLayout;

namespace Core
{
	class EditorManagerPlaceHolder;
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
		};

	} // namespace Internal
} // namespace TabSwitch

#endif // TABSWITCH_H


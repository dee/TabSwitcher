#ifndef TABSWITCH_H
#define TABSWITCH_H

#include "tabswitch_global.h"
#include <extensionsystem/iplugin.h>
#include "tabwindow.h"

class QStringList;
class QString;
class QVBoxLayout;

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
		private:
			FancyTabBar *m_tab;
			QVBoxLayout* m_placeHolderLayout;
			Core::ICore *m_core;
			bool m_initialised;
		};

	} // namespace Internal
} // namespace TabSwitch

#endif // TABSWITCH_H


#ifndef TABSWITCH_H
#define TABSWITCH_H

#include "tabswitch_global.h"
#include <extensionsystem/iplugin.h>
#include "tabwindow.h"

class QStringList;
class QString;

namespace TabSwitch {
	namespace Internal {

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
		private:
			TabWindow *m_wnd;
			Core::ICore *m_core;
		};

	} // namespace Internal
} // namespace TabSwitch

#endif // TABSWITCH_H


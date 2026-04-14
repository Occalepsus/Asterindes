#include "GuiClient.h"

// Asterindes
#include "AsterindesCore.h"

// Qt
#include <QQuickWindow>

// STL
#include <source_location>

using namespace Asterindes;
using namespace Asterindes::Ui;

GuiClient::GuiClient(AsterindesCore& p_parentCoreApp)
	: QObject(&p_parentCoreApp)
	, m_resourcesListModel(this, p_parentCoreApp.getProjectManager().getResourcesManager())
{
	QObject::connect(&m_appQmlEngine, &QQmlApplicationEngine::objectCreated, this, &GuiClient::onQmlFileLoaded);

	m_appQmlEngine.loadFromModule("Asterindes", "Main");
	// TODO: Properly handle errors
	if (m_appQmlEngine.rootObjects().isEmpty()) {
		qFatal("%s(): Cannot load QML component 'Asterindes/Main'.", std::source_location::current().function_name());
	}
}

void GuiClient::onQmlFileLoaded(QObject* p_qmlObject, const QUrl& p_url)
{
	if (p_qmlObject && p_url.fileName() == "Main.qml")
	{
		const QQuickWindow* l_window{ qobject_cast<QQuickWindow*>(p_qmlObject) };

		m_resourcesListModel.setupQmlModel(l_window);
	}
}
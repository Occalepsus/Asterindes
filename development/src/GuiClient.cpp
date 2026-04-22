#include "GuiClient.h"

// Asterindes
#include "AsterindesCore.h"

// Qt
#include <QQmlContext>

using namespace Asterindes;
using namespace Asterindes::Ui;

GuiClient::GuiClient(AsterindesCore& p_parentCoreApp)
	: QObject(&p_parentCoreApp)
	// Create ViewModel (which connects to ResourcesManager internally)
	, m_resourcesViewModel(p_parentCoreApp.getProjectManager().getResourcesManager(), this)
{
	// Setup QML context BEFORE loading QML
	setupQmlContext();

	QObject::connect(&m_appQmlEngine, &QQmlApplicationEngine::objectCreated, 
					 this, &GuiClient::onQmlFileLoaded);

	m_appQmlEngine.loadFromModule("Asterindes", "Main");
	
	if (m_appQmlEngine.rootObjects().isEmpty()) {
		qFatal("Cannot load QML component 'Asterindes/Main'.");
	}
}

void GuiClient::setupQmlContext()
{
	// Expose the resources ViewModel to QML
	m_appQmlEngine.rootContext()->setContextProperty("resourcesViewModel", &m_resourcesViewModel);
}

void GuiClient::onQmlFileLoaded(QObject* p_qmlObject, const QUrl& p_url)
{
	// All setup is done via Q_PROPERTY bindings
}

#include "ProjectWindow.h"

// Asterindes
#include "AsterindesCore.h"
#include "AsterindesProject.h"

// Qt
#include <QQmlContext>
#include <QTimer>

using namespace Asterindes;
using namespace Asterindes::Ui;

ProjectWindow::ProjectWindow(AsterindesProject* p_project, AsterindesCore* p_coreApp)
	: QObject(p_coreApp)
	// Create ViewModels which connects to their respective models internally
	, m_projectViewModel(new ProjectViewModel(p_project, this))
	, m_resourcesViewModel(new ResourcesViewModel(p_project->getResourceRegistry(), this))
{
	//QObject::connect(&p_project, &AsterindesProject::projectLoaded, this, &ProjectWindow::openProjectWindow);
}

ProjectWindow::~ProjectWindow()
{
	m_appQmlEngine.clearComponentCache();
	m_appQmlEngine.clearSingletons();
}

void ProjectWindow::openProjectWindow()
{
	// Setup QML context BEFORE loading QML
	setupQmlContext();

	//QObject::connect(&m_appQmlEngine, &QQmlApplicationEngine::objectCreated, this, &ProjectWindow::onQmlFileLoaded);

	m_appQmlEngine.loadFromModule("Asterindes", "Main");

	if (m_appQmlEngine.rootObjects().isEmpty()) {
		qFatal("Cannot load QML component 'Asterindes/Main'.");
	}
}

void ProjectWindow::setupQmlContext()
{
	// Expose the resources ViewModel to QML
	m_appQmlEngine.rootContext()->setContextProperty("projectViewModel", m_projectViewModel);
	m_appQmlEngine.rootContext()->setContextProperty("resourcesViewModel", m_resourcesViewModel);
}

void ProjectWindow::onQmlFileLoaded(QObject* p_qmlObject, const QUrl& p_url)
{
}

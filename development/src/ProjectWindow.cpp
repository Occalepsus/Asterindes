#include "ProjectWindow.h"

// Asterindes
#include "AsterindesCore.h"
#include "AsterindesProject.h"

// Qt
#include <QQmlContext>
#include <QFileSystemWatcher>

#include <QDirIterator>

using namespace Asterindes;
using namespace Asterindes::Ui;

ProjectWindow::ProjectWindow(AsterindesProject* p_project, AsterindesCore* p_coreApp)
	: QObject(p_coreApp)
	// Create ViewModels which connects to their respective models internally
	, m_projectViewModel(new ProjectViewModel(p_project, this))
	, m_resourcesViewModel(new ResourcesViewModel(p_project->getResourceRegistry(), this))
{
}

ProjectWindow::~ProjectWindow()
{
	m_appQmlEngine->clearComponentCache();
	m_appQmlEngine->clearSingletons();
	m_appQmlEngine->deleteLater();
}

void ProjectWindow::openProjectWindow()
{
	// Setup QML context BEFORE loading QML
	m_appQmlEngine->setInitialProperties({
		{ "projectWindow", QVariant::fromValue(this) },
		{ "qmlLoader", QVariant::fromValue(m_qmlDynamicLoader) }
	});

#ifdef QT_DEBUG
	m_qmlDynamicLoader->loadQmlFile(QUrl::fromLocalFile("../development/res/ProjectWindow.qml"));
#else
	m_appQmlEngine->loadFromModule("Asterindes", "ProjectWindow");
#endif // QT_DEBUG
}

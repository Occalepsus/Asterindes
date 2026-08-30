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

ProjectWindow::ProjectWindow(AsterindesProject* p_project, AsterindesCore* p_coreApp, ProjectManagerService* p_projectManagerService)
	: QObject(p_coreApp)
	// Create ViewModels which connects to their respective models internally
	, m_projectViewModel(new ProjectViewModel(p_project, this))
	, m_resourcesViewModel(new ResourcesViewModel(p_project->getResourceRegistry(), this))
	, m_projectManagerService(p_projectManagerService)
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
		{ "projectManagerService", QVariant::fromValue(m_projectManagerService.data()) },
		{ "qmlLoader", QVariant::fromValue(m_qmlDynamicLoader) }
	});

#ifdef QT_DEBUG
	m_qmlDynamicLoader->loadQmlFile(QUrl::fromLocalFile("../development/res/ProjectWindow.qml"));
#else
	m_appQmlEngine->loadFromModule("Asterindes", "ProjectWindow");
#endif // QT_DEBUG
}

void ProjectWindow::showStartupWindow() const
{
	AsterindesCore* l_coreApp = qobject_cast<AsterindesCore*>(parent());
	if (l_coreApp)
	{
		l_coreApp->showStartupWindow();
	}
}
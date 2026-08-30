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
	, m_project(p_project)
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
	// If the window is not already open, open it and load the QML after setting up the context properties
	if (m_appQmlEngine->rootObjects().isEmpty())
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

		if (const QQuickWindow* l_window{ qobject_cast<QQuickWindow*>(m_appQmlEngine->rootObjects().first()) }; l_window)
		{
			QObject::connect(l_window, &QQuickWindow::closing, this, &ProjectWindow::onProjectWindowClosed);
		}
		else
		{
			qFatal("Cannot load QML component 'Asterindes/ProjectWindow'.");
		}
	}
	// Else if the window is already open, just focus it instead of opening a new one
	else
	{
		QQuickWindow* l_projectQmlWindow = qobject_cast<QQuickWindow*>(m_appQmlEngine->rootObjects().first());
		Q_ASSERT(l_projectQmlWindow);
		l_projectQmlWindow->raise();
		l_projectQmlWindow->requestActivate();
	}
}

void ProjectWindow::showStartupWindow() const
{
	AsterindesCore* l_coreApp = qobject_cast<AsterindesCore*>(parent());
	if (l_coreApp)
	{
		l_coreApp->showStartupWindow();
	}
}
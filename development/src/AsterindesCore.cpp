#include "AsterindesCore.h"

// Qt
#include <QSettings>
#include <QDir>

using namespace Asterindes;

AsterindesCore::AsterindesCore(int& argc, char** argv)
	: QGuiApplication(argc, argv)
{
	if (argc == 2)
	{
		m_startupProject = QString::fromLatin1(argv[1], strlen(argv[1]));
	}
}

bool AsterindesCore::start()
{
	// First check if another instance of the application is already running, if so, send the project path to it and exit.
	if (m_singleInstanceGuard->checkAndSendIfRunning(m_startupProject))
	{
		return false;
	}
	else
	{
		m_singleInstanceGuard->startListeningForConnections();

		// If the startup window is closed, check if the application should exit (if there are no open projects).
		QObject::connect(m_startupWindow, &Ui::StartupWindow::isVisibleChanged, this, [this](bool p_visible) {
			if (!p_visible)
			{
				applicationShouldExit();
			}
		});

		// If a project path is provided, open the project directly, otherwise show the startup window
		if (!m_startupProject.isEmpty())
		{
			openProject(QUrl::fromUserInput(m_startupProject));
		}
		else
		{
			showStartupWindow();
		}
		return true;
	}
}

bool AsterindesCore::openProject(const QUrl& p_projectPath)
{
	AsterindesProject* l_project{ new AsterindesProject(p_projectPath, this) };

	if (l_project->loadProject())
	{
		m_openedProjects.insert(p_projectPath, l_project);
		m_openedProjectWindows.insert(p_projectPath, new Ui::ProjectWindow(l_project, this, m_projectManagerService)).value()->openProjectWindow();

		QObject::connect(m_openedProjectWindows.value(p_projectPath), &Ui::ProjectWindow::projectWindowClosed, this, &AsterindesCore::onProjectCloseRequested);
		return true;
	}
	else
	{
		qCritical("Failed to load project: %s", qUtf8Printable(p_projectPath.toString()));
		l_project->deleteLater();
		return false;
	}
}

void AsterindesCore::applicationShouldExit()
{
	if (m_openedProjects.isEmpty() && !m_startupWindow->isVisible())
	{
		QGuiApplication::quit();
	}
}

void AsterindesCore::onProjectCloseRequested(AsterindesProject* p_project)
{
	QPointer<Ui::ProjectWindow> l_projectWindow{ m_openedProjectWindows.value(p_project->getProjectPath()) };
	QObject::disconnect(l_projectWindow, &Ui::ProjectWindow::projectWindowClosed, this, &AsterindesCore::onProjectCloseRequested);
	l_projectWindow->deleteLater();
	m_openedProjectWindows.remove(p_project->getProjectPath());

	m_openedProjects.remove(p_project->getProjectPath());
	p_project->deleteLater();

	applicationShouldExit();
}

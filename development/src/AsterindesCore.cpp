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

		// If a project path is provided, open the project directly, otherwise show the startup window
		if (!m_startupProject.isEmpty())
		{
			openProject(QUrl::fromUserInput(m_startupProject));
		}
		else
		{
			openStartupWindow();
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
		return true;
	}
	else
	{
		qCritical("Failed to load project: %s", qUtf8Printable(p_projectPath.toString()));
		l_project->deleteLater();
		return false;
	}
}

void AsterindesCore::onProjectCloseResquested(AsterindesProject* p_project)
{
	m_openedProjects.remove(p_project->getProjectPath());

	p_project->deleteLater();

	if (m_openedProjects.isEmpty())
	{
		saveProjectLocation(p_project->getProjectPath().toLocalFile());
		quit();
	}
}

void AsterindesCore::saveProjectLocation(const QString& pProjectPath) const
{
	QSettings l_settings;
	l_settings.setValue("projectLocation", pProjectPath);
}

QString AsterindesCore::getLastProjectLocation() const
{
	QSettings l_settings;
	return l_settings.value("projectLocation", "").toString();
}
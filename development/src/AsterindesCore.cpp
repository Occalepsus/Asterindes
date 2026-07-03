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
		openProject(QString::fromLatin1(argv[1], strlen(argv[1])));
	}
	else
	{
		m_startupWindow->setWindowVisible(true);
	}

	// TODO: put that in a specific startup window
	//QUrl l_lastProjectPath{ getLastProjectLocation() };

	//if (l_lastProjectPath.isEmpty())
	//{
	//	l_lastProjectPath = QUrl::fromLocalFile(QDir::home().filePath("AsterindesProjects/UntitledProject.asterindesproj"));
	//	QFile l_projectFile{ l_lastProjectPath.toLocalFile() };
	//	if (l_projectFile.open(QIODevice::WriteOnly | QIODeviceBase::NewOnly))
	//	{
	//		l_projectFile.write(sc_defaultProjectData.toJson());
	//		l_projectFile.close();
	//	}
	//	else
	//	{
	//		qCritical("Failed to create default project file: %s. Reason: %s", qUtf8Printable(l_lastProjectPath.toString()), qUtf8Printable(l_projectFile.errorString()));
	//		return;
	//	}
	//}

	//openProject(l_lastProjectPath.toLocalFile());
}

bool AsterindesCore::openProject(const QUrl& p_projectPath)
{
	AsterindesProject* l_project{ new AsterindesProject(p_projectPath, this) };

	if (l_project->loadProject())
	{
		m_openedProjects.insert(p_projectPath, l_project);
		m_openedProjectWindows.insert(p_projectPath, new Ui::ProjectWindow(l_project, this)).value()->openProjectWindow();
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
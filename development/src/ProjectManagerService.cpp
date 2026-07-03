#include "ProjectManagerService.h"

// Asterindes
#include "AsterindesCore.h"

// Qt
#include <QSettings>
#include <QFile>

using namespace Asterindes;

ProjectManagerService::ProjectManagerService(AsterindesCore* p_coreApp, QObject* parent)
	: QObject(parent)
	, m_coreApp{ p_coreApp }
{
	if (!m_coreApp)
	{
		qFatal("AsterindesCore instance is null");
	}

	loadRecentProjectList();
}

bool ProjectManagerService::loadProject(const QUrl& p_projectPath)
{
	bool l_result{ m_coreApp->openProject(p_projectPath) };

	if (l_result)
	{
		updateRecentProjectList(p_projectPath);
	}
	else
	{
		m_errorString = QString("Failed to load project: %1").arg(p_projectPath.toString());
	}

	return l_result;
}

bool ProjectManagerService::createProject(const QUrl& p_fileName)
{
	QFile l_newProjectFile(p_fileName.toLocalFile());

	if (l_newProjectFile.exists())
	{
		m_errorString = QString("Project file already exists: %1").arg(p_fileName.toString());
		return false;
	}
	
	if (l_newProjectFile.open(QIODevice::NewOnly | QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		QJsonObject l_projectData;
		l_projectData["resources"] = QJsonArray(); // Initialize with an empty resources array
	
		QJsonDocument l_jsonDoc(l_projectData);
		l_newProjectFile.write(l_jsonDoc.toJson());
	}
	else
	{
		m_errorString = QString("Failed to create project file: %1").arg(p_fileName.toString());
		return false;
	}

	// Close the file and load the created project
	l_newProjectFile.close();
	return loadProject(p_fileName);
}

void ProjectManagerService::loadRecentProjectList()
{
	QSettings l_settings;
	m_recentProjectList.clear();

	int size = l_settings.beginReadArray("recentProjects");
	for (int i = 0; i < size; ++i)
	{
		l_settings.setArrayIndex(i);
		m_recentProjectList.append(l_settings.value("path").toUrl());
	}
	l_settings.endArray();
}

void ProjectManagerService::updateRecentProjectList(const QUrl& p_lastProjectPath)
{
	QSettings l_settings;

	// Remove the project if it already exists in the list
	m_recentProjectList.removeAll(p_lastProjectPath);

	// Add the project to the beginning of the list
	m_recentProjectList.prepend(p_lastProjectPath);
	
	// Save the updated list back to the settings
	l_settings.beginWriteArray("recentProjects");
	for (int i = 0; i < m_recentProjectList.size(); ++i)
	{
		l_settings.setArrayIndex(i);
		l_settings.setValue("path", m_recentProjectList.at(i));
	}
	l_settings.endArray();

	emit recentProjectListChanged(m_recentProjectList);
}
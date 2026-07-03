#include "ProjectManagerService.h"

// Qt
#include <QSettings>

using namespace Asterindes;

ProjectManagerService::ProjectManagerService(QObject* parent)
	: QObject(parent)
{
}

bool ProjectManagerService::createProject(const QUrl& p_fileName)
{
	m_errorString = QString("Failed to create project \"%1\": Not implemented yet").arg(p_fileName.toString());
	qCritical(qPrintable(m_errorString));
	return false;
}

bool ProjectManagerService::loadProject(const QUrl& p_projectPath)
{
	m_errorString = QString("Failed to load project: %1. Reason: Not implemented yet").arg(p_projectPath.toString());
	qCritical(qPrintable(m_errorString));
	return false;
}

void ProjectManagerService::loadRecentProjectList()
{
	QSettings l_settings;
	m_recentProjectList.clear();

	int size = l_settings.beginReadArray("recentProjects");
	for (int i = 0; i < size; ++i) {
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
	for (int i = 0; i < m_recentProjectList.size(); ++i) {
		l_settings.setArrayIndex(i);
		l_settings.setValue("path", m_recentProjectList.at(i));
	}
	l_settings.endArray();

	emit recentProjectListChanged(m_recentProjectList);
}
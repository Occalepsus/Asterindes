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

void ProjectManagerService::loadRecentProjects()
{
	QSettings l_settings;
	m_recentProjects.clear();

	int size = l_settings.beginReadArray("recentProjects");
	for (int i = 0; i < size; ++i) {
		l_settings.setArrayIndex(i);
		m_recentProjects.append(l_settings.value("path").toUrl());
	}
	l_settings.endArray();
}

void ProjectManagerService::updateRecentProjects(const QUrl& p_lastProjectPath)
{
	QSettings l_settings;

	// Remove the project if it already exists in the list
	m_recentProjects.removeAll(p_lastProjectPath);

	// Add the project to the beginning of the list
	m_recentProjects.prepend(p_lastProjectPath);
	
	// Save the updated list back to the settings
	l_settings.beginWriteArray("recentProjects");
	for (int i = 0; i < m_recentProjects.size(); ++i) {
		l_settings.setArrayIndex(i);
		l_settings.setValue("path", m_recentProjects.at(i));
	}
	l_settings.endArray();

	emit recentProjectsChanged(m_recentProjects);
}
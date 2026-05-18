#include "ProjectViewModel.h"

using namespace Asterindes::Ui;

ProjectViewModel::ProjectViewModel(QPointer<AsterindesProject> p_projectManager, QObject* p_parent)
	: QObject(p_parent)
	, m_projectManager(p_projectManager)
	, m_resourceBroadcastManager(p_projectManager ? p_projectManager->getResourceBroadcastManager() : nullptr)
{
	QObject::connect(m_projectManager, &AsterindesProject::projectLoaded, this, &ProjectViewModel::onProjectLoaded);
	QObject::connect(m_resourceBroadcastManager, &ResourceBroadcastManager::broadcastedResourceChanged, this, &ProjectViewModel::broadcastedResourceChanged);
}

void ProjectViewModel::loadProject(const QUrl& p_projectPath)
{
}

void ProjectViewModel::setBroadcastedResourceUrl(const QUrl& p_url)
{
	if (m_resourceBroadcastManager)
	{
		// TODO: Check for error and display it if needed
		m_resourceBroadcastManager->setBroadcastedResourceFromUrl(p_url);

		emit broadcastedResourceChanged();
	}
}

void ProjectViewModel::onProjectLoaded(const QUrl& p_projectPath)
{

}

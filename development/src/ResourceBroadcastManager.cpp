#include "ResourceBroadcastManager.h"

// Asterindes
#include "AsterindesProject.h"

using namespace Asterindes;

ResourceBroadcastManager::ResourceBroadcastManager(AsterindesProject* p_parent)
	: QObject(p_parent)
	, m_resourceRegistry(p_parent ? p_parent->getResourceRegistry() : nullptr)
{
	m_broadcastServer->start();
}

void ResourceBroadcastManager::setBroadcastedResource(const ResourceRegistry::Resource& p_resource)
{
	if (m_broadcastedResource.m_resourceUrl != p_resource.m_resourceUrl)
	{
		m_broadcastedResource = p_resource;
		m_broadcastServer->setBroadcastResourceUrl(p_resource.m_resourceUrl);

		emit broadcastedResourceChanged();
	}
}

bool ResourceBroadcastManager::setBroadcastedResourceFromUrl(const QUrl& p_resourceUrl)
{
	// Defaults to true so if the URL is the same, it means it was valid
	bool l_resourceFound{ true };

	if (m_broadcastedResource.m_resourceUrl != p_resourceUrl && m_resourceRegistry)
	{
		if (auto [l_found, l_resource] {m_resourceRegistry->getResourceByUrl(p_resourceUrl)}; l_found)
		{
			m_broadcastedResource = l_resource;
			m_broadcastServer->setBroadcastResourceUrl(l_resource.m_resourceUrl);

			emit broadcastedResourceChanged();
		}
		else
		{
			l_resourceFound = false;
		}
	}
	return l_resourceFound;
}

#include "ResourceBroadcastManager.h"

using namespace Asterindes;

ResourceBroadcastManager::ResourceBroadcastManager(QObject* p_parent)
	: QObject(p_parent)
{
	m_broadcastServer->start();
}

void ResourceBroadcastManager::setBroadcastedResource(const ResourceRegistry::Resource& resource)
{
	if (m_broadcastedResource.m_resourceUrl != resource.m_resourceUrl)
	{
		m_broadcastedResource = resource;
	}
}
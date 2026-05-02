#include "ResourceListModel.h"

// STL
#include <source_location>

using namespace Asterindes::Ui;

ResourceListModel::ResourceListModel(QObject* p_parent)
	: QAbstractListModel(p_parent)
{
}

QVariant ResourceListModel::data(const QModelIndex& p_index, int p_role) const
{
	if (!p_index.isValid() || p_index.row() >= m_displayedResources.size())
	{
		return QVariant();
	}

	const ResourcesManager::Resource l_resource{ m_displayedResources[p_index.row()] };

	switch (auto l_roleEnum = static_cast<ResourceRoles>(p_role); l_roleEnum)
	{
		using enum ResourceRoles;
	case NameRole:
		return l_resource.m_name;
	case ResourceUrlRole:
		return l_resource.m_resourceUrl;
	default:
		return QVariant();
	}
}

void ResourceListModel::updateFromResourcesList(const QList<ResourcesManager::Resource>& p_resourceList)
{
	// Notify QML that we're about to insert new elements
	beginResetModel();

	m_displayedResources = p_resourceList;

	endResetModel();
}

int ResourceListModel::getResourceIndex(const QUrl& p_resourceUrl) const
{
	auto it{ std::ranges::find_if(m_displayedResources, [&p_resourceUrl](const ResourcesManager::Resource& p_resource)
		{
			return p_resource.m_resourceUrl == p_resourceUrl;
		}) - m_displayedResources.begin() };

	return (it != m_displayedResources.size()) ? static_cast<int>(it) : -1;
}
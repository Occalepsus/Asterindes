#include "ResourceListModel.h"

using namespace Asterindes::Ui;

ResourceListModel::ResourceListModel(QObject* p_parent)
	: QAbstractListModel(p_parent)
{
	qmlRegisterType<ResourceListModel>("Asterindes", 1, 0, "ResourceListModel");
}

QVariant ResourceListModel::data(const QModelIndex& p_index, int p_role) const
{
	if (!p_index.isValid() || p_index.row() >= m_displayedResources.size())
	{
		return QVariant();
	}

	const ResourceRegistry::Resource l_resource{ m_displayedResources[p_index.row()] };

	switch (auto l_roleEnum = static_cast<ResourceRoles>(p_role); l_roleEnum)
	{
		using enum ResourceRoles;
	case NameRole:
		return l_resource.m_name;
	case ResourceUrlRole:
		return l_resource.m_resourceUrl;
	case CreationDateRole:
		return l_resource.m_creationDate;
	case TagListRole:
	{
		QList l_tagsList(l_resource.m_tags.begin(), l_resource.m_tags.end());
		std::ranges::sort(l_tagsList, [] (const QString& a, const QString& b) {
			return QString::compare(a, b, Qt::CaseInsensitive) < 0;
		});
		return QVariant::fromValue(l_tagsList);
	}
	default:
		return QVariant();
	}
}

void ResourceListModel::updateFromResourcesList(const QList<ResourceRegistry::Resource>& p_resourceList)
{
	// Notify QML that we're about to insert new elements
	beginResetModel();

	m_displayedResources = p_resourceList;

	endResetModel();
}

bool ResourceListModel::updateFromSingleResource(const ResourceRegistry::Resource& p_resource)
{
	auto it{ std::ranges::find_if(m_displayedResources, [&p_resource](const ResourceRegistry::Resource& p_existingResource)
		{
			return p_existingResource.m_resourceUrl == p_resource.m_resourceUrl;
		}) };
	if (it != m_displayedResources.end())
	{
		const int l_index{ static_cast<int>(std::distance(m_displayedResources.begin(), it)) };
		m_displayedResources[l_index] = p_resource;
		QModelIndex l_modelIndex = index(l_index, 0);
		emit dataChanged(l_modelIndex, l_modelIndex);
		return false;
	}
	else
	{
		beginInsertRows(QModelIndex(), static_cast<int>(m_displayedResources.size()), static_cast<int>(m_displayedResources.size()));
		m_displayedResources.push_back(p_resource);
		endInsertRows();
		return true;
	}
}

int ResourceListModel::getResourceIndex(const QUrl& p_resourceUrl) const
{
	auto it{ std::ranges::find_if(m_displayedResources, [&p_resourceUrl](const ResourceRegistry::Resource& p_resource)
		{
			return p_resource.m_resourceUrl == p_resourceUrl;
		}) - m_displayedResources.begin() };

	return (it != m_displayedResources.size()) ? static_cast<int>(it) : -1;
}
#include "ResourcesViewModel.h"

#include <utility>

using namespace Asterindes::Ui;

ResourcesViewModel::ResourcesViewModel(ResourceRegistry* p_resourceRegistry, QObject* p_parent)
	: QObject(p_parent)
	, m_resourceRegistry(p_resourceRegistry)
{
	// ResourceRegistry is required for this ViewModel to function
	Q_ASSERT(m_resourceRegistry);

	// Connect to business logic signals
	QObject::connect(m_resourceRegistry, &ResourceRegistry::resourceListChanged, this, &ResourcesViewModel::updateResourceList);
	QObject::connect(m_resourceRegistry, &ResourceRegistry::resourceTagChanged, this, [this](const QUrl& p_resourceUrl, const QString&, bool) {
		if (m_resourceRegistry)
		{
			if (auto [l_found, l_resource] = m_resourceRegistry->getResourceByUrl(p_resourceUrl); l_found)
			{
				updateSingleResource(l_resource);
			}
		}
	});

	m_resourcesSortFilterProxyModel->setSourceModel(m_resourcesListModel);
	QObject::connect(m_resourcesSortFilterProxyModel, &ResourceSortFilterProxyModel::nameSearchFilterChanged, this, &ResourcesViewModel::selectedResourceIndexChanged);
	QObject::connect(m_resourcesSortFilterProxyModel, &ResourceSortFilterProxyModel::tagFilterListChanged, this, &ResourcesViewModel::selectedResourceIndexChanged);
	QObject::connect(m_resourcesSortFilterProxyModel, &ResourceSortFilterProxyModel::sortRoleChanged, this, &ResourcesViewModel::selectedResourceIndexChanged);
	QObject::connect(m_resourcesSortFilterProxyModel, &ResourceSortFilterProxyModel::sortOrderChanged, this, &ResourcesViewModel::selectedResourceIndexChanged);
	
	// Initialize model with current data
	updateResourceList();
}

// TODO: Do this check in the manager instead and return an error message if it fails, so we can display it in the UI
bool ResourcesViewModel::canAddResource(const QUrl& p_url) const
{
	// Resource validation before adding it to the project
	if (!p_url.isValid() || p_url.isEmpty())
	{
		return false;
	}

	QString l_path = p_url.path(QUrl::ComponentFormattingOption::FullyDecoded).toLower();
	return l_path.endsWith(".png") || l_path.endsWith(".jpg") || 
		   l_path.endsWith(".jpeg") || l_path.endsWith(".webp");
}

bool ResourcesViewModel::addResources(const QList<QUrl>& p_resourceUrls)
{
	if (!m_resourceRegistry)
	{
		return false;
	}

	setLoading(true);

	for (const auto& l_resourceUrl : p_resourceUrls)
	{

		if (!canAddResource(l_resourceUrl))
		{
			emit errorOccurred(tr("Invalid resource URL: %1").arg(l_resourceUrl.toString()));
			setLoading(false);
			return false;
		}
	}

	// Call resource registry addResources, it worked if all the resources were added.
	QList<QUrl> l_addedResources{ m_resourceRegistry->addResources(p_resourceUrls) };
	bool l_success{ l_addedResources.size() == p_resourceUrls.size() };
	
	if (!l_success)
	{
		for (const auto& l_resourceToAdd : p_resourceUrls)
		{
			if (!l_addedResources.contains(l_resourceToAdd))
			{
				emit errorOccurred(tr("Failed to add resource: %1").arg(l_resourceToAdd.fileName()));
			}
		}
	}

	// Update selection for the last added resource (first element in the array), clear selection if none were added
	m_selectedResourceUrl = !l_addedResources.isEmpty() ? l_addedResources.first() : QUrl();
	emit selectedResourceIndexChanged();
	
	setLoading(false);
	return l_success;
}

bool ResourcesViewModel::removeResources(const QList<QUrl>& p_resourceUrls)
{
	if (!m_resourceRegistry)
	{
		return false;
	}

	setLoading(true);

	QList<QUrl> l_removedResources{ m_resourceRegistry->removeResources(p_resourceUrls) };
	const bool l_success{ l_removedResources.size() == p_resourceUrls.size() };
	if (!l_success)
	{
		for (const auto& l_resourceToRemove : p_resourceUrls)
		{
			if (!l_removedResources.contains(l_resourceToRemove))
			{
				emit errorOccurred(tr("Failed to remove resource: %1").arg(l_resourceToRemove.fileName()));
			}
		}
	}

	setLoading(false);
	return l_success;
}

bool ResourcesViewModel::renameResource(const QUrl& p_resourceUrl, const QString& p_newName)
{
	if (!m_resourceRegistry)
	{
		return false;
	}

	setLoading(true);

	const bool l_success{ m_resourceRegistry->renameResource(p_resourceUrl, p_newName) };
	if (!l_success)
	{
		emit errorOccurred(tr("Failed to rename resource: %1").arg(p_resourceUrl.toString()));
	}

	setLoading(false);
	return l_success;
}

bool ResourcesViewModel::setResourceTag(const QUrl& p_resourceUrl, const QString& p_tag, bool p_add)
{
	if (!m_resourceRegistry)
	{
		return false;
	}

	setLoading(true);

	const bool l_success{ p_add ? m_resourceRegistry->addTagToResource(p_resourceUrl, p_tag) : m_resourceRegistry->removeTagFromResource(p_resourceUrl, p_tag) };
	if (!l_success)
	{
		emit errorOccurred(tr("Failed to %1 tag: %2 to resource: %3").arg(p_add ? "add" : "remove", p_tag, p_resourceUrl.toString()));
	}

	setLoading(false);
	return l_success;
}

int ResourcesViewModel::getSelectedResourceIndex() const
{
	int l_index{ -1 };

	if (m_selectedResourceUrl.isValid() && !m_selectedResourceUrl.isEmpty())
	{
		for (int i = 0; i < m_resourcesSortFilterProxyModel->rowCount(); ++i)
		{
			QModelIndex lModelIndex = m_resourcesSortFilterProxyModel->index(i, 0);
			QUrl l_resourceUrl = m_resourcesSortFilterProxyModel->data(lModelIndex, std::to_underlying(ResourceListModel::ResourceRoles::ResourceUrlRole)).toUrl();
			if (l_resourceUrl == m_selectedResourceUrl)
			{
				l_index = i;
				break;
			}
		}
	}

	return l_index;
}

void ResourcesViewModel::setSelectedResourceIndex(int p_index)
{
	if (p_index < 0 || p_index >= m_resourcesSortFilterProxyModel->rowCount())
	{
		p_index = -1; // Normalize empty selection
	}

	QModelIndex l_modelIndex = m_resourcesSortFilterProxyModel->index(p_index, 0);

	QUrl l_newSelectedUrl{ m_resourcesSortFilterProxyModel->data(l_modelIndex, std::to_underlying(ResourceListModel::ResourceRoles::ResourceUrlRole)).toUrl() };

	if (m_selectedResourceUrl != l_newSelectedUrl)
	{
		m_selectedResourceUrl = l_newSelectedUrl;
		emit selectedResourceIndexChanged();
	}
}

QVariantMap ResourcesViewModel::getResourceAtIndex(int p_index) const
{
	// TODO: How to improve this?
	QVariantMap l_resourceMap;

	QModelIndex l_modelIndex = m_resourcesSortFilterProxyModel->index(p_index, 0);

	l_resourceMap["name"] = m_resourcesSortFilterProxyModel->data(l_modelIndex, std::to_underlying(ResourceListModel::ResourceRoles::NameRole));
	l_resourceMap["resourceUrl"] = m_resourcesSortFilterProxyModel->data(l_modelIndex, std::to_underlying(ResourceListModel::ResourceRoles::ResourceUrlRole));
	l_resourceMap["tagList"] = m_resourcesSortFilterProxyModel->data(l_modelIndex, std::to_underlying(ResourceListModel::ResourceRoles::TagListRole));

	return l_resourceMap;
}

QList<QString> ResourcesViewModel::getAllResourceTags() const
{
	QList<QString> l_allTags;

	if (m_resourceRegistry)
	{
		QList<ResourceRegistry::Resource> l_resourcesList{ m_resourceRegistry->getResourcesList() };

		// Temporary set to avoid duplicates
		QSet<QString> l_tagsSet;

		for (const auto& l_resource : l_resourcesList)
		{
			l_tagsSet.unite(l_resource.m_tags);
		}

		l_allTags = l_tagsSet.values();
		std::ranges::sort(l_allTags);
	}

	return l_allTags;
}

void ResourcesViewModel::updateResourceList()
{
	QList<ResourceRegistry::Resource> l_resourcesList{ m_resourceRegistry->getResourcesList() };

	std::ranges::sort(l_resourcesList,
		[](const ResourceRegistry::Resource& p_a, const ResourceRegistry::Resource& p_b) {
			return p_a.m_name < p_b.m_name;
		});

	m_resourcesListModel->updateFromResourcesList(l_resourcesList);

	emit displayedResourceListChanged();

	// Update selection in case the selected resource was modified/removed
	emit selectedResourceIndexChanged();
}

void ResourcesViewModel::updateSingleResource(const ResourceRegistry::Resource& p_resource)
{
	m_resourcesListModel->updateFromSingleResource(p_resource);
	if (p_resource.m_resourceUrl == m_selectedResourceUrl)
	{
		emit selectedResourceTagListChanged();
	}
}

void ResourcesViewModel::setLoading(bool p_loading)
{
	if (m_isLoading != p_loading)
	{
		m_isLoading = p_loading;
		emit isLoadingChanged();
	}
}
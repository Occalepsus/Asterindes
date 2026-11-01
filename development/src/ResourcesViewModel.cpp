#include "ResourcesViewModel.h"

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

QVariantMap ResourcesViewModel::resourceToVariantMap(const ResourceRegistry::Resource& p_resource)
{
	QVariantMap l_resourceMap;

	QList l_tagsList(p_resource.m_tags.begin(), p_resource.m_tags.end());
	std::ranges::sort(l_tagsList, [](const QString& a, const QString& b) {
		return QString::compare(a, b, Qt::CaseInsensitive) < 0;
	});

	l_resourceMap["name"] = p_resource.m_name;
	l_resourceMap["resourceUrl"] = p_resource.m_resourceUrl;
	l_resourceMap["resourceType"] = std::to_underlying(p_resource.m_type);
	l_resourceMap["creationDate"] = p_resource.m_creationDate;
	l_resourceMap["tagList"] = QVariant::fromValue(l_tagsList);

	return l_resourceMap;
}

bool ResourcesViewModel::addResources(const QList<QString>& p_resourceUrls)
{
	if (!m_resourceRegistry)
	{
		return false;
	}

	setLoading(true);

	QList<QUrl> l_resourcesToAddUrlDecoded;
	std::ranges::transform(p_resourceUrls, std::back_inserter(l_resourcesToAddUrlDecoded), [](const QString& p_urlString) { return QUrl::fromUserInput(p_urlString); });

	// Call resource registry addResources, it worked if all the resources were added.
	QList<QUrl> l_addedResources{ m_resourceRegistry->addResources(l_resourcesToAddUrlDecoded) };
	bool l_success{ l_addedResources.size() == l_resourcesToAddUrlDecoded.size() };
	
	if (!l_success)
	{
		for (const QUrl& l_resourceToAdd : l_resourcesToAddUrlDecoded)
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

bool ResourcesViewModel::removeResources(const QList<QString>& p_resourceUrls)
{
	if (!m_resourceRegistry)
	{
		return false;
	}

	setLoading(true);

	QList<QUrl> l_resourcesToRemoveUrlDecoded;
	std::ranges::transform(p_resourceUrls, std::back_inserter(l_resourcesToRemoveUrlDecoded), [](const QString& p_urlString) { return QUrl::fromUserInput(p_urlString); });

	QList<QUrl> l_removedResources{ m_resourceRegistry->removeResources(l_resourcesToRemoveUrlDecoded) };
	const bool l_success{ l_removedResources.size() == l_resourcesToRemoveUrlDecoded.size() };
	if (!l_success)
	{
		for (const QUrl& l_resourceToRemove : l_resourcesToRemoveUrlDecoded)
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

bool ResourcesViewModel::renameResource(const QString& p_resourceUrl, const QString& p_newName)
{
	if (!m_resourceRegistry)
	{
		return false;
	}

	setLoading(true);

	const bool l_success{ m_resourceRegistry->renameResource(QUrl::fromUserInput(p_resourceUrl), p_newName) };
	if (!l_success)
	{
		emit errorOccurred(tr("Failed to rename resource: %1").arg(p_resourceUrl));
	}

	setLoading(false);
	return l_success;
}

bool ResourcesViewModel::setResourceTag(const QString& p_resourceUrl, const QString& p_tag, bool p_add)
{
	if (!m_resourceRegistry)
	{
		return false;
	}

	setLoading(true);

	const bool l_success{ p_add ? m_resourceRegistry->addTagToResource(QUrl::fromUserInput(p_resourceUrl), p_tag) : m_resourceRegistry->removeTagFromResource(QUrl::fromUserInput(p_resourceUrl), p_tag) };
	if (!l_success)
	{
		emit errorOccurred(tr("Failed to %1 tag: %2 to resource: %3").arg(p_add ? "add" : "remove", p_tag, p_resourceUrl));
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
	QModelIndex l_modelIndex = m_resourcesSortFilterProxyModel->index(p_index, 0);

	QVariantMap l_resourceMap = resourceToVariantMap(m_resourcesSortFilterProxyModel->data(l_modelIndex, std::to_underlying(ResourceListModel::ResourceRoles::StructReferenceRole)).value<ResourceRegistry::Resource>());

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
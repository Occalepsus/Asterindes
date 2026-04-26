#include "ResourcesViewModel.h"

#include <utility>

using namespace Asterindes::Ui;

ResourcesViewModel::ResourcesViewModel(ResourcesManager& p_resourcesManager, QObject* p_parent)
	: QObject(p_parent)
	, m_resourcesManager(p_resourcesManager)
	, m_resourcesListModel(this)
{
	// Connect to business logic signals
	QObject::connect(&m_resourcesManager, &ResourcesManager::resourcesUpdated, this, &ResourcesViewModel::onManagerResourcesChanged);
	
	// Initialize model with current data
	onManagerResourcesChanged();
}

void ResourcesViewModel::setSelectedResourceId(int p_id)
{
	if (m_selectedResourceId != p_id)
	{
		m_selectedResourceId = p_id;
		emit selectedResourceIdChanged();
	}
}

bool ResourcesViewModel::addResource(const QUrl& p_resourceUrl)
{
	if (!canAddResource(p_resourceUrl))
	{
		emit errorOccurred(tr("Invalid resource URL: %1").arg(p_resourceUrl.toString()));
		return false;
	}

	setLoading(true);
	
	// Delegate to business logic
	bool l_success{ m_resourcesManager.addResource(p_resourceUrl) };
	
	if (!l_success)
	{
		emit errorOccurred(tr("Failed to add resource: %1").arg(p_resourceUrl.fileName()));
	}
	
	setLoading(false);
	return l_success;
}

bool ResourcesViewModel::removeResource(const QUrl& p_resourceUrl)
{
	setLoading(true);

	// TODO: implement removeResource in ResourcesManager and call it here
	bool l_success = false;
	//bool l_success = m_resourcesManager.removeResource(p_resourceUrl);
	//
	//if (!l_success)
	//{
	//    emit errorOccurred(tr("Failed to remove resource: %1").arg(p_resourceUrl.fileName()));
	//}
	emit errorOccurred(tr("Remove not implemented"));
	
	setLoading(false);
	return l_success;
}

bool ResourcesViewModel::canAddResource(const QUrl& p_url) const
{
	// Resource validation before adding it to the project
	if (!p_url.isValid() || p_url.isEmpty())
	{
		return false;
	}

	QString l_path = p_url.toString().toLower();
	return l_path.endsWith(".png") || l_path.endsWith(".jpg") || 
		   l_path.endsWith(".jpeg") || l_path.endsWith(".webp");
}

QVariantMap ResourcesViewModel::getResourceAtIndex(int p_index) const
{
	if (p_index < 0 || p_index >= m_resourcesListModel.rowCount())
	{
		return QVariantMap();
	}

	// TODO: How to improve this?
	QVariantMap l_resourceMap;

	QModelIndex lModelIndex = m_resourcesListModel.index(p_index, 0);

	l_resourceMap["name"] = m_resourcesListModel.data(lModelIndex, std::to_underlying(ResourceListModel::ResourceRoles::NameRole));
	l_resourceMap["resourceUrl"] = m_resourcesListModel.data(lModelIndex, std::to_underlying(ResourceListModel::ResourceRoles::ResourceUrlRole));

	return l_resourceMap;
}

void ResourcesViewModel::onManagerResourcesChanged()
{
	// Get the current resources list from the manager
	const ResourcesManager::ResourceList& l_managerResourcesList{ m_resourcesManager.getResourcesList() };

	// Vector of resources to be sorted and displayed by the model
	std::vector<const ResourcesManager::Resource*> l_displayedResourceList{};
	l_displayedResourceList.reserve(l_managerResourcesList.size());

	// Copy the resources pointer in the displayed resources list
	std::transform(l_managerResourcesList.begin(), l_managerResourcesList.end(),
		std::back_inserter(l_displayedResourceList),
		[](const std::unique_ptr<ResourcesManager::Resource>& p_resource) {
			return p_resource.get();
		});

	// Sort the displayed resources by name
	std::sort(l_displayedResourceList.begin(), l_displayedResourceList.end(),
		[](const ResourcesManager::Resource* p_a, const ResourcesManager::Resource* p_b) {
			return p_a->m_name < p_b->m_name;
		});

	// Update the presentation model
	m_resourcesListModel.updateFromResourcesList(l_displayedResourceList);
	
	emit displayedResourceListChanged();
}

void ResourcesViewModel::setLoading(bool p_loading)
{
	if (m_isLoading != p_loading)
	{
		m_isLoading = p_loading;
		emit isLoadingChanged();
	}
}
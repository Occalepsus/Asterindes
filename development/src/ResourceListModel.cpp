#include "ResourceListModel.h"

// STL
#include <source_location>

using namespace Asterindes::Ui;

ResourceListModel::ResourceListModel(QObject* p_parent, const ResourcesManager& p_resourcesManager)
	: QAbstractListModel(p_parent)
	, m_resourcesManager(p_resourcesManager)
{
	// Initialize the list model
	updateResourceListModel();

	// Connect the signal from the resources manager to update the list model when the resources are updated
	QObject::connect(&m_resourcesManager, &ResourcesManager::resourcesUpdated, this, &ResourceListModel::updateResourceListModel);
}

void ResourceListModel::setupQmlModel(const QQuickWindow* p_window)
{
	// Expose the resources list model to QML
	QObject* l_resourceGridView{ p_window->findChild<QObject*>(QAnyStringView("resourceGridView")) };
	if (!l_resourceGridView)
	{
		qFatal("%s(): Cannot find QML element with objectName 'resourceGridView'. The resources list model will not be displayed in the UI.", std::source_location::current().function_name());
	}
	l_resourceGridView->setProperty("model", QVariant::fromValue(this));

	// Connect the signal from the QML resources panel to the slot in the resources manager to handle the resource add request
	const QObject* l_resourcesPanel{ p_window->findChild<QObject*>("resourcesGridPanel") };
	auto a = QObject::connect(l_resourcesPanel, SIGNAL(qmlResourceAddRequest(QUrl)), &m_resourcesManager, SLOT(addResource(QUrl)));
}

QVariant ResourceListModel::data(const QModelIndex& p_index, int p_role) const
{
	if (!p_index.isValid() || p_index.row() >= m_displayedResources.size())
	{
		return QVariant();
	}

	const ResourcesManager::Resource* l_resource = m_displayedResources[p_index.row()];

	switch (auto l_roleEnum = static_cast<ResourceRoles>(p_role); l_roleEnum)
	{
		using enum ResourceRoles;
	case NameRole:
		return l_resource->m_name;
	case ResourceUrlRole:
		return l_resource->m_resourceUrl;
	default:
		return QVariant();
	}
}

void ResourceListModel::updateResourceListModel()
{
	// Notify QML that we're about to insert new elements
	beginResetModel();
	m_displayedResources.clear();

	const ResourcesManager::ResourceList& l_managerList{ m_resourcesManager.getResourcesList() };
	
	// Reserve memory to avoid multiple reallocations
	m_displayedResources.reserve(l_managerList.size());
	
	// Copy the resources pointer in the displayed resources list
	std::transform(l_managerList.begin(), l_managerList.end(), 
		std::back_inserter(m_displayedResources),
		[](const std::unique_ptr<ResourcesManager::Resource>& p_resource) {
			return p_resource.get();
		});

	// Sort the displayed resources by name
	std::sort(m_displayedResources.begin(), m_displayedResources.end(), 
		[](const ResourcesManager::Resource* p_a, const ResourcesManager::Resource* p_b) {
			return p_a->m_name < p_b->m_name;
		});
	endResetModel();
}

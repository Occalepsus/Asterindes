#include "ResourceListModel.h"

// STL
#include <source_location>

using namespace Asterindes::Ui;

ResourceListModel::ResourceListModel(QObject* p_parent)
	: QAbstractListModel(p_parent)
{
}

void ResourceListModel::setupQmlModel(const QQuickWindow* p_window)
{
	// Expose the resources list model to QML
	QObject* l_resourceGridView{ p_window->findChild<QObject*>(QAnyStringView("resourceGridView")) };
	if (!l_resourceGridView)
	{
		qFatal("Cannot find QML element with objectName 'resourceGridView'. The resources list model will not be displayed in the UI.");
	}
	l_resourceGridView->setProperty("model", QVariant::fromValue(this));

	// Connect the signal from the QML resources panel to the slot in the resources manager to handle the resource add request
	const QObject* l_resourcesPanel{ p_window->findChild<QObject*>("resourcesGridPanel") };
	//auto a = QObject::connect(l_resourcesPanel, SIGNAL(qmlResourceAddRequest(QUrl)), &m_resourcesManager, SLOT(addResource(QUrl)));
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

void ResourceListModel::updateFromResourcesList(const std::vector<const ResourcesManager::Resource*>& p_resourceList)
{
	// Notify QML that we're about to insert new elements
	beginResetModel();

	m_displayedResources = p_resourceList;

	endResetModel();
}

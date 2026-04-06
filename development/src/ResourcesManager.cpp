#include "ResourcesManager.h"

using namespace Asterindes;

QHash<int, QByteArray> Ui::ResourcesListModel::roleNames() const
{
	return m_roleNames;
}

int Ui::ResourcesListModel::rowCount(const QModelIndex& p_parent) const
{
	return static_cast<int>(m_displayedResources.size());
}

QVariant Ui::ResourcesListModel::data(const QModelIndex& p_index, int p_role) const
{
	if (!p_index.isValid() || p_index.row() >= m_displayedResources.size())
	{
		return QVariant();
	}

	const Resource* l_resource = m_displayedResources[p_index.row()];

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

void Ui::ResourcesListModel::updateDisplayedResources(const std::vector<const Resource*>& p_resourcesToDisplay)
{
	// Notify QML that we're about to insert new elements
	beginResetModel();
	m_displayedResources = p_resourcesToDisplay;
	std::sort(m_displayedResources.begin(), m_displayedResources.end(), [](const Resource* a, const Resource* b) {
		return a->m_name < b->m_name;
		});
	endResetModel();
}

/*****************************************************************************/

void ResourcesManager::testCreateResource()
{
	auto l_resource = std::make_unique<Resource>();
	l_resource->m_id = 0;
	l_resource->m_name = "Test Resource 0";
	l_resource->m_resourceUrl = QUrl::fromLocalFile(R"(C:\Users\jujuj\Nextcloud\Documents\Projets persos\JDR\Les Contes de le Faille\Par delà le carnaval de Sorcelume\Chapitre 4\Illustration\Amidor et Pollenela.webp)");

	m_resources.insert(std::move(l_resource));

	l_resource = std::make_unique<Resource>();
	l_resource->m_id = 1;
	l_resource->m_name = "Test Resource 1";
	l_resource->m_resourceUrl = QUrl::fromLocalFile(R"(C:\Users\jujuj\Nextcloud\Documents\Projets persos\JDR\Les Contes de le Faille\Par delà le carnaval de Sorcelume\Chapitre 4\Illustration\Kassam Thaldrîn.jpg)");

	m_resources.insert(std::move(l_resource));

	l_resource = std::make_unique<Resource>();
	l_resource->m_id = 2;
	l_resource->m_name = "Test Resource 2";
	l_resource->m_resourceUrl = QUrl::fromLocalFile(R"(C:\Users\jujuj\Nextcloud\Documents\Projets persos\JDR\Les Contes de le Faille\Par delà le carnaval de Sorcelume\Chapitre 4\Illustration\Tasha et Mordenkainen.jpg)");

	m_resources.insert(std::move(l_resource));

	l_resource = std::make_unique<Resource>();
	l_resource->m_id = 3;
	l_resource->m_name = "Test Resource 3";
	l_resource->m_resourceUrl = QUrl::fromLocalFile(R"(C:\Users\jujuj\Nextcloud\Documents\Projets persos\JDR\Les Contes de le Faille\Par delà le carnaval de Sorcelume\Chapitre 4\Illustration\Théatre.webp)");

	m_resources.insert(std::move(l_resource));

	l_resource = std::make_unique<Resource>();
	l_resource->m_id = 4;
	l_resource->m_name = "Test Resource 4";
	l_resource->m_resourceUrl = QUrl::fromLocalFile(R"(C:\Users\jujuj\Nextcloud\Documents\Projets persos\JDR\Les Contes de le Faille\Par delà le carnaval de Sorcelume\Chapitre 4\Illustration\Zoltorak.jpg)");

	m_resources.insert(std::move(l_resource));

	std::vector<const Resource*> l_resourcesToDisplay;
	for (const auto& l_resource : m_resources)
	{
		l_resourcesToDisplay.push_back(l_resource.get());
	}
	m_displayedResourcesListModel.updateDisplayedResources(l_resourcesToDisplay);
}

void ResourcesManager::addResource(const QUrl& p_resourceUrl)
{
	m_resources.emplace(std::make_unique<Resource>(Resource{ .m_id = m_resources.size(), .m_name = p_resourceUrl.fileName(), .m_resourceUrl = p_resourceUrl }));

	std::vector<const Resource*> l_resourcesToDisplay;
	for (const auto& l_resource : m_resources)
	{
		l_resourcesToDisplay.push_back(l_resource.get());
	}
	m_displayedResourcesListModel.updateDisplayedResources(l_resourcesToDisplay);
}

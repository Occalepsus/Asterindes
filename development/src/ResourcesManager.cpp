#include "ResourcesManager.h"

using namespace Asterindes;

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

	emit resourcesUpdated();

	//std::vector<const Resource*> l_resourcesToDisplay;
	//for (const auto& l_resource : m_resources)
	//{
	//	l_resourcesToDisplay.push_back(l_resource.get());
	//}
	//m_displayedResourcesListModel.updateResourceListModel(l_resourcesToDisplay);
}

void ResourcesManager::addResource(const QUrl& p_resourceUrl)
{
	m_resources.emplace(std::make_unique<Resource>(Resource{ .m_id = m_resources.size(), .m_name = p_resourceUrl.fileName(), .m_resourceUrl = p_resourceUrl }));

	emit resourcesUpdated();

	//std::vector<const Resource*> l_resourcesToDisplay;
	//for (const auto& l_resource : m_resources)
	//{
	//	l_resourcesToDisplay.push_back(l_resource.get());
	//}
	//m_displayedResourcesListModel.updateResourceListModel(l_resourcesToDisplay);
}

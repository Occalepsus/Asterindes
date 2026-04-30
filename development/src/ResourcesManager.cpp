#include "ResourcesManager.h"

using namespace Asterindes;

QList<ResourcesManager::Resource> ResourcesManager::getResourcesList() const
{
	QList<Resource> lResourcesList;
	lResourcesList.reserve(m_resources.size());

	// Extract raw pointers from unique_ptr
	for (const auto& [_, l_resource] : m_resources)
	{
		lResourcesList.push_back(l_resource);
	}

	return lResourcesList;
}

void ResourcesManager::testCreateResource()
{
	Resource l_resource{
		"Test Resource 0",
		QUrl::fromLocalFile(R"(C:\Users\jujuj\Nextcloud\Documents\Projets persos\JDR\Les Contes de le Faille\Par delà le carnaval de Sorcelume\Chapitre 4\Illustration\Amidor et Pollenela.webp)")
	};
	m_resources.try_emplace(l_resource.m_resourceUrl.toString(), std::move(l_resource));

	l_resource = Resource{
		"Test Resource 1",
		QUrl::fromLocalFile(R"(C:\Users\jujuj\Nextcloud\Documents\Projets persos\JDR\Les Contes de le Faille\Par delà le carnaval de Sorcelume\Chapitre 4\Illustration\Kassam Thaldrîn.jpg)")
	};
	m_resources.try_emplace(l_resource.m_resourceUrl.toString(), std::move(l_resource));

	l_resource = Resource{
		"Test Resource 2",
		QUrl::fromLocalFile(R"(C:\Users\jujuj\Nextcloud\Documents\Projets persos\JDR\Les Contes de le Faille\Par delà le carnaval de Sorcelume\Chapitre 4\Illustration\Tasha et Mordenkainen.jpg)")
	};
	m_resources.try_emplace(l_resource.m_resourceUrl.toString(), std::move(l_resource));

	l_resource = Resource{
		"Test Resource 3",
		QUrl::fromLocalFile(R"(C:\Users\jujuj\Nextcloud\Documents\Projets persos\JDR\Les Contes de le Faille\Par delà le carnaval de Sorcelume\Chapitre 4\Illustration\Théatre.webp)")
	};
	m_resources.try_emplace(l_resource.m_resourceUrl.toString(), std::move(l_resource));

	l_resource = Resource{
		"Test Resource 4",
		QUrl::fromLocalFile(R"(C:\Users\jujuj\Nextcloud\Documents\Projets persos\JDR\Les Contes de le Faille\Par delà le carnaval de Sorcelume\Chapitre 4\Illustration\Zoltorak.jpg)")
	};
	m_resources.try_emplace(l_resource.m_resourceUrl.toString(), std::move(l_resource));

	emit resourcesUpdated();
}

bool ResourcesManager::addResource(const QUrl& p_resourceUrl)
{
	auto [_, l_inserted] { m_resources.try_emplace(p_resourceUrl.toString(), Resource{.m_name = p_resourceUrl.fileName(), .m_resourceUrl = p_resourceUrl}) };

	if (l_inserted)
	{
		emit resourcesUpdated();
	}

	return l_inserted;
}

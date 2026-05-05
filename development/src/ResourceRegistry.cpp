#include "ResourceRegistry.h"

using namespace Asterindes;

bool ResourceRegistry::loadResourcesFromJson(const QJsonArray& p_resourceJsonArray)
{
	bool l_jsonValid = true;

	m_resources.clear();

	for (const auto& l_resourceJsonValue : p_resourceJsonArray)
	{
		if (!l_resourceJsonValue.isObject())
		{
			qWarning("Invalid resource JSON value, expected an object");
			l_jsonValid = false;
			continue;
		}

		const auto& l_resourceJsonObject = l_resourceJsonValue.toObject();
		if (!l_resourceJsonObject.contains("name") || !l_resourceJsonObject.contains("url"))
		{
			qWarning("Invalid resource JSON object, missing 'name' or 'url' field");
			l_jsonValid = false;
			continue;
		}

		const auto& l_name = l_resourceJsonObject["name"].toString();
		const auto& l_urlString = l_resourceJsonObject["url"].toString();
		const QUrl l_url{ l_urlString };
		if (!l_url.isValid())
		{
			qWarning("Invalid resource URL");
			continue;
		}
		m_resources.try_emplace(l_url.toString(), Resource{ .m_name = l_name, .m_resourceUrl = l_url });
	}

	return l_jsonValid;
}

QJsonArray ResourceRegistry::getResourcesAsJson() const
{
	QJsonArray l_resourceJsonArray;

	for (const auto& l_resource : m_resources.values())
	{
		l_resourceJsonArray.append(QJsonObject{
			{ "name", l_resource.m_name },
			{ "url", l_resource.m_resourceUrl.toString() }
		});
	}

	return l_resourceJsonArray;
}

QList<ResourceRegistry::Resource> ResourceRegistry::getResourcesList() const
{
	QList<Resource> lResourcesList;
	lResourcesList.reserve(m_resources.size());

	// Copy pointers to the resources in the hash to the list
	for (const auto& l_resource : m_resources.values())
	{
		lResourcesList.push_back(l_resource);
	}

	return lResourcesList;
}

std::pair<bool, ResourceRegistry::Resource> ResourceRegistry::getResourceByUrl(const QUrl& p_resourceUrl) const
{
	if (auto it{ m_resources.find(p_resourceUrl.toString()) };
		it != m_resources.end())
	{
		return { true, it.value() };
	}
	else
	{
		return { false, Resource{} };
	}
}

void ResourceRegistry::testCreateResource()
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

	emit resourcesChanged();
}

bool ResourceRegistry::addResource(const QUrl& p_resourceUrl)
{
	auto [_, l_inserted] { m_resources.try_emplace(p_resourceUrl.toString(), Resource{.m_name = p_resourceUrl.fileName(), .m_resourceUrl = p_resourceUrl}) };

	if (l_inserted)
	{
		emit resourcesChanged();
	}

	return l_inserted;
}

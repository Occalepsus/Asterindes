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
		const auto& l_tagsJsonArray = l_resourceJsonObject["tags"].toArray();
		QSet<QString> l_tags;
		std::ranges::transform(l_tagsJsonArray, std::inserter(l_tags, l_tags.end()), [](const QJsonValue& value) { return value.toString(); });

		m_resources.try_emplace(l_url.toString(), Resource{ .m_name = l_name, .m_resourceUrl = l_url, .m_tags = l_tags });
	}

	return l_jsonValid;
}

QJsonArray ResourceRegistry::getResourcesAsJson() const
{
	QJsonArray l_resourceJsonArray;

	for (const auto& l_resource : m_resources.values())
	{
		QJsonArray l_tagsJsonArray;
		for (const QString& l_tag : l_resource.m_tags)
		{
			l_tagsJsonArray.append(l_tag);
		}

		l_resourceJsonArray.append(QJsonObject{
			{ "name", l_resource.m_name },
			{ "url", l_resource.m_resourceUrl.toString() },
			{ "tags", l_tagsJsonArray }
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

QList<QUrl> ResourceRegistry::addResources(const QList<QUrl>& p_resourceUrls)
{
	QList<QUrl> l_added{};

	for (const auto& l_resourceUrl : p_resourceUrls)
	{
		// Try to emplace the new resource, and if it works add it to the list of added urls
		auto [_, l_inserted] { m_resources.try_emplace(l_resourceUrl.toString(), Resource{.m_name = l_resourceUrl.fileName(), .m_resourceUrl = l_resourceUrl}) };
		if (l_inserted)
		{
			l_added.append(l_resourceUrl);
		}
	}

	if (!l_added.isEmpty())
	{
		emit resourceListChanged();
		emit resourcesChanged();
	}

	return l_added;
}

QList<QUrl> ResourceRegistry::removeResources(const QList<QUrl>& p_resourceUrls)
{
	QList<QUrl> l_removed{};

	for (const auto& l_resourceUrl : p_resourceUrls)
	{
		if (auto it{ m_resources.find(l_resourceUrl.toString()) };
			it != m_resources.end())
		{
			l_removed.append(l_resourceUrl);
			m_resources.erase(it);
		}
	}

	if (!l_removed.isEmpty())
	{
		emit resourceListChanged();
		emit resourcesChanged();
	}

	return l_removed;
}

bool ResourceRegistry::renameResource(const QUrl& p_resourceUrl, const QString& p_newName)
{
	const QString l_cleanName{ p_newName.trimmed() };
	if (l_cleanName.isEmpty())
	{
		return false;
	}

	if (auto it{ m_resources.find(p_resourceUrl.toString()) };
		it != m_resources.end())
	{
		if (it->m_name == l_cleanName)
		{
			return true;
		}
		it->m_name = l_cleanName;

		emit resourceListChanged();
		emit resourcesChanged();
		return true;
	}
	else
	{
		return false;
	}
}

bool ResourceRegistry::addTagToResource(const QUrl& p_resourceUrl, const QString& p_tag)
{
	if (auto it{ m_resources.find(p_resourceUrl.toString()) };
		it != m_resources.end())
	{
		if (!it->m_tags.contains(p_tag))
		{
			it->m_tags.insert(p_tag);
			emit resourceTagChanged(p_resourceUrl, p_tag, true);
			emit resourcesChanged();
			return true;
		}
		else
		{
			// Tag already exists, consider it a success
			return true;
		}
	}

	return false;
}

bool ResourceRegistry::removeTagFromResource(const QUrl& p_resourceUrl, const QString& p_tag)
{
	if (auto it{ m_resources.find(p_resourceUrl.toString()) };
		it != m_resources.end())
	{
		if (it->m_tags.remove(p_tag))
		{
			emit resourceTagChanged(p_resourceUrl, p_tag, false);
			emit resourcesChanged();
			return true;
		}
		else
		{
			// Tag did not exist, consider it a success
			return true;
		}
	}
	return false;
}

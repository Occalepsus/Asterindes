#include "ResourceSortFilterProxyModel.h"

// Qt
#include <QFileInfo>
#include <QDateTime>

using namespace Asterindes::Ui;

ResourceSortFilterProxyModel::ResourceSortFilterProxyModel(QObject* p_parent)
	: QSortFilterProxyModel(p_parent)
{
	setDynamicSortFilter(true);
	setFilterCaseSensitivity(Qt::CaseInsensitive);
	setSortRole(std::to_underlying(ResourceListModel::ResourceRoles::NameRole));
	QSortFilterProxyModel::sort(0, Qt::AscendingOrder);
}

void ResourceSortFilterProxyModel::setNameSearchFilter(const QString& p_nameSearchFilter)
{
	if (m_nameSearchFilter == p_nameSearchFilter)
	{
		return;
	}

	beginFilterChange();
	m_nameSearchFilter = p_nameSearchFilter;
	endFilterChange();

	emit nameSearchFilterChanged(m_nameSearchFilter);
}

void ResourceSortFilterProxyModel::setTagFilterList(const QStringList& p_tagFilterList)
{
	if (m_tagFilterList == p_tagFilterList)
	{
		return;
	}

	beginFilterChange();
	m_tagFilterList = p_tagFilterList;
	endFilterChange();
	
	emit tagFilterListChanged(m_tagFilterList);
}

void ResourceSortFilterProxyModel::setSortOrder(Qt::SortOrder p_sortOrder)
{
	if (sortOrder() == p_sortOrder)
	{
		return;
	}

	invalidate();
	sort(0, p_sortOrder);
	
	emit sortOrderChanged(p_sortOrder);
}

void ResourceSortFilterProxyModel::clearFilters()
{
	setNameSearchFilter("");
	setTagFilterList({});
}

bool ResourceSortFilterProxyModel::filterAcceptsRow(int p_sourceRow, const QModelIndex& p_sourceParent) const
{
	const QModelIndex l_index{ sourceModel()->index(p_sourceRow, 0, p_sourceParent) };
	const QStringList l_resourceTags{ sourceModel()->data(l_index, std::to_underlying(ResourceListModel::ResourceRoles::TagListRole)).toStringList() };

	for (const QString& l_tag : m_tagFilterList)
	{
		if (!l_resourceTags.contains(l_tag))
		{
			return false;
		}
	}

	if (m_nameSearchFilter.isEmpty())
	{
		return true;
	}

	const QString l_name{ sourceModel()->data(l_index, std::to_underlying(ResourceListModel::ResourceRoles::NameRole)).toString() };

	return l_name.contains(m_nameSearchFilter, filterCaseSensitivity());
}

bool ResourceSortFilterProxyModel::lessThan(const QModelIndex& p_left, const QModelIndex& p_right) const
{
	constexpr auto l_nameRole{ std::to_underlying(ResourceListModel::ResourceRoles::NameRole) };
	constexpr auto l_creationDateRole{ std::to_underlying(ResourceListModel::ResourceRoles::CreationDateRole) };

	switch (auto l_sortRole{ static_cast<ResourceListModel::ResourceRoles>(sortRole()) })
	{
	case ResourceListModel::ResourceRoles::NameRole:
	{
		const QString l_leftName{ sourceModel()->data(p_left, l_nameRole).toString() };
		const QString l_rightName{ sourceModel()->data(p_right, l_nameRole).toString() };

		return QString::compare(l_leftName, l_rightName, filterCaseSensitivity()) < 0;
	}
	case ResourceListModel::ResourceRoles::CreationDateRole:
	{
		const QDateTime l_leftDate{ sourceModel()->data(p_left, l_creationDateRole).toDateTime() };
		const QDateTime l_rightDate{ sourceModel()->data(p_right, l_creationDateRole).toDateTime() };
		
		return l_leftDate < l_rightDate;
	}
	default:
		qWarning() << "Unsupported sort role" << l_sortRole;
		break;
	}

	return QSortFilterProxyModel::lessThan(p_left, p_right);
}
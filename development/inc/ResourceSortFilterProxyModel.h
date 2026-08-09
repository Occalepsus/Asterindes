#ifndef RESOURCESORTFILTERPROXYMODEL_H
#define RESOURCESORTFILTERPROXYMODEL_H

// Asterindes
#include "ResourceRegistry.h"
#include "ResourceListModel.h"

// Qt
#include <QSortFilterProxyModel>

namespace Asterindes::Ui
{
	/**
	 * ResourceSortFilterProxyModel class is a proxy model that is used to sort and filter the resources list model, it is used to display the resources as a list in the UI.
	 */
	class ResourceSortFilterProxyModel : public QSortFilterProxyModel
	{
		Q_OBJECT;

		/**
		 * The name search filter string, it is used to filter the resources list model based on the name of the resources.
		 */
		Q_PROPERTY(QString nameSearchFilter READ getNameSearchFilter WRITE setNameSearchFilter NOTIFY nameSearchFilterChanged);

		/**
		 * The tag filter list, it is used to filter the resources list model based on the tags of the resources.
		 */
		Q_PROPERTY(QStringList tagFilterList READ getTagFilterList WRITE setTagFilterList NOTIFY tagFilterListChanged);

		/**
		 * The sort order, it is used to sort the resources list model based on the name of the resources.
		 */
		Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged);

	public:

		/**
		 * Default constructor.
		 *
		 * @param p_parent the parent QObject.
		 */
		explicit ResourceSortFilterProxyModel(QObject* p_parent);

		/**
		 * Destructor.
		 */
		~ResourceSortFilterProxyModel() override = default;

		/**
		 * Returns the search filter string.
		 */
		QString getNameSearchFilter() const { return m_nameSearchFilter; }

		/**
		 * Sets the search filter string.
		 *
		 * @param p_nameSearchFilter The search filter string.
		 */
		void setNameSearchFilter(const QString& p_nameSearchFilter);

		/**
		 * Returns the tag filter list.
		 * 
		 * @return The tag filter list.
		 */
		QStringList getTagFilterList() const { return m_tagFilterList; }

		/**
		 * Sets the tag filter list.
		 *
		 * @param p_tagFilterList The tag filter list.
		 */
		void setTagFilterList(const QStringList& p_tagFilterList);

		/**
		 * Sets the sort order.
		 *
		 * @param p_sortOrder The sort order.
		 */
		void setSortOrder(Qt::SortOrder p_sortOrder);

	signals:
		
		/**
		 * Signal emitted when the name search filter string is changed.
		 * 
		 * @param p_newNameSearchFilter The new name search filter string.
		 */
		void nameSearchFilterChanged(const QString& p_newNameSearchFilter);

		/**
		 * Signal emitted when the tag filter list is changed.
		 *
		 * @param p_newTagFilterList The new tag filter list.
		 */
		void tagFilterListChanged(const QStringList& p_newTagFilterList);

		/**
		 * Signal emitted when the sort order is changed.
		 * 
		 * @param p_newSortOrder The new sort order.
		 */
		void sortOrderChanged(Qt::SortOrder p_newSortOrder);

	protected:

		/**
		 * Reimplemented from QSortFilterProxyModel to filter the resources list model based on the name search filter string.
		 *
		 * @param p_sourceRow The source row index.
		 * @param p_sourceParent The source parent index.
		 */
		bool filterAcceptsRow(int p_sourceRow, const QModelIndex& p_sourceParent) const override;

		/**
		 * Reimplemented from QSortFilterProxyModel to sort the resources list model based on the sort order.
		 *
		 * @param p_left The left index to compare.
		 * @param p_right The right index to compare.
		 */
		bool lessThan(const QModelIndex& p_left, const QModelIndex& p_right) const override;

	private:

		/**
		 * The name search filter string, it is used to filter the resources list model based on the name of the resources.
		 */
		QString m_nameSearchFilter{};

		/**
		 * The tag filter list, it is used to filter the resources list model based on the tags of the resources.
		 */
		QStringList m_tagFilterList{};

	};
}

#endif // RESOURCESORTFILTERPROXYMODEL_H
#ifndef RESOURCEREGISTRY_H
#define RESOURCEREGISTRY_H

// Qt
#include <QAbstractListModel>
#include <QString>
#include <QUrl>
#include <QJsonArray>
#include <QJsonObject>

// STL
#include <utility>

namespace Asterindes
{
	/**
	 * ResourceRegistry class is responsible for managing the resources of the project and organizing them in project folders
	 */
	class ResourceRegistry : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY(ResourceRegistry);

	public:

		/**
		 * Resource struct represents a resource in the project, name and url of the resource
		 */
		struct Resource
		{
			/**
			 * The name of the resource, it is used for display purposes and can be different from the file name.
			 */
			QString m_name;

			/**
			 * The URL of the resource, it is used to access the resource file and should be unique among the project resources.
			 */
			QUrl m_resourceUrl;

			/**
			 * The creation date of the resource, it is used to sort the resources by creation date.
			 */
			QDateTime m_creationDate;

			/**
			 * The list of tags associated with the resource, it is used to categorize the resources and can be used for filtering.
			 */
			QSet<QString> m_tags;

			// NOTE: This struct does not manage its construction. The ResourceRegistry class is responsible for converting it from and to JSON so it can manage errors.
		};

		/**
		 * The list of resources managed by this class.
		 */
		using ResourceList = QHash<QString, Resource>;

		/**
		 * Default constructor.
		 */
		using QObject::QObject;

		/**
		 * Destructor.
		 */
		~ResourceRegistry() override = default;

		/**
		 * Loads the resources from a JSON array and adds them to the project. Removes all the existing resources before loading the new ones.
		 *
		 * @param p_resourceJsonArray The JSON array containing the resources to load.
		 * 
		 * @return true if the resources were loaded successfully, false otherwise.
		 */
		bool loadResourcesFromJson(const QJsonArray& p_resourceJsonArray);

		/**
		 * Gets the resources of the project as a JSON array.
		 *
		 * @return a JSON array containing the resources of the project.
		 */
		QJsonArray getResourcesAsJson() const;

		/**
		 * Gets all the resources managed by this class.
		 * 
		 * @return the resource list
		 */
		QList<Resource> getResourcesList() const;

		/**
		 * Gets a resource by its URL.
		 *
		 * @param p_resourceUrl The URL of the resource to get.
		 * 
		 * @return a pair of a bool indicating if the resource was found, and the resource with the given URL, or a default-constructed Resource if not found.
		 */
		std::pair<bool, Resource> getResourceByUrl(const QUrl& p_resourceUrl) const;

		/**
		 * Checks if a resource with the given URL already exists in the project.
		 *
		 * @param p_resourceUrl The URL of the resource to check.
		 * 
		 * @return true if the resource exists, false otherwise.
		 */
		inline bool containsResource(const QUrl& p_resourceUrl) const { return m_resources.contains(p_resourceUrl.toString()); }

		/**
		 * Adds new resources to the project from the given file path list.
		 *
		 * @param p_resourceUrls The list of URLs of the resource files to add.
		 * 
		 * @return the list of the url of the added resources.
		 */
		QList<QUrl> addResources(const QList<QUrl>& p_resourceUrls);

		/**
		 * Removes resources from the project by their URLs.
		 *
		 * @param p_resourceUrls The list of URLs of the resources to remove.
		 *
		 * @return the list of the urls of the removed resources.
		 */
		QList<QUrl> removeResources(const QList<QUrl>& p_resourceUrls);

		/**
		 * Rename a resource from the project from its URL.
		 *
		 * @param p_resourceUrl The URL of the resource to rename.
		 * @param p_newName The new name for the resource.
		 * 
		 * @return true if the resource was renamed successfully, false otherwise.
		 */
		bool renameResource(const QUrl& p_resourceUrl, const QString& p_newName);

		/**
		 * Adds a tag to a resource from the project from its URL.
		 *
		 * @param p_resourceUrl The URL of the resource to add the tag to.
		 * @param p_tag The tag to add to the resource.
		 *
		 * @return true if the tag was added successfully, false otherwise.
		 */
		bool addTagToResource(const QUrl& p_resourceUrl, const QString& p_tag);

		/**
		 * Removes a tag from a resource from the project from its URL.
		 *
		 * @param p_resourceUrl The URL of the resource to remove the tag from.
		 * @param p_tag The tag to remove from the resource.
		 *
		 * @return true if the tag was removed successfully, false otherwise.
		 */
		bool removeTagFromResource(const QUrl& p_resourceUrl, const QString& p_tag);

	signals:

		/**
		 * Signal called when someting in the resources of this manager is changed.
		 */
		void resourcesChanged();

		/**
		 * Signal called when the resource list of this manager is changed.
		 */
		void resourceListChanged();

		/**
		 * Signal called when a resource's tag is changed in the project.
		 * 
		 * @param p_resourceUrl The URL of the resource whose tag was changed.
		 * @param p_tag The tag that was added or removed.
		 * @param p_added true if the tag was added, false if it was removed.
		 */
		void resourceTagChanged(const QUrl& p_resourceUrl, const QString& p_tag, bool p_added);

	private:

		/**
		 * All the resources of the project
		 * unique_ptr to allow access to the resources by reference.
		 */
		ResourceList m_resources{};
	};
}



#endif // RESOURCEREGISTRY_H

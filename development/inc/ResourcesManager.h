#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

// Qt
#include <QAbstractListModel>
#include <QString>
#include <QUrl>

// STL
#include <utility>

namespace Asterindes
{
	/**
	 * ResourcesManager class is responsible for managing the resources of the project and organizing them in project folders
	 */
	class ResourcesManager : public QObject
	{
		Q_OBJECT;

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
		};

		/**
		 * The list of resources managed by this class, it stores unique_ptr to allow keeping the resources by reference.
		 */
		using ResourceList = std::unordered_map<QString, Resource>;

		/**
		 * Default constructor.
		 */
		using QObject::QObject;

		/**
		 * Destructor.
		 */
		~ResourcesManager() override = default;

		/**
		 * Gets all the resources managed by this class.
		 * 
		 * @return the resource list
		 */
		QList<Resource> getResourcesList() const;

		void testCreateResource();

		/**
		 * Checks if a resource with the given URL already exists in the project.
		 *
		 * @param p_resourceUrl The URL of the resource to check.
		 * @return true if the resource exists, false otherwise.
		 */
		inline bool containsResource(const QUrl& p_resourceUrl) const { return m_resources.contains(p_resourceUrl.toString()); }

		/**
		 * Adds a new resource to the project from the given file path.
		 *
		 * @param p_resourceUrl The URL of the resource file to add.
		 */
		bool addResource(const QUrl& p_resourceUrl);

	signals:

		/**
		 * Signal called when the resources of this manager are updated.
		 */
		void resourcesUpdated();

	private:

		/**
		 * All the resources of the project
		 * unique_ptr to allow access to the resources by reference.
		 */
		ResourceList m_resources{};
	};
}



#endif // RESOURCESMANAGER_H

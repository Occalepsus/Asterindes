#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

// Qt
#include <QAbstractListModel>
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
		 * Resource struct represents a resource in the project, it contains the id, name and url of the resource
		 */
		struct Resource
		{
			size_t m_id;
			QString m_name;
			QUrl m_resourceUrl;
		};

		/**
		 * The list of resources managed by this class, it stores unique_ptr to allow keeping the resources by reference.
		 */
		using ResourceList = std::unordered_set<std::unique_ptr<Resource>>;

		/**
		 * Default constructor.
		 */
		using QObject::QObject;

		/**
		 * Destructor.
		 */
		~ResourcesManager() override = default;

		/**
		 * Gets the resource list managed by this class. This reference is valid as long as the ResourcesManager instance is alive and should not be stored.
		 * 
		 * @return the resource list
		 */
		inline const ResourceList& getResourcesList() const { return m_resources; };

		void testCreateResource();

	public slots:

		/**
		 * Adds a new resource to the project from the given file path.
		 *
		 * @param p_resourceUrl The URL of the resource file to add.
		 */
		void addResource(const QUrl& p_resourceUrl);

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
		ResourceList m_resources;
	};
}



#endif // RESOURCESMANAGER_H

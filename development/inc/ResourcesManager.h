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
		 * Resource struct represents a resource in the project, it contains the id, name and url of the resource
		 */
		struct Resource
		{
			QString m_name;
			QUrl m_resourceUrl;

			/**
			 * Default constructor.
			 * 
			 * @param p_id The id of the resource, it is used to identify the resource in the project and to manage it in the project folders.
			 * @param p_name The name of the resource, it is used to display the resource in the UI and to manage it in the project folders.
			 */
			//Resource(QString p_name, QUrl p_resourceUrl)
			//	: m_name(std::move(p_name)), m_resourceUrl(std::move(p_resourceUrl)) {};
		};

		struct ResourcePtrHash {
			size_t operator()(const std::unique_ptr<Resource>& p_res) const {
				//return std::hash<Resource*>{}(p_res.get());
				return std::hash<QString>{}(p_res->m_resourceUrl.toString());
			}
		};

		struct ResourcePtrEqual {
			bool operator()(const std::unique_ptr<Resource>& lA,
				const std::unique_ptr<Resource>& lB) const {
				//return lA.get() == lB.get();
				return lA->m_resourceUrl.toString() == lB->m_resourceUrl.toString();
			}
		};

		/**
		 * The list of resources managed by this class, it stores unique_ptr to allow keeping the resources by reference.
		 */
		using ResourceList = std::unordered_set<std::unique_ptr<Resource>, ResourcePtrHash, ResourcePtrEqual>;

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

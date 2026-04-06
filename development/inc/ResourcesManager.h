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
	 * Resource struct represents a resource in the project, it contains the id, name and url of the resource
	 */
	struct Resource
	{
		size_t m_id;
		QString m_name;
		QUrl m_resourceUrl;
	};

	class ResourcesManager;

	namespace Ui
	{
	/**
	 * ResourcesListModel class is a model that represents the current resources list model, it is used to display the resources as a list in the UI.
	 */
	class ResourcesListModel : public QAbstractListModel
	{
		friend class ResourcesManager;

	public:
		/**
		 * The enum that contains the roles for the resources list model, it is used to map the data in the model to the UI elements in the QML file.
		 */
		enum class ResourceRoles
		{
			NameRole = Qt::UserRole + 1,
			ResourceUrlRole
		};

		/**
		 * Default constructor.
		 */
		using QAbstractListModel::QAbstractListModel;

		/**
		 * Destructor.
		 */
		~ResourcesListModel() override = default;

		/**
		 * Returns the model's role names.
		 */
		QHash<int, QByteArray> roleNames() const override;

		/**
		 * Returns the number of rows in the model.
		 *
		 * @param parent The parent index, it is ignored in this model since it is a list model.
		 */
		int rowCount(const QModelIndex& p_parent = QModelIndex()) const override;

		/**
		 * Returns the data stored under the given role for the item referred to by the index.
		 *
		 * @param index The index of the item in the model.
		 * @param role The role for which the data is requested.
		 */
		QVariant data(const QModelIndex& p_index, int p_role = Qt::DisplayRole) const override;

	private:

		/**
		 * The hash that contains the role names for the model, it is used to map the roles to the data in the model.
		 */
		const QHash<int, QByteArray> m_roleNames
		{
			{ std::to_underlying(ResourceRoles::NameRole), "name" },
			{ std::to_underlying(ResourceRoles::ResourceUrlRole), "resourceUrl" }
		};

		/**
		 * The vector that contains the resources to be displayed in the UI, it is filled by the ResourcesManager class.
		 */
		std::vector<const Resource*> m_displayedResources;

		/**
		 * Updates the displayed resources in the model, it is called by the ResourcesManager class when the resources list needs to be updated.
		 */
		void updateDisplayedResources(const std::vector<const Resource*>& p_resourcesToDisplay);
	};
	}

	/**
	 * ResourcesManager class is responsible for managing the resources of the project and organizing them in project folders
	 */
	class ResourcesManager : public QObject
	{
		Q_OBJECT;

	public:

		/**
		 * Default constructor.
		 */
		using QObject::QObject;

		/**
		 * Destructor.
		 */
		~ResourcesManager() override = default;

		/**
		 * Gets the current resources list model. This reference is valid as long as the ResourcesManager instance is alive.
		 */
		inline Ui::ResourcesListModel* getDisplayedResourcesListModel() { return &m_displayedResourcesListModel; };

		void testCreateResource();

	public slots:

		/**
		 * Adds a new resource to the project from the given file path.
		 *
		 * @param p_resourceUrl The URL of the resource file to add.
		 */
		void addResource(const QUrl& p_resourceUrl);

	private:
		/**
		 * All the resources of the project
		 * unique_ptr to allow access to the resources by reference.
		 */
		std::unordered_set<std::unique_ptr<Resource>> m_resources;

		/**
		 * A model that represents the current resources list, it is used to display the resources in the UI and to manage them in the project folders.
		 */
		Ui::ResourcesListModel m_displayedResourcesListModel{};
	};
}



#endif // RESOURCESMANAGER_H

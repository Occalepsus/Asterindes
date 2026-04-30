// Asterindes
#include "ResourcesManager.h"

// Qt
#include <QAbstractListModel>
#include <QQuickWindow>

namespace Asterindes::Ui
{
	/**
	 * ResourceListModel class is a model that represents the current resources list model, it is used to display the resources as a list in the UI.
	 */
	class ResourceListModel : public QAbstractListModel
	{
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
		 * 
		 * @param p_parent the parent QObject.
		 */
		explicit ResourceListModel(QObject* p_parent);

		/**
		 * Destructor.
		 */
		~ResourceListModel() override = default;

		/**
		 * Returns the model's role names.
		 */
		inline QHash<int, QByteArray> roleNames() const override { return m_roleNames; };

		/**
		 * Returns the number of rows in the model.
		 *
		 * @param parent The parent index, it is ignored in this model since it is a list model.
		 */
		inline int rowCount(const QModelIndex& p_parent = QModelIndex()) const override { return static_cast<int>(m_displayedResources.size()); }

		/**
		 * Returns the data stored under the given role for the item referred to by the index.
		 *
		 * @param index The index of the item in the model.
		 * @param role The role for which the data is requested.
		 */
		QVariant data(const QModelIndex& p_index, int p_role = Qt::DisplayRole) const override;

		/**
		 * Updates the model with the given resources list, it is called by the ResourcesViewModel class when the resources list is updated in the ResourcesManager class.
		 * 
		 * @param p_resourceList The list of resources to update the model with.
		 */
		void updateFromResourcesList(const QList<ResourcesManager::Resource>& p_resourceList);

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
		 * The list that contains the resources to be displayed in the UI.
		 */
		QList<ResourcesManager::Resource> m_displayedResources{};
	};
}

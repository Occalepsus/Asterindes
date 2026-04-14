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
		 * @param p_resourcesManager the resource manager to display
		 */
		ResourceListModel(QObject* p_parent, const ResourcesManager& p_resourcesManager);

		/**
		 * Destructor.
		 */
		~ResourceListModel() override = default;

		/**
		 * Sets up the model for the QML view, it is called by the GuiClient class when the QML file is loaded.
		 *
		 * @param p_window The QQuickWindow instance of the QML view to set up the model for.
		 */
		void setupQmlModel(const QQuickWindow* p_window);

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
		 * The resources manager to display in the model.
		 */
		const ResourcesManager& m_resourcesManager;

		/**
		 * The vector that contains the resources to be displayed in the UI, it is filled by the ResourcesManager class.
		 */
		std::vector<const ResourcesManager::Resource*> m_displayedResources{};

		/**
		 * Updates the displayed resources in the model, it is called by the ResourcesManager class when the resources list needs to be updated.
		 */
		void updateResourceListModel();
	};
}

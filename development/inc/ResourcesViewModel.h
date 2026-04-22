#ifndef RESOURCESVIEWMODEL_H
#define RESOURCESVIEWMODEL_H

// Asterindes
#include "ResourcesManager.h"
#include "ResourceListModel.h"

// Qt
#include <QObject>
#include <QUrl>

namespace Asterindes::Ui
{
	/**
	 * ResourcesViewModel class is responsible for exposing the resources data and operations to the QML UI. 
	 * It acts as a bridge between the ResourcesManager and the QML views, providing properties, methods, and signals for UI interaction.
	 */
	class ResourcesViewModel : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY_MOVE(ResourcesViewModel);

		/**
		 * The list model for GridView/ListView binding.
		 */
		Q_PROPERTY(ResourceListModel* resourcesModel READ getResourcesModel NOTIFY resourceListChanged)
		
		/**
		 * Number of resources (for display).
		 */
		Q_PROPERTY(int resourceCount READ getResourceCount NOTIFY resourceListChanged)
		
		/**
		 * Whether resources are currently being loaded (for loading indicators).
		 */
		Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)

	public:
		/**
		 * Constructor.
		 * 
		 * @param p_resourcesManager Reference to the business logic manager.
		 * @param p_parent Parent QObject.
		 */
		explicit ResourcesViewModel(ResourcesManager& p_resourcesManager, QObject* p_parent = nullptr);

		/**
		 * Destructor.
		 */
		~ResourcesViewModel() override = default;

		/**
		 * Get the displayed resource list model.
		 * 
		 * @return Pointer to the ResourceListModel.
		 */
		inline ResourceListModel* getResourcesModel() { return &m_resourcesListModel; }

		/**
		 * Get the count of resources.
		 * 
		 * @return The number of resources.
		 */
		inline int getResourceCount() const { return m_resourcesListModel.rowCount(); }

		/**
		 * Get the loading state.
		 * 
		 * @return true if loading, false otherwise.
		 */
		inline bool isLoading() const { return m_isLoading; }

		/**
		 * Adds a resource, use the name of the file.
		 * 
		 * @param p_resourceUrl The resource URL.
		 * @return true if successful, false otherwise.
		 */
		Q_INVOKABLE bool addResource(const QUrl& p_resourceUrl);

		/**
		 * Removes a resource by its URL.
		 * 
		 * @param p_resourceUrl The resource URL.
		 * @return true if successful, false otherwise.
		 */
		Q_INVOKABLE bool removeResource(const QUrl& p_resourceUrl);

		/**
		 * Checks if a resource exists using its URL.
		 * 
		 * @param p_url The URL to check.
		 * @return true if the resource exists, false otherwise.
		 */
		Q_INVOKABLE bool canAddResource(const QUrl& p_url) const;

	signals:
		/**
		 * Emitted when the resource list changes.
		 */
		void resourceListChanged();

		/**
		 * Emitted when loading state changes.
		 */
		void isLoadingChanged();

		/**
		 * Emitted when an error occurs (for QML error display).
		 */
		void errorOccurred(const QString& p_errorMessage);

	private:
		/**
		 * Reference to the business logic manager.
		 */
		ResourcesManager& m_resourcesManager;

		/**
		 * The presentation model for QML.
		 */
		ResourceListModel m_resourcesListModel;

		/**
		 * Loading state.
		 */
		bool m_isLoading{ false };

		/**
		 * Sets loading state and emits signal.
		 */
		void setLoading(bool p_loading);

	private slots:
		/**
		 * Handles ResourcesManager's resourcesChanged signal.
		 */
		void onResourcesUpdated();
	};
}

#endif // RESOURCESVIEWMODEL_H
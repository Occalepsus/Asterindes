#ifndef PROJECTVIEWMODEL_H
#define PROJECTVIEWMODEL_H

// Asterindes
#include "AsterindesProject.h"

// Qt
#include <QObject>
#include <QUrl>

namespace Asterindes::Ui
{
	/**
	 * ProjectViewModel class is responsible for exposing the project data and operations to the QML UI.
	 * It acts as a bridge between the AsterindesProject and the QML views, providing properties, methods, and signals for UI interaction.
	 */
	class ProjectViewModel : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY(ProjectViewModel);

		/**
		 * The URL of the currently opened project, empty if no project is opened. This is used to display the project path in the UI and to check if a project is currently opened.
		 */
		Q_PROPERTY(QUrl currentProjectPath READ getCurrentProjectPath NOTIFY currentProjectPathChanged);

		/**
		 * The url of the resource being broadcasted, empty means nothing is being broadcasted.
		 */
		Q_PROPERTY(QUrl broadcastedResourceUrl READ getBroadcastedResourceUrl WRITE setBroadcastedResourceUrl NOTIFY broadcastedResourceChanged);

	public:
		/**
		 * Constructor.
		 *
		 * @param p_projectManager Reference to the business logic manager.
		 * @param p_parent Parent QObject.
		 */
		explicit ProjectViewModel(QPointer<AsterindesProject> p_projectManager, QObject* p_parent = nullptr);

		/**
		 * Destructor.
		 */
		~ProjectViewModel() override = default;

		/**
		 * Loads a project from the given path, it is used to load the project when the user opens a project from the UI.
		 */
		void loadProject(const QUrl& p_projectPath);

		/**
		 * Gets the URL of the currently opened project, empty if no project is opened. This is used to display the project path in the UI and to check if a project is currently opened.
		 *
		 * @return The URL of the currently opened project.
		 */
		inline QUrl getCurrentProjectPath() const { return m_projectPath; };

		/**
		 * Get the URL of the resource being broadcasted.
		 *
		 * @return The URL of the resource being broadcasted, empty if nothing is being broadcasted.
		 */
		inline QUrl getBroadcastedResourceUrl() const { return m_resourceBroadcastManager ? m_resourceBroadcastManager->getBroadcastedResource().m_resourceUrl : QUrl(); }

		/**
		 * Set the resource being broadcasted using its URL.
		 *
		 * @param p_url The URL to set as being broadcasted, empty to indicate nothing is being broadcasted.
		 */
		Q_INVOKABLE void setBroadcastedResourceUrl(const QUrl& p_url);

	signals:
		/**
		 * Signal emitted when the current project path changes.
		 */
		void currentProjectPathChanged();

		/**
		 * Signal emitted when the broadcasted resource changes.
		 */
		void broadcastedResourceChanged();

	private:
		/**
		 * Reference to the business logic manager.
		 */
		QPointer<AsterindesProject> m_projectManager;

		/**
		 * The resource broadcast manager of the project
		 */
		QPointer<ResourceBroadcastManager> m_resourceBroadcastManager;

		/**
		 * The URL of the currently opened project, empty if no project is opened. This is used to display the project path in the UI and to check if a project is currently opened.
		 */
		QUrl m_projectPath{};

	private slots:
		/**
		 * Slot called when a project is loaded. This will update the current project path and emit the currentProjectPathChanged signal to update the UI.
		 */
		void onProjectLoaded(const QUrl& p_projectPath);
	};
}

#endif // PROJECTVIEWMODEL_H
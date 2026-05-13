#ifndef ASTERINDESPROJECT_H
#define ASTERINDESPROJECT_H

// Asterindes
#include "ResourceRegistry.h"
#include "ResourceBroadcastManager.h"

// Qt
#include <QObject>
#include <QVariant>
#include <QQmlEngine>

namespace Asterindes
{
	class AsterindesCore;

	class AsterindesProject : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY(AsterindesProject);

	public:
		/**
		 * Default constructor.
		 *
		 * @param p_projectPath The URL of the project file to load, it should be a local file URL pointing to a valid project file.
		 * @param p_parent The parent QObject, it is used for memory management and should be the AsterindesCore instance.
		 */
		AsterindesProject(const QUrl& p_projectPath, AsterindesCore* p_parent);

		/**
		 * Destructor.
		 */
		~AsterindesProject() override;

		/**
		 * Gets the URL of the project file.
		 *
		 * @return The URL of the project file.
		 */
		inline QUrl getProjectPath() const { return m_projectPath; }

		/**
		 * Gets the resources registry of the project, it keeps track of all the resources in the project and provides methods to manage them.
		 * This reference is valid as long as the AsterindesProject instance is alive.
		 */
		inline ResourceRegistry* getResourcesRegistry() { return m_resourcesRegistry; }

		/**
		 * Loads the project from its path, it reads the project file and loads the data into the project managers.
		 * This method should only be called once
		 *
		 * @return true if the project was loaded successfully, false otherwise.
		 */
		bool loadProject();

		/**
		 * Saves the project to its path.
		 *
		 * @return true if the project was saved successfully, false otherwise.
		 */
		bool saveProject() const;

	signals:
		/**
		 * Signal emitted when the project loading is done, it can be used to trigger UI updates when a new project is loaded.
		 *
		 * @param p_projectPath The URL of the loaded project, it should be a local file URL pointing to a valid project file.
		 */
		void projectLoaded(const QUrl& p_projectPath);

		/**
		 * Emitted when the project window requests to close (e.g. user clicked X).
		 * The Core will handle the actual closure.
		 *
		 * @param p_project The project that is requesting to be closed.
		 */
		void projectCloseRequested(AsterindesProject* p_project);

	private:
		/**
		 * Indicates whether the project is loaded or not, it is used to prevent multiple loading of the same project and to manage the project state.
		 */
		bool m_isLoaded{ false };

		/**
		 * The URL of this project.
		 */
		QUrl m_projectPath{};

		/**
		 * The resources registry of the project, it keeps track of all the resources in the project and provides methods to manage them.
		 */
		ResourceRegistry* m_resourcesRegistry{ new ResourceRegistry(this) };

		/**
		 * The resource broadcast manager of the project, it is responsible for managing the resources and broadcasting them to the clients via the BroadcastServer.
		 */
		ResourceBroadcastManager* m_resourceBroadcastManager{ new ResourceBroadcastManager(this) };
	};
}

#endif // ASTERINDESPROJECT_H
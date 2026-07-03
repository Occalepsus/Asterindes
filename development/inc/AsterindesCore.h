#ifndef ASTERINDESCORE_H
#define ASTERINDESCORE_H

// Asterindes
#include "ProjectManagerService.h"
#include "AsterindesProject.h"

// Asterindes UI
#include "ProjectWindow.h"
#include "StartupWindow.h"

// Qt
#include <QGuiApplication>

namespace Asterindes
{
	/**
	 * AsterindesCore is the main class of the Asterindes application.
	 * It is responsible for initializing the application, managing global resources, and providing access to core functionalities.
	 */
	class AsterindesCore : public QGuiApplication
	{
	public:

		/**
		 * Default constructor.
		 * 
		 * @param argc The number of command-line arguments.
		 * @param argv The array of command-line arguments.
		 */
		AsterindesCore(int& argc, char** argv);

		/**
		 * Destructor.
		 */
		~AsterindesCore() final = default;

		/**
		 * Opens a project from the given file path, if the project is already open it will just focus the project window.
		 * Creates a new file if if does not exist, and loads the project data into the project managers.
		 * 
		 * @param p_projectPath The path of the project to open, it should be a local file path pointing to a valid project file.
		 * 
		 * @return true if the project was loaded successfully, false otherwise.
		 */
		bool openProject(const QUrl& p_projectPath);

	public slots:
		/**
		 * Slot called by a project when it requests to be closed, it will destroy the project and exit the application if there are no more projects open.
		 */
		void onProjectCloseResquested(AsterindesProject* p_project);

	private:

		/**
		 * The ProjectManagerService instance used to manage the projects of the application.
		 * It is responsible for creating, loading, and managing recent projects.
		 */
		ProjectManagerService* m_projectManagerService{ new ProjectManagerService(this) };

		/**
		 * The map of project manager instances, each responsible for managing their project and its data. We ensure each project is only loaded once, so we can use the project file URL as the key to identify each project manager instance.
		 * The key is the URL of the project file, and the value is a pointer to the corresponding AsterindesProject instance.
		 */
		// TODO: move to ProjectManagerService
		QHash<QUrl, QPointer<AsterindesProject>> m_openedProjects;

		/**
		 * The startup window instance, used to display the initial UI when no projects are open. It is created when the application starts and destroyed when the first project is opened.
		 */
		Ui::StartupWindow* m_startupWindow{ new Ui::StartupWindow(m_projectManagerService, this) };

		/**
		 * The map of project window instances, each responsible for managing the UI and communicating with the other classes of the project. This mirrors the m_openedProjects map, ensuring each project has a corresponding window instance.
		 * The key is the URL of the project file, and the value is a pointer to the corresponding ProjectWindow instance.
		 */
		QHash<QUrl, QPointer<Ui::ProjectWindow>> m_openedProjectWindows;

		/**
		 * Saves the project location to the application settings.
		 * 
		 * @param pProjectPath The path of the project to save, it should be a local file path pointing to a valid project file.
		 */
		void saveProjectLocation(const QString& pProjectPath) const;

		/**
		 * Loads the project location from the application settings.
		 * 
		 * @return The path of the last opened project, it should be a local file path pointing to a valid project file.
		 */
		QString getLastProjectLocation() const;
	};
}


#endif // !ASTERINDESCORE_H

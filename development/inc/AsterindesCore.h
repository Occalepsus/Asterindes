#ifndef ASTERINDESCORE_H
#define ASTERINDESCORE_H

// Asterindes
#include "SingleInstanceGuard.h"
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
		Q_OBJECT;

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
		 * Starts the application, it will check if another instance is running or start the application normally.
		 * 
		 * @return true if the application started successfully, false otherwise (if another instance is already running).
		 */
		bool start();

		/**
		 * Opens and shows the startup window.
		 */
		inline void showStartupWindow() { m_startupWindow->showStartupWindow(); }

		/**
		 * Opens a project from the given file path, if the project is already open it will just focus the project window.
		 * Creates a new file if if does not exist, and loads the project data into the project managers.
		 * 
		 * @param p_projectPath The path of the project to open, it should be a local file path pointing to a valid project file.
		 * 
		 * @return true if the project was loaded successfully, false otherwise.
		 */
		bool openProject(const QUrl& p_projectPath);

	private:

		/**
		 * The path of the project to open on startup, it is set when the application is started with a project path argument.
		 * If empty, the application will open the startup window instead of a project window.
		 */
		QString m_startupProject{ "" };

		/**
		 * The SingleInstanceGuard instance used to ensure that only a single instance of the application is running.
		 * It is responsible for checking if another instance is running and sending the project path to it if so.
		 */
		SingleInstanceGuard* m_singleInstanceGuard{ new SingleInstanceGuard(this, this) };

		/**
		 * The ProjectManagerService instance used to manage the projects of the application.
		 * It is responsible for creating, loading, and managing recent projects.
		 */
		ProjectManagerService* m_projectManagerService{ new ProjectManagerService(this) };

		/**
		 * The startup window instance, used to display the initial UI when no projects are open. It is created when the application starts and destroyed when the first project is opened.
		 */
		Ui::StartupWindow* m_startupWindow{ new Ui::StartupWindow(m_projectManagerService, this) };

		/**
		 * The map of project manager instances, each responsible for managing their project and its data. We ensure each project is only loaded once, so we can use the project file URL as the key to identify each project manager instance.
		 * The key is the URL of the project file, and the value is a pointer to the corresponding AsterindesProject instance.
		 */
		// TODO: move to ProjectManagerService
		QHash<QUrl, QPointer<AsterindesProject>> m_openedProjects;

		/**
		 * The map of project window instances, each responsible for managing the UI and communicating with the other classes of the project. This mirrors the m_openedProjects map, ensuring each project has a corresponding window instance.
		 * The key is the URL of the project file, and the value is a pointer to the corresponding ProjectWindow instance.
		 */
		QHash<QUrl, QPointer<Ui::ProjectWindow>> m_openedProjectWindows;

		/**
		 * Checks if the application should close, it will close the application if there are no more projects open and the startup window is not visible.
		 */
		void applicationShouldExit();

	private slots:

		/**
		 * Slot called by a project when it requests to be closed, it will destroy the project and exit the application if there are no more projects open.
		 */
		void onProjectCloseRequested(AsterindesProject* p_project);
	};
}


#endif // !ASTERINDESCORE_H

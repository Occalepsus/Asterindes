#ifndef ASTERINDESAPP_H
#define ASTERINDESAPP_H

// Asterindes
#include "ProjectManager.h"

// Qt
#include <QGuiApplication>

// STL
#include <memory>

namespace Asterindes
{
	/**
	 * AsterindesApp is the main singleton class of the Asterindes application. It is responsible for initializing the application, managing global resources, and providing access to core functionalities.
	 */
	class AsterindesApp : public QGuiApplication
	{
	public:

		/**
		 * Default constructor.
		 * 
		 * @param argc The number of command-line arguments.
		 * @param argv The array of command-line arguments.
		 */
		AsterindesApp(int& argc, char** argv);

		/**
		 * Destructor.
		 */
		~AsterindesApp() final = default;

		/**
		 * Gets the project manager instance, which is responsible for managing the current project and its data.
		 * 
		 * @return A reference to the instance of AsterindesApp.
		 */
		inline ProjectManager& getProjectManager() { return m_projectManager; }

	private:

		/**
		 * The project manager instance, responsible for managing the current project and its data.
		 */
		ProjectManager m_projectManager{ this };

	};
}


#endif // !ASTERINDESAPP_H

#ifndef ASTERINDESCORE_H
#define ASTERINDESCORE_H

// Asterindes
#include "ProjectManager.h"
#include "GuiClient.h"

// Qt
#include <QGuiApplication>

// STL
#include <memory>

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
		 * Gets the project manager instance, which is responsible for managing the current project and its data.
		 * 
		 * @return A reference to the instance of AsterindesCore.
		 */
		inline ProjectManager& getProjectManager() { return m_projectManager; }

	private:

		/**
		 * The project manager instance, responsible for managing the current project and its data.
		 */
		ProjectManager m_projectManager{ this };

		/**
		 * The GUI client instance, responsible for managing the UI and communicating with the other classes of the project.
		 */
		Ui::GuiClient m_guiClient{ *this };

	};
}


#endif // !ASTERINDESCORE_H

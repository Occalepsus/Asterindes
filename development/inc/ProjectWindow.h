#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

// Asterindes UI
#include "ProjectViewModel.h"
#include "ResourcesViewModel.h"
#include "QmlDynamicLoader.h"

// Qt
#include <QQmlApplicationEngine>

namespace Asterindes
{
	class AsterindesCore;
	class AsterindesProject;
}

namespace Asterindes::Ui
{
	/**
	 * ProjectWindow manages the UI layer of a specific project.
	 */
	class ProjectWindow : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY_MOVE(ProjectWindow);

		/**
		 * Exposes the project ViewModel to QML for data binding and UI interaction.
		 */
		Q_PROPERTY(ProjectViewModel* projectViewModel READ projectViewModel CONSTANT)

		/**
		 * Exposes the resources ViewModel to QML for data binding and UI interaction.
		 */
		Q_PROPERTY(ResourcesViewModel* resourcesViewModel READ resourcesViewModel CONSTANT)

	public:
		/**
		 * Default constructor.
		 *
		 * @param p_project Reference to the project this window is associated with.
		 * @param p_coreApp Reference to the core application, used as the parent QObject.
		 */
		explicit ProjectWindow(AsterindesProject* p_project, AsterindesCore* p_coreApp);

		/**
		 * Destructor, it is responsible for cleaning up the QML engine and any resources used by the window.
		 */
		~ProjectWindow() override;

		/**
		 * Opens the project window, it should be called after the project is loaded and ready to be displayed. It will show the window and make it active.
		 */
		void openProjectWindow();

		/**
		 * Gets the project ViewModel, it is used to access the project data and operations from QML.
		 *
		 * @return Pointer to the ProjectViewModel.
		 */
		inline ProjectViewModel* projectViewModel() const { return m_projectViewModel; }

		/**
		 * Gets the resources ViewModel, it is used to access the resources data and operations from QML.
		 *
		 * @return Pointer to the ResourcesViewModel.
		 */
		inline ResourcesViewModel* resourcesViewModel() const { return m_resourcesViewModel; }

	private:

		/**
		 * Project ViewModel
		 */
		ProjectViewModel* m_projectViewModel;

		/**
		 * Resource ViewModel exposed to QML for data binding and UI interaction.
		 */
		ResourcesViewModel* m_resourcesViewModel;

		/**
		 * The QML engine for loading and managing QML components.
		 * This is not the child of the ProjectWindow since it needs to have deleteLater called on it to ensure proper cleanup.
		 */
		QQmlApplicationEngine* m_appQmlEngine{ new QQmlApplicationEngine() };

		/**
		 * The QmlDynamicLoader is responsible for loading QML components and handling hot reloading (only for debug builds).
		 * It is initialized with the QML engine and the ProjectWindow as its parent.
		 */
		QmlDynamicLoader* m_qmlDynamicLoader{ new QmlDynamicLoader(m_appQmlEngine, this) };
	};
}

#endif // PROJECTWINDOW_H
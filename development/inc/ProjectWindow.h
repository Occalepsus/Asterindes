#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

// Asterindes UI
#include "ProjectViewModel.h"
#include "ResourcesViewModel.h"

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
		 */
		QQmlApplicationEngine m_appQmlEngine{ this };

		/**
		 * Sets up the QML context by exposing necessary ViewModels and properties to QML.
		 */
		void setupQmlContext();

	private slots:
		void onQmlFileLoaded(QObject* p_qmlObject, const QUrl& p_url);
	};
}

#endif // PROJECTWINDOW_H
#ifndef GUICLIENT_H
#define GUICLIENT_H

// Asterindes
#include "ResourceListModel.h"

// Qt
#include <QQmlApplicationEngine>

namespace Asterindes
{
	class AsterindesCore;
}

namespace Asterindes::Ui
{
	/**
	 * GuiClient class is the main class of the UI, it is used to manage the UI and to communicate with the other classes of the project.
	 */
	class GuiClient : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY_MOVE(GuiClient);

	public:

		/**
		 * Default constructor.
		 * 
		 * @param p_parentCoreApp The parent AsterindesCore object.
		 */
		explicit GuiClient(AsterindesCore& p_parentCoreApp);

		/**
		 * Destructor.
		 */
		~GuiClient() override = default;

	private:

		/**
		 * The QML engine, it is used to load the QML files and to manage the UI.
		 */
		QQmlApplicationEngine m_appQmlEngine{};

		/**
		 * The resources list model, it is used to display the resources in the UI and to manage them in the project folders.
		 * It is filled from the resources manager of the project.
		 */
		ResourceListModel m_resourcesListModel;

	private slots:

		void onQmlFileLoaded(QObject* p_qmlObject, const QUrl& p_url);
	};
}

#endif // GUICLIENT_H
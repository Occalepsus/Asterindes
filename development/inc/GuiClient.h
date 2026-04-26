#ifndef GUICLIENT_H
#define GUICLIENT_H

// Asterindes
#include "ResourcesViewModel.h"

// Qt
#include <QQmlApplicationEngine>

namespace Asterindes
{
    class AsterindesCore;
}

namespace Asterindes::Ui
{
    /**
     * GuiClient manages the UI layer.
     */
    class GuiClient : public QObject
    {
        Q_OBJECT;
        Q_DISABLE_COPY_MOVE(GuiClient);

    public:
        explicit GuiClient(AsterindesCore& p_parentCoreApp);
        ~GuiClient() override;

    private:
        /**
		 * The QML engine for loading and managing QML components.
         */
        QQmlApplicationEngine m_appQmlEngine{ this };

        /**
		 * Resource ViewModel exposed to QML for data binding and UI interaction.
         */
        ResourcesViewModel m_resourcesViewModel;

		/**
		 * Sets up the QML context by exposing necessary ViewModels and properties to QML.
         */
        void setupQmlContext();

    private slots:
        void onQmlFileLoaded(QObject* p_qmlObject, const QUrl& p_url);
    };
}

#endif // GUICLIENT_H
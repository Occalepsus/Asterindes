#ifndef GUICLIENT_H
#define GUICLIENT_H

// Asterindes
#include "ResourcesViewModel.h"

// Qt
#include <QQmlApplicationEngine>
#include <memory>

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
        ~GuiClient() override = default;

    private:
        QQmlApplicationEngine m_appQmlEngine{};
        
        /**
         * ViewModel for resources (exposed to QML).
         */
        ResourcesViewModel m_resourcesViewModel;

        void setupQmlContext();

    private slots:
        void onQmlFileLoaded(QObject* p_qmlObject, const QUrl& p_url);
    };
}

#endif // GUICLIENT_H
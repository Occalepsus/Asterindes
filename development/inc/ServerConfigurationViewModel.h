#ifndef SERVERCONFIGURATIONVIEWMODEL_H
#define SERVERCONFIGURATIONVIEWMODEL_H

// Asterindes
#include "BroadcastServer.h"

// Qt
#include <QObject>
#include <QPointer>

namespace Asterindes::Ui
{
	/**
	 * ServerConfigurationViewModel is the ViewModel for the server configuration window, it is responsible for exposing the project and resources ViewModels to QML for data binding and UI interaction.
	 */
	class ServerConfigurationViewModel : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY_MOVE(ServerConfigurationViewModel);

		/**
		 * The host address the Broadcast server will bind to. QHostAddress::Any for all interfaces.
		 */
		Q_PROPERTY(QString serverAddress READ getServerAddress WRITE setServerAddress NOTIFY serverAddressChanged);

		/**
		 * The port number the Broadcast server will listen on.
		 */
		Q_PROPERTY(quint16 serverPort READ getServerPort WRITE setServerPort NOTIFY serverPortChanged);

		/**
		 * The server state, "Running", "Stopped", or an error message.
		 */
		Q_PROPERTY(QString serverState READ getServerState NOTIFY serverStateChanged);

	public:

		/**
		 * Default constructor.
		 *
		 * @param p_broadcastServer The BroadcastServer to manage.
		 * @param p_parent The parent QObject, default is nullptr.
		 */
		explicit ServerConfigurationViewModel(BroadcastServer* p_broadcastServer, QObject* p_parent = nullptr);

		/**
		 * Destructor, it is responsible for cleaning up the QML engine and any resources used by the window.
		 */
		~ServerConfigurationViewModel() override;

		/**
		 * Gets the available IP addresses on the host machine.
		 *
		 * @return A QHash of available IP addresses, where the key is the interface name and the value is the IP address.
		 */
		Q_INVOKABLE QHash<QString, QVariant> getAvailableIpAddresses() const;

		/**
		 * Sets the host address the Broadcast server will bind to.
		 * Note: The server must be restarted for the changes to take effect.
		 *
		 * @param p_hostAddress The host address the Broadcast server will bind to.
		 */
		inline QString getServerAddress() const { return m_broadcastServer ? m_broadcastServer->getHostAddress().toString() : "Unknown"; }

		/**
		 * Sets the host address the Broadcast server will bind to.
		 * Note: The server must be restarted for the changes to take effect.
		 *
		 * @param p_hostAddress The host address the Broadcast server will bind to.
		 */
		inline void setServerAddress(const QString& p_hostAddress) { if (m_broadcastServer) m_broadcastServer->setHostAddress(QHostAddress(p_hostAddress)); }

		/**
		 * Sets the server port the Broadcast server will listen on.
		 * Note: The server must be restarted for the changes to take effect.
		 *
		 * @param p_serverPort The server port the Broadcast server will listen on.
		 */
		inline quint16 getServerPort() const { return m_broadcastServer ? m_broadcastServer->getServerPort() : 8080; }

		/**
		 * Sets the server port the Broadcast server will listen on.
		 * Note: The server must be restarted for the changes to take effect.
		 *
		 * @param p_serverPort The server port the Broadcast server will listen on.
		 */
		inline void setServerPort(quint16 p_serverPort) { if (m_broadcastServer) m_broadcastServer->setServerPort(p_serverPort); }

		/**
		 * Starts the Broadcast server, if it is not already running.
		 * After it has started, the serverStateChanged signal will be emitted with the new state of the server.
		 */
		Q_INVOKABLE void startServer() { if (m_broadcastServer) m_broadcastServer->start(); }

		/**
		 * Stops the Broadcast server, if it is running.
		 * After it has stopped, the serverStateChanged signal will be emitted with the new state of the server.
		 */
		Q_INVOKABLE void stopServer() { if (m_broadcastServer) m_broadcastServer->stop(); }

		/**
		 * Gets the last error string from the Broadcast server.
		 *
		 * @return The last error string from the Broadcast server.
		 */
		Q_INVOKABLE QString getLastErrorString() const { return m_broadcastServer ? m_broadcastServer->getLastErrorString() : QString(); }

		/**
		 * Gets the server state, "Running", "Stopped", or an error message.
		 *
		 * @return The server state, "Running", "Stopped", or an error message.
		 */
		QString getServerState() const;

	signals:

		/**
		 * Signal emitted when the server host address has changed.
		 */
		void serverAddressChanged(const QHostAddress& p_hostAddress);

		/**
		 * Signal emitted when the server port has changed.
		 */
		void serverPortChanged(quint16 p_serverPort);

		/**
		 * Signal emitted when the server state has changed.
		 * 
		 * @param p_serverState The server state, "Running", "Stopped", or an error message.
		 */
		void serverStateChanged(const QString& p_serverState);

	private:

		/**
		 * The BroadcastServer to manage
		 */
		QPointer<BroadcastServer> m_broadcastServer{ nullptr };
	};
}

#endif // SERVERCONFIGURATIONVIEWMODEL_H
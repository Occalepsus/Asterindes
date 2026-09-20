#ifndef BROADCASTSERVER_H
#define BROADCASTSERVER_H

// Qt
#include <QTcpServer>
#include <QHttpServer>
#include <QList>
#include <QWebSocket>

namespace Asterindes
{
	/**
	 * The BroadcastServer class is responsible for handling the HTTP server and sending resources to the clients via websockets.
	 */
	class BroadcastServer : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY(BroadcastServer);

	public:

		/**
		 * The state of the server.
		 */
		enum class ServerState : char
		{
			Stopped,
			Running,
			Error
		};

		/**
		 * Default constructor.
		 * 
		 * @param p_parent Parent QObject.
		 */
		explicit BroadcastServer(QObject* p_parent = nullptr);

		/**
		 * Destructor.
		 */
		~BroadcastServer() final;

		/**
		 * Gets the host address the HTTP and WebSocket servers will bind to.
		 *
		 * @return The host address the HTTP and WebSocket servers will bind to.
		 */
		QHostAddress getHostAddress() const { return m_hostAddress; }

		/**
		 * Sets the host address the HTTP and WebSocket servers will bind to.
		 * Note: The server must be restarted for the changes to take effect.
		 *
		 * @param p_hostAddress The host address the HTTP and WebSocket servers will bind to.
		 */
		void setHostAddress(const QHostAddress& p_hostAddress);

		/**
		 * Gets the port number the HTTP and WebSocket servers will listen on.
		 *
		 * @return The port number the HTTP and WebSocket servers will listen on.
		 */
		quint16 getServerPort() const { return m_serverPort; }

		/**
		 * Sets the port number the HTTP and WebSocket servers will listen on.
		 * Note: The server must be restarted for the changes to take effect.
		 *
		 * @param p_serverPort The port number the HTTP and WebSocket servers will listen on.
		 */
		void setServerPort(quint16 p_serverPort);

		/**
		 * Starts the HTTP and WebSocket servers, if they are not already running.
		 *
		 * @return true if the servers were started successfully, false otherwise.
		 */
		bool start();

		/**
		 * Stops the HTTP and WebSocket servers, if they are running.
		 */
		void stop();

		/**
		 * Gets the current state of the server.
		 *
		 * @return The current state of the server.
		 */
		ServerState getServerState() const { return m_serverState; }
		
		/**
		 * Gets the last error string from the TCP server.
		 *
		 * @return The last error string from the TCP server.
		 */
		QString getLastErrorString() const { return m_lastErrorString.isEmpty() ? m_tcpServer->errorString() : m_lastErrorString; }

		/**
		 * Gets the broadcasted resource url, empty means no resource is being broadcasted.
		 *
		 * @return The broadcasted resource url, empty means no resource is being broadcasted.
		 */
		QUrl getBroadcastResourceUrl() const { return m_broadcastResourceUrl; }

		/**
		 * Sets the broadcasted resource url, empty means no resource is being broadcasted.
		 *
		 * @param p_broadcastResourceUrl The broadcasted resource url, empty means no resource is being broadcasted.
		 */
		void setBroadcastResourceUrl(const QUrl& p_broadcastResourceUrl);

		/**
		 * Gets the response sent to the clients when they fetch the broadcast resource.
		 *
		 * @param p_withoutContent true to only send headers without the content, false otherwise
		 * 
		 * @return The response sent to the clients when they fetch the broadcast resource.
		 */
		QHttpServerResponse getBroadcastResourceResponse(bool p_withoutContent) const;

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
		 * @param p_serverState The new server state
		 */
		void serverStateChanged(ServerState p_serverState);

	private:

		/**
		 * The host address the HTTP and WebSocket servers will bind to. QHostAddress::Any for all interfaces.
		 */
		QHostAddress m_hostAddress{ QHostAddress::Any };

		/**
		 * The port number the HTTP and WebSocket servers will listen on.
		 */
		quint16 m_serverPort{ 8080 };

		/**
		 * The current state of the server.
		 */
		ServerState m_serverState{ ServerState::Stopped };

		/**
		 * The last startup error string.
		 */
		QString m_lastErrorString{};

		/**
		 * The TcpServer used to handle incoming HTTP connections.
		 */
		QTcpServer* m_tcpServer{ new QTcpServer(this) };

		/**
		 * The HTTP server that handles the routes for the clients to fetch.
		 */
		QHttpServer* m_httpServer{ new QHttpServer(this) };

		/**
		 * The list of WebSocket connections on the server.
		 */
		QList<QWebSocket*> m_webSocketConnections;

		/**
		 * The broadcasted resource url, empty means no resource is being broadcasted.
		 */
		QUrl m_broadcastResourceUrl{};

		/**
		 * Sets the current state of the server.
		 *
		 * @param p_serverState The new state of the server.
		 */
		void setServerState(ServerState p_serverState);

		/**
		 * Notifies all connected clients that the broadcast resource has changed.
		 */
		void notifyBroadcastResourceChanged() const;
	};
}

#endif // BROADCASTSERVER_H
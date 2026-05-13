#ifndef BROADCASTSERVER_H
#define BROADCASTSERVER_H

// Qt
#include <QTcpServer>
#include <QHttpServer>
#include <QWebSocketServer>

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
		 * Default constructor.
		 * 
		 * @param p_parent Parent QObject.
		 */
		explicit BroadcastServer(QObject* p_parent = nullptr);

		/**
		 * Destructor.
		 */
		~BroadcastServer() final = default;

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
		qint16 getServerPort() const { return m_serverPort; }

		/**
		 * Sets the port number the HTTP and WebSocket servers will listen on.
		 * Note: The server must be restarted for the changes to take effect.
		 *
		 * @param p_serverPort The port number the HTTP and WebSocket servers will listen on.
		 */
		void setServerPort(qint16 p_serverPort);

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

	private:
		
		/**
		 * The host address the HTTP and WebSocket servers will bind to. QHostAddress::Any for all interfaces.
		 */
		QHostAddress m_hostAddress{ QHostAddress::Any };

		/**
		 * The port number the HTTP and WebSocket servers will listen on.
		 */
		qint16 m_serverPort{ 8080 };

		/**
		 * The TcpServer used to handle incoming HTTP connections.
		 */
		QTcpServer* m_tcpServer{ new QTcpServer(this) };

		/**
		 * The HTTP server that handles the routes for the clients to fetch.
		 */
		QHttpServer* m_httpServer{ new QHttpServer(this) };

		/**
		 * The WebSocket server used notify the clients of a resource broadcast change.
		 */
		QWebSocketServer* m_webSocketServer{ nullptr };
	};
}

#endif // BROADCASTSERVER_H
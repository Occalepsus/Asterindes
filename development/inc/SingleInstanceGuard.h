#ifndef SINGLEINSTANCEGUARD_H
#define SINGLEINSTANCEGUARD_H

// Qt
#include <QPointer>
#include <QLocalServer>
#include <QLocalSocket>

namespace Asterindes
{
	class AsterindesCore;

	/**
	 * SingleInstanceGuard class is responsible for ensuring that only a single instance of the application is running.
	 */
	class SingleInstanceGuard : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY(SingleInstanceGuard);

	public:

		/**
		 * Default constructor.
		 *
		 * @param p_coreApp The pointer to the AsterindesCore application instance.
		 * @param p_parent The parent QObject, default is nullptr.
		 */
		explicit SingleInstanceGuard(AsterindesCore* p_coreApp, QObject* p_parent = nullptr);

		/**
		 * Destructor.
		 */
		~SingleInstanceGuard() final;

		/**
		 * Checks if another instance of the application is already running, and if so, sends what to do to the running instance.
		 *
		 * @param p_projectPath The path of the project to open. Empty project path means to open the startup window, otherwise it should be a local file path pointing to a valid project file.
		 *
		 * @return true if another instance is running and the project path was sent successfully, false otherwise.
		 */
		bool checkAndSendIfRunning(const QString& p_projectPath);

		/**
		 * Starts listening for incoming connections from other instances of the application.
		 */
		void startListeningForConnections();

	private:
		
		/**
		 * The name of the local server used in Asterindes.
		 */
		static constexpr const char* msc_serverName = "asterindesSingleInstanceGuard";

		/**
		 * The local server used to check for existing instances of the application.
		 */
		QLocalServer* m_localServer{ new QLocalServer(this) };

		/**
		 * The connected socket from another instance of the application.
		 * This only deal with one connection at a time.
		 */
		QLocalSocket* m_connectedSocket{ nullptr };

		/**
		 * The pointer to the AsterindesCore application instance.
		 */
		QPointer<AsterindesCore> m_coreApp{ nullptr };

	private slots:

		/**
		 * Slot called when a new connection is received from another instance of the application.
		 * This will ignore the connection if there is already a connected socket, this will be handled after the current connection is closed.
		 */
		void onNewConnection();
	};
}



#endif // SINGLEINSTANCEGUARD_H

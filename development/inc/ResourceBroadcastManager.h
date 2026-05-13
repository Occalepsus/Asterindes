#ifndef RESOURCEBROADCASTMANAGER_H
#define RESOURCEBROADCASTMANAGER_H

// Asterindes
#include "ResourceRegistry.h"
#include "BroadcastServer.h"

// Qt
#include <QObject>

namespace Asterindes
{
	/**
	 * The ResourceBroadcastManager class is responsible for managing the resources that are broadcasted to the clients via the BroadcastServer.
	 * It keeps track of the resource that is currently being broadcasted and provide methods to set and get the broadcasted resource.
	 */
	class ResourceBroadcastManager : public QObject
	{
		Q_OBJECT;
		Q_DISABLE_COPY(ResourceBroadcastManager);

	public:
		
		/**
		 * Default constructor.
		 * 
		 * @param p_parent Parent QObject.
		 */
		explicit ResourceBroadcastManager(QObject* p_parent = nullptr);

		/**
		 * Destructor.
		 */
		~ResourceBroadcastManager() final = default;

		/**
		 * Gets the resource that is currently being broadcasted.
		 * @return The resource that is currently being broadcasted.
		 */
		ResourceRegistry::Resource getBroadcastedResource() const { return m_broadcastedResource; }

		/**
		 * Sets the resource that is currently being broadcasted.
		 * @param resource The resource to broadcast.
		 */
		void setBroadcastedResource(const ResourceRegistry::Resource& resource);

	private:

		/**
		 * The resource that is currently being broadcasted, empty means nothing is being broadcasted.
		 */
		ResourceRegistry::Resource m_broadcastedResource;

		/**
		 * The BroadcastServer instance responsible for handling the HTTP server and sending resources updates to the clients via websockets.
		 */
		BroadcastServer* m_broadcastServer{ new BroadcastServer(this) };
	};
}

#endif // !RESOURCEBROADCASTMANAGER_H

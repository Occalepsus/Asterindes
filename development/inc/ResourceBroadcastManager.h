#ifndef RESOURCEBROADCASTMANAGER_H
#define RESOURCEBROADCASTMANAGER_H

// Asterindes
#include "ResourceRegistry.h"
#include "BroadcastServer.h"

// Qt
#include <QPointer>

namespace Asterindes
{
	class AsterindesProject;

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
		 * @param p_parent Parent AsterindesProject.
		 */
		explicit ResourceBroadcastManager(AsterindesProject* p_parent = nullptr);

		/**
		 * Destructor.
		 */
		~ResourceBroadcastManager() final = default;

		/**
		 * Gets the resource that is currently being broadcasted.
		 * 
		 * @return The resource that is currently being broadcasted.
		 */
		ResourceRegistry::Resource getBroadcastedResource() const { return m_broadcastedResource; }

		/**
		 * Sets the resource that is currently being broadcasted.
		 * 
		 * @param resource The resource to broadcast.
		 */
		void setBroadcastedResource(const ResourceRegistry::Resource& p_resource);

		/**
		 * Sets the resource that is currently being broadcasted using its URL and looking at the resource registry
		 * 
		 * @param p_resourceUrl The URL of the resource to look for at the resource registy and broadcast if found.
		 * 
		 * @return true if the resource was found at the resource registry and set as the broadcasted resource, false otherwise.
		 */
		bool setBroadcastedResourceFromUrl(const QUrl& p_resourceUrl);

	signals:

		/**
		 * Signal emitted when the broadcasted resource changes
		 */
		void broadcastedResourceChanged();

	private:

		/**
		 * A pointer to the resource registry, used to look for resources by their URL when setting the broadcasted resource.
		 */
		QPointer<ResourceRegistry> m_resourceRegistry;

		/**
		 * The resource that is currently being broadcasted, empty means nothing is being broadcasted.
		 */
		ResourceRegistry::Resource m_broadcastedResource{ ResourceRegistry::Resource() };

		/**
		 * The BroadcastServer instance responsible for handling the HTTP server and sending resources updates to the clients via websockets.
		 */
		BroadcastServer* m_broadcastServer{ new BroadcastServer(this) };
	};
}

#endif // !RESOURCEBROADCASTMANAGER_H

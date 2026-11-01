import Asterindes 1.0

import QtQuick

import QtQml.Models

import QtMultimedia

Item {
	id: resourceDisplayComponentRoot
	required property var resource
	
	Loader {
		anchors.fill: parent

		sourceComponent: resourceDisplayComponentRoot.resource && resourceDisplayComponentRoot.resource.resourceType === ResourceRegistry.Image ? imagePreview
						: resourceDisplayComponentRoot.resource && resourceDisplayComponentRoot.resource.resourceType === ResourceRegistry.Video ? videoPreview
						: null
		Component {
			id: imagePreview
			Image {
				anchors.fill: parent

				fillMode: Image.PreserveAspectFit
				source: resourceDisplayComponentRoot.resource ? (resourceDisplayComponentRoot.resource.resourceUrl || "") : ""
			}
		}

		Component {
			id: videoPreview
				
			Video {
				anchors.fill: parent
				
				autoPlay: true
				loops: MediaPlayer.Infinite
				source: resourceDisplayComponentRoot.resource ? (resourceDisplayComponentRoot.resource.resourceUrl || "") : ""
			}
		}
	}
}
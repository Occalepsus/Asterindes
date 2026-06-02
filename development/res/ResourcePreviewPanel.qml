import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
	property var previewResource: projectWindow.resourcesViewModel ? projectWindow.resourcesViewModel.getResourceAtIndex(projectWindow.resourcesViewModel.selectedResourceIndex) : null
	property bool hasValidSelection: projectWindow.resourcesViewModel && projectWindow.resourcesViewModel.selectedResourceIndex >= 0
	
	id: resourcePreviewPanelRoot
	anchors.fill: parent

	ColumnLayout {

		anchors.fill: parent
		visible: resourcePreviewPanelRoot.hasValidSelection

		// Resource preview image
		Image {
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.minimumHeight: 20
			Layout.maximumWidth: parent.width
			Layout.maximumHeight: parent.height - 20
			fillMode: Image.PreserveAspectFit
			source: resourcePreviewPanelRoot.previewResource ? (resourcePreviewPanelRoot.previewResource.resourceUrl || "") : ""
		}

		// Resource name field, modifiable
		TextField {
			id: resourceNameField
			Layout.fillWidth: false
			placeholderText: "Nom affiché"
			text: resourcePreviewPanelRoot.previewResource ? (resourcePreviewPanelRoot.previewResource.name || "") : ""

			onAccepted: {
				if (projectWindow.resourcesViewModel && resourcePreviewPanelRoot.previewResource) {
					projectWindow.resourcesViewModel.renameResource(resourcePreviewPanelRoot.previewResource.resourceUrl, text)
				}
			}
		}
	}
	
	// Placeholder when nothing is selected
	Text {
		anchors.centerIn: parent
		visible: !resourcePreviewPanelRoot.hasValidSelection
		text: "No resource selected"
		color: "#888888"
	}

	Column {
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.margins: 10
		spacing: 8

		Button {
			enabled: resourcePreviewPanelRoot.hasValidSelection
			text: "🗑"
			width: 32
			height: 32

			onClicked: {
				if (projectWindow.resourcesViewModel && resourcePreviewPanelRoot.hasValidSelection) {
					projectWindow.resourcesViewModel.removeResources([resourcePreviewPanelRoot.previewResource.resourceUrl])
				}
			}
		}

		Button {
			enabled: resourcePreviewPanelRoot.hasValidSelection

			text: "⇣"
			width: 32
			height: 32

			onClicked: {
				if (projectViewModel && resourcePreviewPanelRoot.hasValidSelection) {
					projectViewModel.setBroadcastedResourceUrl(resourcePreviewPanelRoot.previewResource.resourceUrl)
				}
			}
		}
	}
}
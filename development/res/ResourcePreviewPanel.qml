import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
	property var previewResource: resourcesViewModel ? resourcesViewModel.getResourceAtIndex(resourcesViewModel.selectedResourceIndex) : null
	property bool hasValidSelection: resourcesViewModel ? resourcesViewModel.selectedResourceIndex >= 0 : false
	
	id: resourcePreviewPanelRoot
	anchors.fill: parent

	ColumnLayout {

		anchors.fill: parent
		visible: resourcePreviewPanelRoot.hasValidSelection

		Image {
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.minimumHeight: 20
			Layout.maximumWidth: parent.width
			Layout.maximumHeight: parent.height - 20
			fillMode: Image.PreserveAspectFit
			source: resourcePreviewPanelRoot.previewResource ? (resourcePreviewPanelRoot.previewResource.resourceUrl || "") : ""
		}
		Text {
			Layout.fillWidth: true
			Layout.preferredHeight: 20
			horizontalAlignment: Text.AlignHCenter

			text: resourcePreviewPanelRoot.previewResource ? (resourcePreviewPanelRoot.previewResource.name || "") : ""
		}
	}
	
	// Placeholder when nothing is selected
	Text {
		anchors.centerIn: parent
		visible: !resourcePreviewPanelRoot.hasValidSelection
		text: "No resource selected"
		color: "#888888"
	}

	// Send to broadcast button
	Button {
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.margins: 10
		
		width: 25
		height: 25

		visible: resourcePreviewPanelRoot.hasValidSelection
		
		text: "⇣"
		
		onClicked: {
			if (resourcesViewModel && resourcePreviewPanelRoot.hasValidSelection) {
				resourcesViewModel.setBroadcastedResourceUrl(resourcePreviewPanelRoot.previewResource.resourceUrl);
			}
		}
	}
}
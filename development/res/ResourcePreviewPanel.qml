import QtQuick
import QtQuick.Layouts

Item {
	property var previewResource: resourcesViewModel.getResourceAtIndex(resourcesViewModel.selectedResourceIndex)
	property bool hasValidSelection: resourcesViewModel.selectedResourceIndex >= 0
	
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
			source: resourcePreviewPanelRoot.previewResource.resourceUrl || ""
		}
		Text {
			Layout.fillWidth: true
			Layout.preferredHeight: 20
			horizontalAlignment: Text.AlignHCenter
			
			text: resourcePreviewPanelRoot.previewResource.name || ""
		}
	}
	
	// Placeholder when nothing is selected
	Text {
		anchors.centerIn: parent
		visible: !resourcePreviewPanelRoot.hasValidSelection
		text: "No resource selected"
		color: "#888888"
	}
}
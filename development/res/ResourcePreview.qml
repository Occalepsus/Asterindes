import QtQuick
import QtQuick.Layouts

Item {
	Layout.fillWidth: true
	Layout.fillHeight: true
	
	ColumnLayout {
		property var previewResource: resourcesViewModel.getResourceAtIndex(resourcesViewModel.selectedResourceId)
		property bool hasValidSelection: resourcesViewModel.selectedResourceId >= 0

		anchors.fill: parent
		visible: hasValidSelection

		Image {
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.minimumHeight: 20
			Layout.maximumWidth: parent.width
			Layout.maximumHeight: parent.height - 20
			fillMode: Image.PreserveAspectFit
			source: parent.previewResource.resourceUrl || ""
		}
		Text {
			Layout.fillWidth: true
			Layout.preferredHeight: 20
			horizontalAlignment: Text.AlignHCenter
			
			text: parent.previewResource.name || ""
		}
	}
	
	// Placeholder when nothing is selected
	Text {
		anchors.centerIn: parent
		visible: resourcesViewModel.selectedResourceId < 0
		text: "No resource selected"
		color: "#888888"
	}
}
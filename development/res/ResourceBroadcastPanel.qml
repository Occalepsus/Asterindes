import QtQuick

Item {
	id: resourcePreviewPanelRoot
	anchors.fill: parent
	
	ResourceDisplayComponent {
		anchors.fill: parent

		resource: projectWindow.projectViewModel ? projectWindow.projectViewModel.broadcastedResource : null

	}
}
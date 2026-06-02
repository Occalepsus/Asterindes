import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

Item {
	id: resourcePreviewPanelRoot
	anchors.fill: parent

	Image {
		anchors.fill: parent

		fillMode: Image.PreserveAspectFit

		source: projectWindow.projectViewModel ? projectWindow.projectViewModel.broadcastedResourceUrl : "" || ""
	}
}
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
	id: resourcePreviewPanelRoot

	property int tagRevision: 0
	property var previewResource: {
		return tagRevision >= 0 && projectWindow.resourcesViewModel
			? projectWindow.resourcesViewModel.getResourceAtIndex(projectWindow.resourcesViewModel.selectedResourceIndex)
			: null
	}
	property bool hasValidSelection: projectWindow.resourcesViewModel && projectWindow.resourcesViewModel.selectedResourceIndex >= 0
	
	anchors.fill: parent

	Connections {
		target: projectWindow.resourcesViewModel
		function onSelectedResourceTagListChanged() {
			resourcePreviewPanelRoot.tagRevision++
			if (tagSelectionPopup.visible) {
				tagSelectionPopup.refreshTagSelectionData()
			}
		}
	}
	
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
		
		Item {
			id: resourceTags
			Layout.fillWidth: true
			Layout.preferredHeight: Math.max(24, tagsFlow.implicitHeight)

			Flow {
				id: tagsFlow
				anchors.left: parent.left
				anchors.right: parent.right
				spacing: 4

				Repeater {
					model: resourcePreviewPanelRoot.previewResource
						   ? resourcePreviewPanelRoot.previewResource.tagList
						   : []

					delegate: Rectangle {
						required property var modelData

						property int horPadding: 6
						property int vertPadding: 2

						implicitWidth: tagText.implicitWidth + horPadding * 2
						implicitHeight: tagText.implicitHeight + vertPadding * 2

						color: "#cccccc"
						radius: implicitHeight / 3

						Text {
							id: tagText

							anchors.centerIn: parent
							text: modelData
							color: "#000000"
						}
					}
				}
			}

			Text {
				anchors.left: parent.left
				anchors.verticalCenter: parent.verticalCenter
				
				visible: resourcePreviewPanelRoot.previewResource && resourcePreviewPanelRoot.previewResource.tagList !== undefined && resourcePreviewPanelRoot.previewResource.tagList.length === 0

				text: "No tags yet, click here to add..."
				color: "#333333"
			}
			
			TapHandler {
				acceptedButtons: Qt.LeftButton

				onTapped: {
				    if (resourcePreviewPanelRoot.previewResource) {
						tagSelectionPopup.open()
					}
				}
			}

			Popup {
				id: tagSelectionPopup
				parent: resourceTags
				x: 0
				y: resourceTags.height

				width: 200
				height: 300
				
				function refreshTagSelectionData() {
					if (!projectWindow.resourcesViewModel) {
						return
					}
					
					tagSelectionList.selectedTagList = resourcePreviewPanelRoot.previewResource
						? resourcePreviewPanelRoot.previewResource.tagList
						: []
					tagSelectionList.allTagList = projectWindow.resourcesViewModel.getAllResourceTags()
				}
				   
				onOpened: {
					refreshTagSelectionData()
					tagSelectionList.mClearSearch()
				}

				TagSelectionList {
					id: tagSelectionList
					allTagList: projectWindow.resourcesViewModel ? projectWindow.resourcesViewModel.getAllResourceTags() : []
					selectedTagList: resourcePreviewPanelRoot.previewResource ? resourcePreviewPanelRoot.previewResource.tagList : []

					canCreateNewTag: true

					onTagSelected: function(pTag, pIsSelected) {
						if (projectWindow.resourcesViewModel && resourcePreviewPanelRoot.previewResource) {
							projectWindow.resourcesViewModel.setResourceTag(resourcePreviewPanelRoot.previewResource.resourceUrl, pTag, pIsSelected)
						}
						tagSelectionPopup.refreshTagSelectionData();
					}
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
				if (projectWindow.projectViewModel && resourcePreviewPanelRoot.hasValidSelection) {
					projectWindow.projectViewModel.setBroadcastedResourceUrl(resourcePreviewPanelRoot.previewResource.resourceUrl)
				}
			}
		}
	}
}
import Asterindes 1.0

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

Item {
	id: resourcesGridPanel

	anchors.fill: parent

	property int mMinColCount: 1
	property int mMaxColCount: 10

	property int prefColCount: 3
	property int realColCount: prefColCount

	property int mGridItemSize: 120
	property int mGridTextHeight: 20

	// Update real column count based on available width and preferred column count
	function updateGridItemSize() {
		mGridItemSize = resourceGridView.width / realColCount // 5px margin
	}

	onPrefColCountChanged: {
		realColCount = prefColCount
	}

	onRealColCountChanged: {
		if (realColCount < 1) realColCount = 1
		updateGridItemSize()
	}

	ColumnLayout {
		anchors.fill: parent
		
		// ===== Toolbar with search and sort =====
		ToolBar {
			Layout.fillWidth: true
			
			RowLayout {
				anchors.fill: parent
				spacing: 10

				TextField {
					id: searchField
					Layout.fillWidth: true
					placeholderText: "Search resources..."
					
					// Bind to ViewModel's search filter
					text: projectWindow.resourcesViewModel ? projectWindow.resourcesViewModel.displayedResourceListModel.nameSearchFilter : ""
					onTextChanged: {
						if (projectWindow.resourcesViewModel) {
							projectWindow.resourcesViewModel.displayedResourceListModel.nameSearchFilter = text
						}
					}
				}

				ComboBox {
					id: sortRoleComboBox
					Layout.preferredWidth: 150
					
					model: ["Name", "Creation date"]
					
					// Bind to ViewModel's sort order
					currentIndex: {
						let currentRoleIndex = 0;

						if (projectWindow.resourcesViewModel) {
							const currentRole = projectWindow.resourcesViewModel.displayedResourceListModel.sortRole
							
							switch (currentRole) {
								case ResourceListModel.CreationDateRole:
									currentRoleIndex = 1;
									break;
								default:
									currentRoleIndex = 0;
							}
						}

						return currentRoleIndex
					}
					onCurrentIndexChanged: {
						if (projectWindow.resourcesViewModel) {

							switch (currentIndex) {
								case 1:
									projectWindow.resourcesViewModel.displayedResourceListModel.sortRole = ResourceListModel.CreationDateRole;
									break;
								default:
									projectWindow.resourcesViewModel.displayedResourceListModel.sortRole = ResourceListModel.NameRole;
							}
						}
					}
				}

				Button {
					id: sortOrderButton

					text: projectWindow.resourcesViewModel && projectWindow.resourcesViewModel.displayedResourceListModel.sortOrder === Qt.DescendingOrder ? "↓" : "↑"

					onClicked: {
						if (projectWindow.resourcesViewModel) {
							const currentOrder = projectWindow.resourcesViewModel.displayedResourceListModel.sortOrder
							projectWindow.resourcesViewModel.displayedResourceListModel.sortOrder = currentOrder === Qt.AscendingOrder ? Qt.DescendingOrder : Qt.AscendingOrder
						}
					}
				}

				Button {
					id: tagsFilterButton

					text: "Tags " + (projectWindow.resourcesViewModel && projectWindow.resourcesViewModel.displayedResourceListModel.tagFilterList.length > 0
						? "(" + projectWindow.resourcesViewModel.displayedResourceListModel.tagFilterList.length + ")"
						: "")

					onClicked: {
						tagListFilterPopup.open()
					}
					
					Popup {
						id: tagListFilterPopup

						x: 0
						y: 0

						width: 200
						height: 300

						TagSelectionList {
							id: tagListFilter
							allTagList: projectWindow.resourcesViewModel ? projectWindow.resourcesViewModel.getAllResourceTags() : []
							selectedTagList: projectWindow.resourcesViewModel ? projectWindow.resourcesViewModel.displayedResourceListModel.tagFilterList : []
							canCreateNewTag: false
							onTagSelected: function(pTag, pIsSelected) {
								if (projectWindow.resourcesViewModel) {
									let newTagFilterList = projectWindow.resourcesViewModel.displayedResourceListModel.tagFilterList
									const l_index = newTagFilterList.indexOf(pTag)
									
									if (pIsSelected) {
										if (l_index < 0) {
											newTagFilterList.push(pTag)
										}
									} else {
										if (l_index >= 0) {
											newTagFilterList.splice(l_index, 1)
										}
									}

									projectWindow.resourcesViewModel.displayedResourceListModel.tagFilterList = newTagFilterList
								}
							}
						}
					}
				}

				Label {
					text: "Item per columns"
				}

				Slider {
					id: gridSizeSlider
					Layout.preferredWidth: 140
					from: resourcesGridPanel.mMinColCount
					to: resourcesGridPanel.mMaxColCount
					stepSize: 1
					value: resourcesGridPanel.prefColCount
					onMoved: resourcesGridPanel.prefColCount = value
				}

				Label {
					text: resourcesGridPanel.realColCount + " items"
				}

				Button {
					text: "Clear Filters"
					enabled: searchField.text !== "" || sortRoleComboBox.currentIndex !== 0
					onClicked: projectWindow.resourcesViewModel ?? projectWindow.resourcesViewModel.displayedResourceListModel.clearFilters()
				}

				//Label {
				//	text: "bb"/*projectWindow.resourcesViewModel.filteredCount */+ " / " + "0"
				//	color: "blue"//projectWindow.resourcesViewModel.filteredCount < projectWindow.resourcesViewModel.displayedResourceListCount ? "blue" : "black"
				//}
			}
		}
			
		// ===== Grid view =====
		DropArea {
			Layout.fillWidth: true
			Layout.fillHeight: true

			enabled: projectWindow.resourcesViewModel ? !projectWindow.resourcesViewModel.isLoading : true

			onEntered: (drag) => {
				let lCanDrop = true;
				if (projectWindow.resourcesViewModel && drag.hasUrls) {
					for (let i = 0; i < drag.urls.length; i++) {
						if (!projectWindow.resourcesViewModel.canAddResource(drag.urls[i])) {
							lCanDrop = false;
							break;
						}
					}
				} else {
					lCanDrop = false;
				}
				drag.action = Qt.LinkAction;
				drag.accepted = lCanDrop;
			}

			onDropped: (drag) => {
				if (projectWindow.resourcesViewModel && drag.hasUrls) {
					projectWindow.resourcesViewModel.addResources(drag.urls);
				}
			}

			Component {
				id: resourceGridDelegate

				Column {
					width: resourceGridView.cellWidth
					height: resourceGridView.cellHeight

					Image {
						width: parent.width
						height: parent.height - resourcesGridPanel.mGridTextHeight
						anchors.horizontalCenter: parent.horizontalCenter

						fillMode: Image.PreserveAspectFit

						source: resourceUrl
					}

					Text {
						width: parent.width
						height: resourcesGridPanel.mGridTextHeight
						anchors.horizontalCenter: parent.horizontalCenter
				
						text: name
						horizontalAlignment: Text.AlignHCenter
						elide: Text.ElideRight
					}
				}
			}
	
			GridView {
				id: resourceGridView

				// Property to avoid infinite loop when syncing selection between ViewModel and GridView
				property bool l_syncingSelection: false

				anchors.fill: parent
				anchors.margins: 10

				onWidthChanged: {
					updateGridItemSize()
				}

				// Bind to ViewModel's model (already filtered and sorted)
				model: projectWindow.resourcesViewModel ? projectWindow.resourcesViewModel.displayedResourceListModel : null

				cellWidth: resourcesGridPanel.mGridItemSize
				cellHeight: resourcesGridPanel.mGridItemSize

				delegate: resourceGridDelegate
				highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
				// No move animation when changing selection
				highlightMoveDuration: 0
				focus: true
								
				currentIndex: -1
				onCurrentIndexChanged: {
					if (l_syncingSelection || !projectWindow.resourcesViewModel) {
						return
					}
					projectWindow.resourcesViewModel.setSelectedResourceIndex(currentIndex)
				}

				Connections {
					target: projectWindow.resourcesViewModel
					function onSelectedResourceIndexChanged() {
						if (!projectWindow.resourcesViewModel) return
						const l_newIndex = projectWindow.resourcesViewModel.selectedResourceIndex
						if (resourceGridView.currentIndex === l_newIndex) return
						resourceGridView.l_syncingSelection = true
						resourceGridView.currentIndex = l_newIndex
						resourceGridView.l_syncingSelection = false
					}
				}

				// Handle clicks to select resources
				MouseArea {
					anchors.fill: parent
					onClicked: (mouse) => {
						if (projectWindow.resourcesViewModel) {
							let posInGridView = Qt.point(mouse.x, mouse.y)
							let posInContentItem = mapToItem(resourceGridView.contentItem, posInGridView)
							projectWindow.resourcesViewModel.setSelectedResourceIndex(
								resourceGridView.indexAt(posInContentItem.x, posInContentItem.y)
							)
						}
					}
					onDoubleClicked: (mouse) => {
						if (projectWindow.resourcesViewModel) {
							let posInGridView = Qt.point(mouse.x, mouse.y)
							let posInContentItem = mapToItem(resourceGridView.contentItem, posInGridView)
							let index = resourceGridView.indexAt(posInContentItem.x, posInContentItem.y)

							if (index >= 0 && projectWindow.projectViewModel) {
								let resource = projectWindow.resourcesViewModel.getResourceAtIndex(index);
								if (resource && resource.resourceUrl) {
									projectWindow.projectViewModel.setBroadcastedResourceUrl(resource.resourceUrl);
								}
							}
						}
					}
					onWheel: (wheel) => {
						if (!(wheel.modifiers & Qt.ControlModifier)) {
							wheel.accepted = false
							return
						}

						let newPrefColCount = resourcesGridPanel.prefColCount + (wheel.angleDelta.y < 0 ? 1 : -1)
						if (newPrefColCount < 1) newPrefColCount = 1
						resourcesGridPanel.prefColCount = newPrefColCount
						
						wheel.accepted = true
					}
				}

				// Empty state
				Label {
					anchors.centerIn: parent
					visible: parent.count === 0 && (projectWindow.resourcesViewModel ? !projectWindow.resourcesViewModel.isLoading : true)
					text: false//projectWindow.resourcesViewModel.searchFilter !== "" 
							? "No resources match your search"
							: "Drop images here or click 'Add Resource'"
					font.pixelSize: 16
					color: "gray"
				}
			}

			BusyIndicator {
				anchors.centerIn: parent
				running: projectWindow.resourcesViewModel ? projectWindow.resourcesViewModel.isLoading : false
				visible: running
			}
		}

		// ===== Bottom toolbar =====
		ToolBar {
			Layout.fillWidth: true
			
			RowLayout {
				anchors.fill: parent

				FileDialog {
					id: resourceFileDialog
					title: "Select Resource File"
					fileMode: FileDialog.OpenFiles
					nameFilters: ["Images (*.png *.jpg *.jpeg *.webp)"]
					onAccepted: {
						if (projectWindow.resourcesViewModel) {
							projectWindow.resourcesViewModel.addResources(selectedFiles)
						}
					}
				}

				Button {
					text: "Add Resource"
					enabled: projectWindow.resourcesViewModel ? !projectWindow.resourcesViewModel.isLoading : false
					onClicked: resourceFileDialog.open()
				}

				Button {
					text: "Clear All"
					enabled: false//!projectWindow.resourcesViewModel.isLoading && projectWindow.resourcesViewModel.displayedResourceListCount > 0
					//onClicked: confirmClearDialog.open()
				}

				TextField {
					id: resourceUrlField
					Layout.preferredWidth: 280
					placeholderText: "http://exemple.com/image.png"
					selectByMouse: true

					onAccepted: addWebResourceButton.clicked()
				}

				Button {
					id: addResourceUrlButton
					text: "Add Resource"
					enabled: projectWindow.resourcesViewModel ? !projectWindow.resourcesViewModel.isLoading && resourceUrlField.text !== "" : false

					onClicked: {
						if (projectWindow.resourcesViewModel) {
							const url = resourceUrlField.text
							if (projectWindow.resourcesViewModel.addResources([url])) {
								resourceUrlField.clear()
							}
						}
					}
				}

				Item { Layout.fillWidth: true } // Spacer
			}
		}

		// Error dialog
		Connections {
			target: projectWindow.resourcesViewModel
			function onErrorOccurred(errorMessage) {
				errorDialog.text = errorMessage;
				errorDialog.open();
			}
		}

		Dialog {
			id: errorDialog
			title: "Error"
			property alias text: errorText.text
		
			Text { id: errorText }
			standardButtons: Dialog.Ok
		}
	}
}
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

Item {
	id: resourcesGridPanel

	anchors.fill: parent

	property int mMinGridItemSize: 80
	property int mMaxGridItemSize: 320
	property int mGridItemSize: 120
	property int mGridTextHeight: 20

	function setGridItemSize(pSize) {
		let lClampedSize = Math.max(mMinGridItemSize, Math.min(mMaxGridItemSize, Math.round(pSize / 10) * 10))
		mGridItemSize = lClampedSize
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
					text: "aa"//projectWindow.resourcesViewModel.searchFilter
					//onTextChanged: projectWindow.resourcesViewModel.searchFilter = text
				}

				ComboBox {
					id: sortComboBox
					Layout.preferredWidth: 150
					
					model: ["Name (A-Z)", "Name (Z-A)", "Date Added", "File Size"]
					
					// Bind to ViewModel's sort order
					//currentIndex: projectWindow.resourcesViewModel.sortOrder
					onCurrentIndexChanged: {
						//projectWindow.resourcesViewModel.sortOrder = currentIndex;
					}
				}

				Label {
					text: "Item size"
				}

				Slider {
					id: gridSizeSlider
					Layout.preferredWidth: 140
					from: resourcesGridPanel.mMinGridItemSize
					to: resourcesGridPanel.mMaxGridItemSize
					stepSize: 10
					value: resourcesGridPanel.mGridItemSize
					onMoved: resourcesGridPanel.setGridItemSize(value)
				}

				Label {
					text: resourcesGridPanel.mGridItemSize + " px"
				}

				Button {
					text: "Clear Filters"
					enabled: searchField.text !== "" || sortComboBox.currentIndex !== 0
					//onClicked: projectWindow.resourcesViewModel.clearFilters()
				}

				Label {
					text: "bb"/*projectWindow.resourcesViewModel.filteredCount */+ " / " + (projectWindow.resourcesViewModel ? projectWindow.resourcesViewModel.displayedResourceListCount : 0)
					color: "blue"//projectWindow.resourcesViewModel.filteredCount < projectWindow.resourcesViewModel.displayedResourceListCount ? "blue" : "black"
				}
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
					resourceViewModel.addResources(drag.urls);
				}
			}

			GridView {
				id: resourceGridView
				anchors.fill: parent
				anchors.margins: 10

				// Bind to ViewModel's model (already filtered and sorted)
				model: projectWindow.resourcesViewModel ? projectWindow.resourcesViewModel.displayedResourceListModel : null

				cellWidth: resourcesGridPanel.mGridItemSize
				cellHeight: resourcesGridPanel.mGridItemSize

				delegate: resourceGridDelegate
				highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
				// No move animation when changing selection
				highlightMoveDuration: 0
				focus: true
								
				currentIndex: projectWindow.resourcesViewModel ? projectWindow.resourcesViewModel.selectedResourceIndex : -1
				onCurrentIndexChanged: {
					if (projectWindow.resourcesViewModel && currentIndex !== projectWindow.resourcesViewModel.selectedResourceIndex) {
						projectWindow.resourcesViewModel.setSelectedResourceIndex(currentIndex)
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

						resourcesGridPanel.setGridItemSize(resourcesGridPanel.mGridItemSize + (wheel.angleDelta.y > 0 ? 10 : -10))
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
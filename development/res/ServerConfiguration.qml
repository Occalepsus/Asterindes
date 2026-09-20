import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
	id: serverConfigurationRoot
	
	property bool isServerRunning: projectWindow ? projectWindow.serverConfiguration.serverState.startsWith("Running") : false

	MouseArea {
		anchors.fill: parent

		onClicked: {
			configurationLayout.forceActiveFocus()
		}

		ColumnLayout {
			id: configurationLayout

			anchors.fill: parent
		
			// Server status
			Rectangle {
				id: statusArea

				readonly property int margins: 5
				property bool addressCopied: false

				Layout.fillWidth: true
				implicitHeight: statusText.implicitHeight + 2 * margins
		
				Layout.margins: 3

				function getStatusColor(p_lightColor) {
					// Dirty but easy to do: since this is called whenever serverState is changed, that means that the copied text status is not anymore true
					addressCopied = false

					const state = projectWindow ? projectWindow.serverConfiguration.serverState : "Error";
					
					if (state.startsWith("Stopped")) {
						return p_lightColor ? "lightgray" : "gray";
					}
					else if (state.startsWith("Running")) {
						return p_lightColor ? "lightgreen" : "green";
					}
					else {
						return p_lightColor ? "lightCoral" : "red";
					}
				}

				color: getStatusColor(true)

				border.color: getStatusColor(false)
				radius: margins

				Item {
					anchors.fill: parent

					Text {
						id: statusText
			
						anchors.left: parent.left
						anchors.margins: statusArea.margins
						anchors.verticalCenter: parent.verticalCenter
					
						text: projectWindow ? projectWindow.serverConfiguration.serverState : "Error: no server"
						elide: Qt.ElideRight
					}

					Button {
						id: copyAddressButton
						
						anchors.left: statusText.right
						anchors.margins: statusArea.margins

						visible: serverConfigurationRoot.isServerRunning
						hoverEnabled: true

						text: statusArea.addressCopied ? "✅" : "📑"
						background: Item {}

						onClicked: {
							console.log(projectWindow.serverConfiguration.getUsableIpAddress() + ":" + projectWindow.serverConfiguration.serverPort)
							projectWindow.serverConfiguration.copyTextToClipboard(projectWindow.serverConfiguration.getUsableIpAddress() + ":" + projectWindow.serverConfiguration.serverPort)
							statusArea.addressCopied = true
						}

						ToolTip.text: statusArea.addressCopied ? "Address copied!" : "Click to copy address"
						ToolTip.visible: hovered
						ToolTip.delay: 0
					}
				}
			}
			
			// Configuration : port
			RowLayout {
				Layout.fillWidth: true

				Layout.margins: 3

				Label {
					text: "Port:"
				}

				// Port input
				TextField {
					id: portInput

					placeholderText: "8080"

					text: projectWindow ? projectWindow.serverConfiguration.serverPort : "0"
					color: acceptableInput ? "black" : "red"

					enabled: !serverConfigurationRoot.isServerRunning
					validator: IntValidator { bottom: 1; top: 65535; }

					onEditingFinished: {
						if (projectWindow) {
							projectWindow.serverConfiguration.serverPort = text
						}
						parent.forceActiveFocus()
					}
				}
			}

			// Spacer
			Item {
				Layout.fillHeight: true
			}

			// Start / Stop server
			Button {
				id: startServerButton

				Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

				Layout.margins: 3

				text: serverConfigurationRoot.isServerRunning ? "⏹ Stop server" : "▶️ Start server"

				onClicked: {
					if (projectWindow) {
						if (serverConfigurationRoot.isServerRunning) {
							projectWindow.serverConfiguration.stopServer()
						} else {
							projectWindow.serverConfiguration.startServer()
						}
					}
				}
			}
		}
	}
}
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

				Layout.fillWidth: true
				implicitHeight: statusText.implicitHeight + 2 * margins
		
				Layout.margins: 3

				function getStatusColor(p_lightColor) {
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

				Text {
					id: statusText
			
					anchors.fill: parent
					anchors.margins: statusArea.margins
					
					text: projectWindow ? projectWindow.serverConfiguration.serverState : "Error: no server"
					elide: Qt.ElideRight
				}
			}

			// Configuration : ip address
			ComboBox {
				id: ipAddressInput

				Layout.fillWidth: true

				readonly property var ipAddresses: projectWindow
					? projectWindow.serverConfiguration.getAvailableIpAddresses()
					: ({})

				enabled: !serverConfigurationRoot.isServerRunning
				model: ["Unavailable"]

				onIpAddressesChanged: {
					const l_keys = Object.keys(ipAddresses || {})
					l_keys.sort()
					ipAddressInput.model = l_keys.length > 0 ? l_keys : ["Unavailable"]
					
					if (projectWindow) {
						const l_address = projectWindow.serverConfiguration.serverAddress
						l_keys.forEach((key) => {
							if (key.includes(l_address)) {
								currentValue = key;
							}
						})
					}
				}

				onActivated: {
					if (projectWindow && ipAddresses[currentText] !== undefined) {
						projectWindow.serverConfiguration.serverAddress = ipAddresses[currentText]
					}
				}
			}

			// Configuration : port
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

			// Spacer
			Item {
				Layout.fillHeight: true
			}

			// Start / Stop server
			Button {
				id: startServerButton

				Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

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
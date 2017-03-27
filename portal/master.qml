/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
import QtQuick 2.2

Item {
    width: 400
    height: 300

    Rectangle {
		anchors.fill: parent
		color: "#2d333a"

		Image {
			id: logo
			anchors.fill: parent
			source: "http://develorium.com/wp-content/uploads/2015/02/logo2_3.png"
		}

		Timer {
			interval: 40
			repeat: true
			running: true
			onTriggered: logo.rotation += 1
		}

		border.color: "red"
    }
}
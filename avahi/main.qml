/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
import QtQuick 2.2
import QtQuick.Window 2.1

Window {
    visible: true
    width: 600
    height: 200
    property string colorString: "red"
    property string statusString: ""

    Rectangle {
	anchors.fill: parent
	color: "#2d333a"

	Row {
	    anchors.centerIn: parent

 	    Rectangle {
		width: 30
		height: 30
		radius: width / 2
		color: colorString
	    }

	    Item {
		width: 5
		height: 1
	    }
	    
	    Text {
		text: statusString
		font.bold: true
		font.pixelSize: 32
	    }
	}
    }
}

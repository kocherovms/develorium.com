/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
import QtQuick 2.2
import com.develorium.Portal 1.0

Item {
    width: 400
    height: 300
    anchors.fill: parent

    Portal {
	objectName: "portal"
	anchors.fill: parent
    }
}
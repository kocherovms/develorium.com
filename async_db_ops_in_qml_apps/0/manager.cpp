#include <stdexcept>
#include <QQuickWindow>
#include <QApplication>
#include <QDesktopWidget>
#include "manager.h"

Manager::Manager(): rootObject_(0) {
	loadQml();
	QQuickWindow & window = dynamic_cast<QQuickWindow &>(*rootObject_);
	const QPoint screenCenter = QApplication::desktop()->screen()->rect().center();
	window.setX(screenCenter.x() - window.width() / 2);
	window.setY(screenCenter.y() - window.height() / 2);
	window.show();
}

void Manager::loadQml() {
	engine_.load(QUrl("qrc:/main.qml"));
	const QList<QObject *> ro = engine_.rootObjects();
	
	if(ro.isEmpty())
		throw std::runtime_error("No root objects in QML");

	rootObject_ = ro[0];
}


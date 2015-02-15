#include <stdexcept>
#include <QQuickWindow>
#include <QCoreApplication>
#include "manager.h"

Manager::Manager(): rootObject_(0) {
	engine_.load(QUrl::fromLocalFile("main.qml"));
	const QList<QObject *> ro = engine_.rootObjects();
	
	if(ro.isEmpty())
		throw std::runtime_error("No root objects in QML");

	rootObject_ = ro[0];
	QObject::connect(rootObject_, SIGNAL(searchButtonPressed(QString)), this, SLOT(onSearchButtonPressed(QString)));
	QQuickWindow & window = dynamic_cast<QQuickWindow &>(*rootObject_);
	window.show();
}

void Manager::onSearchButtonPressed(const QString & theSearchPhrase) {
}

/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include <QDebug>
#include <QTimer>
#include <QQuickWindow>
#include <QApplication>
#include <QDesktopWidget>
#include "manager.h"

/* Manager */

Manager::Manager(): rootObject_(0) {
	initAdvertiser();
	loadQml();
	QQuickWindow & window = dynamic_cast<QQuickWindow &>(*rootObject_);
	const QPoint screenCenter = QApplication::desktop()->screen()->rect().center();
	window.setX(screenCenter.x() - window.width() / 2);
	window.setY(screenCenter.y() - window.height() / 2);
	window.show();
	onServiceAdvertisingNeeded();
}

void Manager::onServiceAdvertisingNeeded() {
	rootObject_->setProperty("colorString", "yellow");
	rootObject_->setProperty("statusString", "Service advertising is running");
	advertiser_->start();
}

void Manager::onServiceAdvertisingOked() {
	rootObject_->setProperty("colorString", "green");
	rootObject_->setProperty("statusString", "Service advertised");
}

void Manager::onServiceAdvertisingFailed() {
	rootObject_->setProperty("colorString", "red");
	rootObject_->setProperty("statusString", "Service is not advertised");
	enum { ReadvertiseTimeout = 15 };
	qDebug() << QString("Avahi service advertising failed, will reschedule advertising in %1 seconds").arg(ReadvertiseTimeout);
	QTimer::singleShot(ReadvertiseTimeout * 1000, this, SLOT(onServiceAdvertisingNeeded()));
}

void Manager::initAdvertiser() {
	advertiser_.reset(new AvahiServiceAdvertiser("my_service", 9999));
	connect(advertiser_.data(), &AvahiServiceAdvertiser::serviceAdvertisingOk, this, &Manager::onServiceAdvertisingOked);
	connect(advertiser_.data(), &AvahiServiceAdvertiser::serviceAdvertisingFailed, this, &Manager::onServiceAdvertisingFailed);
}

void Manager::loadQml() {
	qsrand(time(0));
	engine_.load(QUrl("qrc:/main.qml"));
	const QList<QObject *> ro = engine_.rootObjects();
	
	if(ro.isEmpty())
		throw std::runtime_error("No root objects in QML");

	rootObject_ = ro[0];
}



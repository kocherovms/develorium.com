/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef MANAGER_H_
#define MANAGER_H_

#include <QQmlApplicationEngine>
#include "avahiserviceadvertiser.h"

class Manager: public QObject {
	Q_OBJECT
public:
	Manager();
public slots:
	void onServiceAdvertisingNeeded();
	void onServiceAdvertisingOked();
	void onServiceAdvertisingFailed();
private:
	void initAdvertiser();
	void loadQml();
private:
	QScopedPointer<AvahiServiceAdvertiser> advertiser_;
	QQmlApplicationEngine engine_;
	QObject * rootObject_;
};

#endif

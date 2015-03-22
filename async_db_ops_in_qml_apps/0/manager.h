/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef MANAGER_H_
#define MANAGER_H_

#include <QQmlApplicationEngine>

class Manager: public QObject {
	Q_OBJECT
public:
	Manager();
private:
	void loadQml();
private:
	QQmlApplicationEngine engine_;
	QObject * rootObject_;
};

#endif

/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef MANAGER_H_
#define MANAGER_H_

#include <QQmlApplicationEngine>
#include "facade.h"

class Manager: public QObject {
	Q_OBJECT
public:
	Manager();
public slots:
	void onSearchButtonPressed(const QString & theSearchPhrase);
private:
	void loadQml();
	void connectToDatabase();
private:
	QQmlApplicationEngine engine_;
	QObject * rootObject_;
	ConnectionPtr connection_;
};

#endif

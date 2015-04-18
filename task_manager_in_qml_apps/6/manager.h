/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef MANAGER_H_
#define MANAGER_H_

#include <QQmlApplicationEngine>
#include "taskmanager.h"

class Manager: public QObject {
	Q_OBJECT
public:
	Manager();
public slots:
	void onProcessRandomData();
private:
	Q_INVOKABLE TaskManager::TaskResult taskGenerateRandomData();
	Q_INVOKABLE TaskManager::TaskResult taskShowRandomData();
private:
	void loadQml();
private:
	QQmlApplicationEngine engine_;
	QObject * rootObject_;
	TaskManager tm_;
	typedef TaskManager::ScenarioPtr ScenarioPtr;
	typedef TaskManager::TaskPtr TaskPtr;
	typedef TaskManager::Task::Branch Branch;
	int randomData_ = 0;
};

#endif

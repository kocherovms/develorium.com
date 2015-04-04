/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef MANAGER_H_
#define MANAGER_H_

#include <QQmlApplicationEngine>
#include <QMessageBox>
#include <QFutureWatcher>
#include "taskmanager.h"

class Manager: public QObject {
	Q_OBJECT
public:
	Manager();
public slots:
	void onAsyncOperationWithNotification();
private:
	Q_INVOKABLE TaskManager::TaskResult taskShowNotification();
	Q_INVOKABLE TaskManager::TaskResult taskPerformAsyncOperations();
	Q_INVOKABLE TaskManager::TaskResult taskWaitForAsyncOperations();
	Q_INVOKABLE TaskManager::TaskResult taskHideNotification();
private:
	void loadQml();
private:
	QQmlApplicationEngine engine_;
	QObject * rootObject_;
	TaskManager tm_;
	typedef TaskManager::ScenarioPtr ScenarioPtr;
	typedef TaskManager::TaskPtr TaskPtr;
	typedef TaskManager::Task::Branch Branch;
	int scenariosCounter_ = 0;
	QMessageBox messageBox_;
	QFutureWatcher<void> fw1_, fw2_;
};

#endif

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
	void onSlowOperation();
	void onLongOperationsChain();
private:
	Q_INVOKABLE TaskManager::TaskResult taskSlowOperation();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain0();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain1();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain2();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain3();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain4();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain5();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain6();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain7();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain8();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain9();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain10();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain11();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain12();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain13();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain14();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain15();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain16();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain17();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain18();
	Q_INVOKABLE TaskManager::TaskResult taskLongOperationsChain19();
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
};

#endif

/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include <stdexcept>
#include <unistd.h>
#include <QDebug>
#include <QQuickWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QThread>
#include <QtConcurrent>
#include <QFutureWatcher>
#include "manager.h"

namespace {

struct Operation {
	Operation(const char * theName, const int theDuration): name(theName) {
		qDebug() << name << "begin";
		QThread::msleep(theDuration);
	}
	~Operation() {
		qDebug() << name << "end";
	}
	QString name;
};

}

/* Manager */

Manager::Manager(): rootObject_(0) {
	loadQml();
	QQuickWindow & window = dynamic_cast<QQuickWindow &>(*rootObject_);
	const QPoint screenCenter = QApplication::desktop()->screen()->rect().center();
	window.setX(screenCenter.x() - window.width() / 2);
	window.setY(screenCenter.y() - window.height() / 2);
	window.show();
}

void Manager::onSlowOperation() {
	ScenarioPtr s = tm_.addScenario(QString("Slow operation #%1").arg(++scenariosCounter_));
	TaskPtr t = s->getRootTask();
	t->addNextTask(this, "taskSlowOperation", QVariantList());
}

static void execSlowOperation() {
	Operation op(__FUNCTION__, 500);
}

TaskManager::TaskResult Manager::taskSlowOperation() {
	QFutureWatcher<void> fw;
	QEventLoop loop;
	connect(&fw, SIGNAL(finished()), &loop, SLOT(quit()));
	fw.setFuture(QtConcurrent::run(execSlowOperation));
	loop.exec();
	return TaskManager::Success;
}

void Manager::onLongOperationsChain() {
	ScenarioPtr s = tm_.addScenario(QString("Long operations chain #%1").arg(++scenariosCounter_));
	s->getRootTask()->
		addNextTask(this, "taskLongOperationsChain0", QVariantList())->
		addNextTask(this, "taskLongOperationsChain1", QVariantList())->
		addNextTask(this, "taskLongOperationsChain2", QVariantList())->
		addNextTask(this, "taskLongOperationsChain3", QVariantList())->
		addNextTask(this, "taskLongOperationsChain4", QVariantList())->
		addNextTask(this, "taskLongOperationsChain5", QVariantList())->
		addNextTask(this, "taskLongOperationsChain6", QVariantList())->
		addNextTask(this, "taskLongOperationsChain7", QVariantList())->
		addNextTask(this, "taskLongOperationsChain8", QVariantList())->
		addNextTask(this, "taskLongOperationsChain9", QVariantList())->
		addNextTask(this, "taskLongOperationsChain10", QVariantList())->
		addNextTask(this, "taskLongOperationsChain11", QVariantList())->
		addNextTask(this, "taskLongOperationsChain12", QVariantList())->
		addNextTask(this, "taskLongOperationsChain13", QVariantList())->
		addNextTask(this, "taskLongOperationsChain14", QVariantList())->
		addNextTask(this, "taskLongOperationsChain15", QVariantList())->
		addNextTask(this, "taskLongOperationsChain16", QVariantList())->
		addNextTask(this, "taskLongOperationsChain17", QVariantList())->
		addNextTask(this, "taskLongOperationsChain18", QVariantList())->
		addNextTask(this, "taskLongOperationsChain19", QVariantList());
}

TaskManager::TaskResult Manager::taskLongOperationsChain0() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain1() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain2() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain3() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain4() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain5() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain6() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain7() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain8() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain9() {
	Operation op(__FUNCTION__, 20);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain10() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain11() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain12() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain13() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain14() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain15() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain16() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain17() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain18() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskLongOperationsChain19() {
	Operation op(__FUNCTION__, 15);
	return TaskManager::Success;
}

void Manager::loadQml() {
	engine_.load(QUrl("qrc:/main.qml"));
	const QList<QObject *> ro = engine_.rootObjects();
	
	if(ro.isEmpty())
		throw std::runtime_error("No root objects in QML");

	rootObject_ = ro[0];
	QObject::connect(rootObject_, SIGNAL(slowOperation()), this, SLOT(onSlowOperation()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain()));
}


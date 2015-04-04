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
	Operation(const QString & theName, const int theDuration): name(theName) {
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

void Manager::onAsyncOperationWithNotification() {
	ScenarioPtr s = tm_.addScenario(QString("Async operation with notification #%1").arg(++scenariosCounter_));
	s->getRootTask()->
		addNextTask(this, "taskShowNotification", QVariantList())->
		addNextTask(this, "taskPerformAsyncOperations", QVariantList())->
		addNextTask(this, "taskWaitForAsyncOperations", QVariantList())->
		addNextTask(this, "taskHideNotification", QVariantList());
}

TaskManager::TaskResult Manager::taskShowNotification() {
	messageBox_.setText("Please, wait until long running operations are complete");
	messageBox_.setStandardButtons(QMessageBox::NoButton);
	messageBox_.show();
	return TaskManager::Success;
}

static void execSlowOperation(const QString theOperationName, const int theDuration) {
	Operation op(theOperationName, theDuration);
}

TaskManager::TaskResult Manager::taskPerformAsyncOperations() {
	fw1_.setFuture(QtConcurrent::run(execSlowOperation, QString("Async op #1"), 2000 + (qrand() % 2000)));
	fw2_.setFuture(QtConcurrent::run(execSlowOperation, QString("Async op #2"), 2000 + (qrand() % 2000)));
	return TaskManager::Success;
}

TaskManager::TaskResult Manager::taskWaitForAsyncOperations() {
	return fw1_.isRunning() || fw2_.isRunning()
		? TaskManager::NeedToWait
		: TaskManager::Success;
}

TaskManager::TaskResult Manager::taskHideNotification() {
	messageBox_.hide();
	return TaskManager::Success;
}

void Manager::loadQml() {
	qsrand(time(0));
	engine_.load(QUrl("qrc:/main.qml"));
	const QList<QObject *> ro = engine_.rootObjects();
	
	if(ro.isEmpty())
		throw std::runtime_error("No root objects in QML");

	rootObject_ = ro[0];
	QObject::connect(rootObject_, SIGNAL(asyncOperationWithNotification()), this, SLOT(onAsyncOperationWithNotification()));
	QObject::connect(&fw1_, SIGNAL(finished()), &tm_, SLOT(invokeProcessing()));
	QObject::connect(&fw2_, SIGNAL(finished()), &tm_, SLOT(invokeProcessing()));
}

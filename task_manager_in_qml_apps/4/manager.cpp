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

class Counter: public QObject {
	Q_OBJECT
public:
	Counter(const int theValue): value(theValue) {}
public slots:
	void dropDown() {
		if(!--value) 
			emit done();
	}
signals:
	void done();
private:
	int value;
};

#include "manager.moc"

TaskManager::TaskResult Manager::taskPerformAsyncOperations() {
	QFutureWatcher<void> fw1, fw2;
	Counter counter(2);
	QEventLoop loop;
	connect(&fw1, SIGNAL(finished()), &counter, SLOT(dropDown()));
	connect(&fw2, SIGNAL(finished()), &counter, SLOT(dropDown()));
	connect(&counter, SIGNAL(done()), &loop, SLOT(quit()));
	fw1.setFuture(QtConcurrent::run(execSlowOperation, QString("Async op #1"), 2000 + (qrand() % 2000)));
	fw2.setFuture(QtConcurrent::run(execSlowOperation, QString("Async op #2"), 2000 + (qrand() % 2000)));
	loop.exec();
	return TaskManager::Success;
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
}

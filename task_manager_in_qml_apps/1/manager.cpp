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

static void execSlowOperation() {
	Operation op(__FUNCTION__, 500);
}

void Manager::onSlowOperation() {
	QFutureWatcher<void> fw;
	QEventLoop loop;
	connect(&fw, SIGNAL(finished()), &loop, SLOT(quit()));
	fw.setFuture(QtConcurrent::run(execSlowOperation));
	loop.exec();
}

void Manager::onLongOperationsChain0() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain1() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain2() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain3() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain4() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain5() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain6() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain7() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain8() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain9() {
	Operation op(__FUNCTION__, 20);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain10() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain11() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain12() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain13() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain14() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain15() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain16() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain17() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain18() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::onLongOperationsChain19() {
	Operation op(__FUNCTION__, 15);
	QCoreApplication::processEvents();
}

void Manager::loadQml() {
	engine_.load(QUrl("qrc:/main.qml"));
	const QList<QObject *> ro = engine_.rootObjects();
	
	if(ro.isEmpty())
		throw std::runtime_error("No root objects in QML");

	rootObject_ = ro[0];
	QObject::connect(rootObject_, SIGNAL(slowOperation()), this, SLOT(onSlowOperation()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain0()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain1()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain2()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain3()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain4()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain5()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain6()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain7()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain8()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain9()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain10()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain11()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain12()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain13()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain14()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain15()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain16()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain17()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain18()));
	QObject::connect(rootObject_, SIGNAL(longOperationsChain()), this, SLOT(onLongOperationsChain19()));
}


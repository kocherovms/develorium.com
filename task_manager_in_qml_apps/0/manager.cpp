#include <stdexcept>
#include <time.h>
#include <unistd.h>
#include <QDebug>
#include <QQuickWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QThread>
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
	qsrand(time(0));
	loadQml();
	QQuickWindow & window = dynamic_cast<QQuickWindow &>(*rootObject_);
	const QPoint screenCenter = QApplication::desktop()->screen()->rect().center();
	window.setX(screenCenter.x() - window.width() / 2);
	window.setY(screenCenter.y() - window.height() / 2);
	window.show();
}

void Manager::onSlowOperation() {
	Operation op(__FUNCTION__, 3000);
}

void Manager::onLongOperationsChain0() {
	Operation op(__FUNCTION__, qrand() % 300);
}

void Manager::onLongOperationsChain1() {
	Operation op(__FUNCTION__, qrand() % 300);
}

void Manager::onLongOperationsChain2() {
	Operation op(__FUNCTION__, qrand() % 300);
}

void Manager::onLongOperationsChain3() {
	Operation op(__FUNCTION__, qrand() % 300);
}

void Manager::onLongOperationsChain4() {
	Operation op(__FUNCTION__, qrand() % 300);
}

void Manager::onLongOperationsChain5() {
	Operation op(__FUNCTION__, qrand() % 300);
}

void Manager::onLongOperationsChain6() {
	Operation op(__FUNCTION__, qrand() % 300);
}

void Manager::onLongOperationsChain7() {
	Operation op(__FUNCTION__, qrand() % 300);
}

void Manager::onLongOperationsChain8() {
	Operation op(__FUNCTION__, qrand() % 300);
}

void Manager::onLongOperationsChain9() {
	Operation op(__FUNCTION__, qrand() % 300);
}

void Manager::loadQml() {
	engine_.load(QUrl::fromLocalFile("main.qml"));
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
}


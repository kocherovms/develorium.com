/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include <stdexcept>
#include <QQuickWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include "manager.h"

/* Manager */

Manager::Manager(): rootObject_(0) {
	loadQml();
	QQuickWindow & window = dynamic_cast<QQuickWindow &>(*rootObject_);
	const QPoint screenCenter = QApplication::desktop()->screen()->rect().center();
	window.setX(screenCenter.x() - window.width() / 2);
	window.setY(screenCenter.y() - window.height() / 2);
	window.show();
}

void Manager::onProcessRandomData() {
	ScenarioPtr s = tm_.addScenario("Random data");
	s->getRootTask()->
		addNextTask(this, "taskGenerateRandomData", QVariantList(), TaskManager::GeneratedValueNames() << "randomData")->
		addNextTask(this, "taskShowRandomData", QVariantList() << TaskManager::DynArg("randomData"));
}

TaskManager::TaskResult Manager::taskGenerateRandomData() {
	QVariantHash gv;
	gv["randomData"] = qrand() % std::numeric_limits<int>::max();
	return TaskManager::TaskResult(TaskManager::TaskResult::Success, gv);
}

TaskManager::TaskResult Manager::taskShowRandomData(const QVariant theRandomData) {
	QMessageBox messageBox;
	messageBox.setText(QString("Random data is %1").arg(theRandomData.toInt()));
	messageBox.setStandardButtons(QMessageBox::Ok);
	messageBox.exec();
	return TaskManager::TaskResult::Success;
}

void Manager::loadQml() {
	qsrand(time(0));
	engine_.load(QUrl("qrc:/main.qml"));
	const QList<QObject *> ro = engine_.rootObjects();
	
	if(ro.isEmpty())
		throw std::runtime_error("No root objects in QML");

	rootObject_ = ro[0];
	QObject::connect(rootObject_, SIGNAL(processRandomData()), this, SLOT(onProcessRandomData()));
}

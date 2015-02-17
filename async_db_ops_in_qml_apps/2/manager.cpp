#include <stdexcept>
#include <QQuickWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QEventLoop>
#include <QDebug>
#include "manager.h"

Manager::Manager(): rootObject_(0) {
	loadQml();
	connectToDatabase();
	QQuickWindow & window = dynamic_cast<QQuickWindow &>(*rootObject_);
	const QPoint screenCenter = QApplication::desktop()->screen()->rect().center();
	window.setX(screenCenter.x() - window.width() / 2);
	window.setY(screenCenter.y() - window.height() / 2);
	window.show();
}

void Manager::onSearchButtonPressed(const QString & theSearchPhrase) {
	qDebug() << __FUNCTION__ << "started";
	QFutureWatcher<Facade::SearchResults> fw;
	QEventLoop loop;
	connect(&fw, SIGNAL(finished()), &loop, SLOT(quit()));
	Facade facade(connection_);
	fw.setFuture(QtConcurrent::run(&facade, &Facade::search, theSearchPhrase));
	loop.exec(QEventLoop::ExcludeUserInputEvents);
	fw.result();
	qDebug() << __FUNCTION__ << "finished";
}

void Manager::loadQml() {
	engine_.load(QUrl::fromLocalFile("main.qml"));
	const QList<QObject *> ro = engine_.rootObjects();
	
	if(ro.isEmpty())
		throw std::runtime_error("No root objects in QML");

	rootObject_ = ro[0];
	QObject::connect(rootObject_, SIGNAL(searchButtonPressed(QString)), this, SLOT(onSearchButtonPressed(QString)));
}

void Manager::connectToDatabase() {
	// Here must be the code of real connecting to DB (e.g. to PostgreSQL)
	connection_.reset(new Connection);
}


#include <stdexcept>
#include <QQuickWindow>
#include <QCoreApplication>
#include "manager.h"

Manager::Manager(): rootObject_(0) {
	loadQml();
	connectToDatabase();
	QQuickWindow & window = dynamic_cast<QQuickWindow &>(*rootObject_);
	window.show();
}

void Manager::onSearchButtonPressed(const QString & theSearchPhrase) {
	Facade facade(connection_);
	const Facade::SearchResults results = facade.search(theSearchPhrase);
	Q_UNUSED(results);
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


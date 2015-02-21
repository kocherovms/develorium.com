#include <stdexcept>
#include <QQuickWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include "searchfacade.h"
#include "statisticsfacade.h"
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
	SearchFacade facade(connection_);
	const SearchFacade::SearchResults results = facade.asyncCall(&SearchFacade::search, theSearchPhrase);
	Q_UNUSED(results);
	qDebug() << __FUNCTION__ << "finished";
}

void Manager::onStatisticsButtonPressed() {
	qDebug() << __FUNCTION__ << "started";
	StatisticsFacade facade(connection_);
	facade.asyncCall(&StatisticsFacade::putIntoStatistics, StatisticsFacade::StatisticsItem());
	qDebug() << __FUNCTION__ << "finished";
}

void Manager::loadQml() {
	engine_.load(QUrl::fromLocalFile("main.qml"));
	const QList<QObject *> ro = engine_.rootObjects();
	
	if(ro.isEmpty())
		throw std::runtime_error("No root objects in QML");

	rootObject_ = ro[0];
	QObject::connect(rootObject_, SIGNAL(searchButtonPressed(QString)), this, SLOT(onSearchButtonPressed(QString)));
	QObject::connect(rootObject_, SIGNAL(statisticsButtonPressed()), this, SLOT(onStatisticsButtonPressed()));
}

void Manager::connectToDatabase() {
	// Here must be the code of real connecting to DB (e.g. to PostgreSQL)
	connection_.reset(new Connection);
}


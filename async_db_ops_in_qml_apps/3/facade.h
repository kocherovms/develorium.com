/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef FACADE_H_
#define FACADE_H_

#include <QSharedPointer>
#include <QFutureWatcher>
#include <QEventLoop>
#include <QtConcurrent>

// Abstraction of database connection
class Connection {
};

typedef QSharedPointer<Connection> ConnectionPtr;

template<class RetType>
RetType extractResult(QFutureWatcher<RetType> & theFw) { return theFw.result(); }

template<>
void extractResult(QFutureWatcher<void> & theFw);

class Facade {
public:
	Facade(ConnectionPtr & theConnection): connection_(theConnection) {}
	virtual ~Facade() {}
	struct SearchResults {
		// ... domain specific data
	};
	struct StatisticsItem {
		// ... domain specific data
	};
	SearchResults search(const QString & theSearchPhrase);
	void putIntoStatistics(const StatisticsItem & theItem);
	template <class RetType, class T, class Param1, class Arg1> 
	RetType asyncCall(RetType (T::*theMethod)(Param1), const Arg1 & theArg1) {
		QFutureWatcher<RetType> fw;
		QEventLoop loop;
		QObject::connect(&fw, SIGNAL(finished()), &loop, SLOT(quit()));
		fw.setFuture(QtConcurrent::run(&dynamic_cast<T &>(*this), theMethod, theArg1));
		loop.exec(QEventLoop::ExcludeUserInputEvents);
		return extractResult(fw);
	}
protected:
	ConnectionPtr connection_;
};

#endif

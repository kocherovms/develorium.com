#ifndef FACADE_H_
#define FACADE_H_

#include <QSharedPointer>

// Abstraction of database connection
class Connection {
};

typedef QSharedPointer<Connection> ConnectionPtr;

class Facade {
public:
	Facade(ConnectionPtr & theConnection): connection_(theConnection) {}
	struct SearchResults {
		// ... domain specific data
	};
	SearchResults search(const QString & theSearchPhrase);
private:
	ConnectionPtr connection_;
};

#endif

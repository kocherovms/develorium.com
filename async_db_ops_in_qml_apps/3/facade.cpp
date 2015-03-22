/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include <unistd.h>
#include "facade.h"

template<>
void extractResult(QFutureWatcher<void> & theFw) { 
	theFw.waitForFinished(); 
}

Facade::SearchResults Facade::search(const QString & theSearchPhrase) {
	Q_UNUSED(theSearchPhrase);
	// Emulate some heavy SQL(s) here
	sleep(3);
	return SearchResults();
}

void Facade::putIntoStatistics(const StatisticsItem & theItem) {
	Q_UNUSED(theItem);
	// Emulate some heavy SQL(s) here
	sleep(3);
}

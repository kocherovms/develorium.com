/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include <unistd.h>
#include "facade.h"

Facade::SearchResults Facade::search(const QString & theSearchPhrase) {
	Q_UNUSED(theSearchPhrase);
	// Emulate some heavy SQL(s) here
	sleep(3);
	return SearchResults();
}

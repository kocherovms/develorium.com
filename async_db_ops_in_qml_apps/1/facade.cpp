#include <unistd.h>
#include "facade.h"

Facade::SearchResults Facade::search(const QString & theSearchPhrase) {
	Q_UNUSED(theSearchPhrase);
	sleep(3);
	return SearchResults();
}

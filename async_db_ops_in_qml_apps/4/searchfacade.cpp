#include <unistd.h>
#include "searchfacade.h"

SearchFacade::SearchResults SearchFacade::search(const QString & theSearchPhrase) {
	Q_UNUSED(theSearchPhrase);
	// Emulate some heavy SQL(s) here
	sleep(3);
	return SearchResults();
}

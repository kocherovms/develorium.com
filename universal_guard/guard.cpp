/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/
#include <functional>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

class Guard {
public:
	Guard(const std::function<void ()> & theRelease): release_(theRelease) {
	}
	~Guard() {
		release();
	}
	void release() {
		if(isAlreadyReleased_)
			return;

		release_();
		isAlreadyReleased_ = true;
	}
private:
	std::function<void ()> release_;
	bool isAlreadyReleased_ = false;
};

int main(int argc, char * argv[]) {
	const char * const FileName = "test.txt";
	const int fd = open(FileName, O_CREAT | O_TRUNC, 0666);

	if(fd == -1) {
		printf("Failed to open \"%s\": %s\n", FileName, strerror(errno));
		return 1;
	}

	printf("File \"%s\" opened\n", FileName);

	Guard g = { [ fd, FileName ]() {
		::close(fd);
		printf("File \"%s\" closed\n", FileName);
	} };
	// Using fd somehow with guaranteed ::close
	return 0;
}

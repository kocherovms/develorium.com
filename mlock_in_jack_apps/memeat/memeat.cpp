/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include <sys/select.h>
#include <string.h>
#include <iostream>

static bool waitForQuit() {
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(0, &rfds); // wait for an event on stdin
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000 * 10; // 10 ms
	return select(1, &rfds, 0, 0, &tv) > 0;
}

int main(int argc, char * argv[]) {
	enum { BufferSize = 1024 * 1024 * 1024, WindowSize = 4096 };
	char * buffer = new char[BufferSize];
	int offset = 0;

	while(!waitForQuit()) {
		for(int i = 0; i < 100; ++i) {
			const char * const HelloWorld = "Hello, world!";
			char * ptr = buffer;
			ptr += offset;
			memcpy(ptr, HelloWorld, strlen(HelloWorld));
			offset += WindowSize;

			if(offset + WindowSize > BufferSize)
				offset = 0;
		}
	}

	delete [] buffer;
	return 0;
}

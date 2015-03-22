/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include <stdexcept>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <jack/jack.h>
#include <iostream>

class Client {
public:
	Client(): buffer_(0), offset_(0), jackClient_(0), jackPort_(0) {
		try {
			init();
		} catch(...) {
			release();
			throw;
		}
	}
	~Client() {
		release();
	}
private:
	enum { BufferSize = 1014 * 1024 * 16 };
	void init() {
		buffer_ = new char[BufferSize];
		
		if(mlock(buffer_, BufferSize))
			throw std::runtime_error(std::string("Failed to lock memory: ") + strerror(errno));

		const int pageSize = getpagesize();
		char * ptr = buffer_;
		// Touch every locked page to avoid COW (copy-on-write) faults
		for(int i = 0; i < BufferSize; i += pageSize, ptr += pageSize)
			*ptr = 0;

		jack_status_t status;
		jackClient_ = jack_client_open("Test client", JackNullOption, &status);

		if(!jackClient_)
			throw std::runtime_error("Failed to create JACK client");

		if(jack_set_process_callback(jackClient_, processCallback, this))
			throw std::runtime_error("Failed to set process callback function");

		const std::string PortName = "input";
		jackPort_ = jack_port_register(jackClient_, PortName.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

		if(!jackPort_)
			throw std::runtime_error("Failed to register port");

		if(jack_activate(jackClient_))
			throw std::runtime_error("Failed to activate JACK client");

		const std::string t = std::string(jack_get_client_name(jackClient_)) + ":" + PortName;

		if(jack_connect(jackClient_, "system:capture_1", t.c_str()))
			throw std::runtime_error("Failed to connect capture port to input port");
	}
	void release() {
 		if(jackClient_)
			jack_client_close(jackClient_);

		if(buffer_)
			munlock(buffer_, BufferSize);

		delete [] buffer_;
	}
	int processCallback_impl(const int theFramesCount) {
		const int bytesCount = theFramesCount * sizeof(float); // JACK processes data in 32-bit float
		void * captureData = jack_port_get_buffer(jackPort_, theFramesCount);

		if(captureData) {
			int bytesToCopy = BufferSize - offset_;
			bytesToCopy = bytesToCopy >= bytesCount ? bytesCount : bytesToCopy;
			char * sink = buffer_;
			sink += offset_;
			memcpy(sink, reinterpret_cast<const char *>(captureData), bytesToCopy);
			offset_ += bytesToCopy;

			if(bytesCount > bytesToCopy) {
				const char * source = reinterpret_cast<const char *>(captureData);
				source += bytesToCopy;
				bytesToCopy = bytesCount - bytesToCopy;
				memcpy(buffer_, source, bytesToCopy);
				offset_ = bytesToCopy;
			}
		}

		return 0;
	}
	static int processCallback(const jack_nframes_t theFramesCount, void * theOpaque) {
		Client * client = reinterpret_cast<Client *>(theOpaque);
		assert(client);
		return client->processCallback_impl(theFramesCount);
	}
private:
	char * buffer_;
	int offset_;
	jack_client_t * jackClient_;
	jack_port_t * jackPort_;
};

int main(int argc, char * argv[]) {
	try {
		Client client[10];
		(void)client;
		std::cout << "Press Enter or Ctrl + D to quit" << std::endl;
		std::cin.get();
		return 0;
	} catch(const std::exception & e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}

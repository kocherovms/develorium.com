#include <stdexcept>
#include <jack/jack.h>
#include <iostream>
#include <unistd.h>

class Client {
public:
	Client(): buffer_(0), payload_(0), jackClient_(0), jackPort_(0) {
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
	enum { BufferSize = 1 << 27 };
	void init() {
		buffer_ = new char[BufferSize];

		jack_status_t status;
        jackClient_ = jack_client_open("Test client", static_cast<jack_options_t>(JackUseExactName), &status);

		if(!jackClient_)
			throw std::runtime_error("Failed to create JACK client");

		if(jack_set_process_callback(jackClient_, processCallback, this))
			throw std::runtime_error("Failed to set process callback function");

		jackPort_ = jack_port_register(jackClient_, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

		if(!jackPort_)
			throw std::runtime_error("Failed to register port");

		if(jack_activate(jackClient_))
			throw std::runtime_error("Failed to activate JACK client");
	}
	void release() {
		delete [] buffer_;

		if(jackClient_)
			jack_client_close(jackClient_);
	}
	static int processCallback(jack_nframes_t theFramesCount, void * theOpaque) {
		return 0;
	}
private:
	char * buffer_;
	int payload_;
	jack_client_t * jackClient_;
	jack_port_t * jackPort_;
};

int main(int argc, char * argv[]) {
	try {
		Client client;
		std::cout << "Press [Ctrl + D] to quit" << std::endl;
		std::cin.get();
	} catch(const std::exception & e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}

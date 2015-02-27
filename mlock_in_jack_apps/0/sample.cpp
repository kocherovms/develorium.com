#include <stdexcept>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <jack/jack.h>
#include <iostream>

class Client {
public:
	Client(): buffer_(0), payload_(0), jackClient_(0), jackPort_(0) {
		pipeFds[0] = pipeFds[1] = -1;

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
	int getTermFd() const {
		return pipeFds[0];
	}
private:
	enum { BufferSize = 1 << 27 };
	void init() {
		buffer_ = new char[BufferSize];

		if(pipe(pipeFds))
			throw std::runtime_error("Failed to create communication pipe");

		if(fcntl(pipeFds[1], F_SETFL, O_NONBLOCK)) // set non-blocking mode in order to avoid need of reading from pipeFds[0]
			throw std::runtime_error("Failed to set non-blocking mode for write end of communication pipe");

		jack_status_t status;
		const std::string ClientName = "Test client";
        jackClient_ = jack_client_open(ClientName.c_str(), static_cast<jack_options_t>(JackUseExactName), &status);

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

		const std::string t = ClientName + ":" + PortName;

		if(jack_connect(jackClient_, "system:capture_1", t.c_str()))
			throw std::runtime_error("Failed to connect capture port to input port");
	}
	void release() {
 		if(jackClient_)
			jack_client_close(jackClient_);

		close(pipeFds[0]);
		close(pipeFds[1]);

		delete [] buffer_;
	}
	int processCallback_impl(const int theFramesCount) {
		const int bytesCount = theFramesCount * sizeof(float); // JACK processes data in 32-bit float

		if(payload_ + bytesCount > BufferSize) {
			write(pipeFds[1], "q", 1);
			return 0; // indicate stop for JACK
		}

		void * captureData = jack_port_get_buffer(jackPort_, theFramesCount);

		if(captureData) {
			char * sink = buffer_;
			sink += payload_;
			memcpy(sink, reinterpret_cast<const char *>(captureData), bytesCount);
			payload_ += bytesCount;
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
	int payload_;
	int pipeFds[2];
	jack_client_t * jackClient_;
	jack_port_t * jackPort_;
};

int main(int argc, char * argv[]) {
	try {
		Client client;
		std::cout << "Press [Ctrl + D] or Enter to quit" << std::endl;
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(0, &rfds); // wait for an event on stdin
		FD_SET(client.getTermFd(), &rfds); // wait for termination event on client
		select(client.getTermFd() + 1, &rfds, 0, 0, 0);
		return 0;
	} catch(const std::exception & e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}

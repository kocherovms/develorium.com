JACK client which creates 10 input ports and maintains 16Mb round robin buffer for each port. 
Program requires JACK devel package to be installed. On Ubuntu: sudo apt-get install libjack-jackd2-dev.
Compile with: g++ -Wall sample.cpp -ljack
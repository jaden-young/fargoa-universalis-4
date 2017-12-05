all: serial

serial: serial.cpp
	g++ -w -O3 -std=c++11 serial.cpp -o serial

#pragma once
#ifndef BASE_H
#define BASE_H

class Config;
class NetworkCommunication;
class Handle;

class Base {
public:
	static Config& config();
	static NetworkCommunication& network();
	static Handle& handle();

private:
	static Config config_;
	static NetworkCommunication network_;
	static Handle handle_;
};

#endif
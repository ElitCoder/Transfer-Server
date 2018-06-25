#pragma once
#ifndef BASE_H
#define BASE_H

class Config;
class NetworkCommunication;
class Handle;

class Base {
public:
	static Config& settings();
	static NetworkCommunication& network();
	static Handle& handle();
	
private:
	static Config settings_;
	static NetworkCommunication network_;
	static Handle handle_;
};

#endif
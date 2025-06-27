#pragma once

#include <iostream>
#include <list>

class FileQueueItem
{
	std::string path;
	bool Arcived;
public:
	FileQueueItem(std::string p);
	std::string Path();
};

class FileQueue
{
	std::string path;
	std::string qname() const { return path + "\\_queue.csv"; };
	std::string terminal() const { return ":eof"; };
	std::list<FileQueueItem> files;
public:
	FileQueue();
	FileQueue(std::string p);
	void AddItem(std::string fn);
	int Count();
	std::string GetFront();
	void PopFront();
	// Stores queue to disc
	void Store();
	void Load();
	// Remove queue file from disc
	void Remove();
	bool IsClosed();
};

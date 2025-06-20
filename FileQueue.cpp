#include "FileQueue.h"
#include <filesystem>
#include <stdio.h>
#include <fstream>
#include <string>

FileQueue::FileQueue()
{
	//count = 0;
}

FileQueue::FileQueue(std::string p)
{
	path = p;
	//count = 0;
}

void FileQueue::AddItem(std::string fn)
{
	{
		FileQueueItem* newFile;
		newFile = new FileQueueItem(fn);

		files.push_back(*newFile);
	}
}

int FileQueue::Count()
{
	return files.size();
}

std::string FileQueue::GetFront()
{
	//std::string path = files.front().Path();
	//files.pop_front();

	//return path;

	return files.front().Path();
}

void FileQueue::PopFront()
{
	files.pop_front();
}

void FileQueue::Store()
{
	using namespace std;
	namespace fs = std::filesystem;

	if (fs::exists(qname()))
		std::cout << "WARN queue file already exists\n";

	list<FileQueueItem>::const_iterator iter;
	ofstream dFile(qname());

	std::cout << "size: " << files.size() << "\n";

	for (auto it = files.begin(); it != files.end(); ++it)
	{
		dFile << (*it).Path() << '\t';
	}

	dFile.close();
}

void FileQueue::Load()
{
	namespace fs = std::filesystem;

	if (fs::exists(qname()))
	{
		std::ifstream sFile(qname());
		std::string line;
		while (std::getline(sFile, line, '\t'))
		{
			AddItem(line);
			//std::cout << line << std::endl;
		}

		sFile.close();
	}
	else
	{
		std::cout << "WARN " +qname() +" does not exist\n";
	}
}

void FileQueue::Remove()
{
	if (std::filesystem::exists(qname()))
		remove(qname().c_str());
}


// FileQueItem

FileQueueItem::FileQueueItem(std::string p)
{
	Arcived = 0;
	path = p;
}

std::string FileQueueItem::Path()
{
	return path;
}

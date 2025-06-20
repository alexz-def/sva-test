//
// Packer.cpp
//
#include <iostream>
#include "..\FileQueue.h"
#include "zlib\zlib.h"
#include <Windows.h>
#include <thread>
#include <filesystem>
#include <mutex>

bool compress_file(const std::string& input_filename, const std::string& output_filename);
void compress_wrapper(std::string sfName);
void thd_func(int thd_id);

std::mutex mtx;
FileQueue queue;

int main(int argc, char* argv[])
{
    std::string folderName;

    if (argc == 1)
    {
        std::cout << "Input folder name:\n";
        std::cin >> folderName;
    }
    else
    {
        folderName = argv[1];
    }

    if (!std::filesystem::exists(folderName))
    {
        std::cout << folderName << " does not exist!";
        return 0;
    }
    if (!std::filesystem::exists(folderName +"\\_queue.csv"))
    {
        std::cout << "Queue file does not exist!";
        return 0;
    }

    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    //std::cout << "NumProc: " << sysinfo.dwNumberOfProcessors << std::endl;
    std::cout << "NumThds: " << std::thread::hardware_concurrency() << std::endl;

    queue = FileQueue(folderName);
    queue.Load();
    std::cout << "Files: " << queue.Count() << std::endl;

    std::thread thd[4];

    //int tCount = 4;
    int tCount = std::thread::hardware_concurrency();

    for (int i = 1; i <= tCount; i++)
    {
        std::cout << "starting thread[" << i << "]...\n";
        thd[i - 1] = std::thread(thd_func, i);
    }

    for (int i = 1; i <= tCount; i++)
    {
        thd[i-1].join();
        //std::cout << "thread[" << i << "] finished\n";
    }

    queue.Remove();

    std::cout << "Completed successfully";
}

void compress_wrapper(std::string fName)
{
    if (std::filesystem::exists(fName))
    {
        if (compress_file(fName, fName + ".gz"))
        {
            std::cout << "compressed\n";
        }
        else
        {
            std::cout << "Ooops!\n";
        }
    }
    else
        std::cout << "File not found!\n";
}

void thd_func(int thd_id)
{
    std::string fn;
    bool haveItems = false;
    int cnt = 0;

    do
    {
        mtx.lock();

        haveItems = queue.Count() > 0;
        if (haveItems)
        {
            fn = queue.GetFront();
            queue.PopFront();
            cnt++;

            std::cout << "thd[" << thd_id << "]: " << fn << "\n";
        }

        mtx.unlock();

        if (haveItems)
            compress_wrapper(fn);
    }
    while (haveItems);

    if (cnt == 0)
        std::cout << "thd[" << thd_id << "]: queue is empty\n";
}

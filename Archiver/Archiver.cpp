//
// Archiver.cpp - interface application.
// Creates a queue-file containing a list of files in the folder and subfolders.
//
#include <iostream>
#include <filesystem>
#include "..\FileQueue.h"

bool buildQuery(std::string topFolderName);

namespace fs = std::filesystem;

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

    if (fs::exists(folderName))
    {
        std::cout << folderName << " exists\nFiles:\n";

        if (buildQuery(folderName))
            std::cout << "Query was built successfully\n";
        else
            std::cout << "WARN Something went wrong!\n";
    }
    else
        std::cout << folderName << " does not exist!";
}

// Read folder directory, build and store query.
bool buildQuery(std::string topFolderName)
{
    FileQueue queue(topFolderName);
    FileQueue folders;

    folders.AddItem(topFolderName);

    while (folders.Count() > 0)
    {
        std::string folderName = folders.GetFront();

        for (const auto& entry : fs::directory_iterator(folderName))
        {
            if (fs::is_regular_file(entry))
            {
                if (entry.path().generic_string().find("_queue.csv") == std::string::npos)
                {
                    std::cout << entry.path() << "\n";
                    queue.AddItem(entry.path().generic_string());
                }
            }
            else
            {
                folders.AddItem(entry.path().generic_string());
            }
        }

        folders.PopFront();
    }

    queue.Store();

    return true;
}

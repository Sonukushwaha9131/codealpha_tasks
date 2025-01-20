#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

void listFiles(const fs::path& path) {
    std::cout << "Files in directory: " << path << std::endl;
    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path().filename().string() << std::endl;
    }
}

void createDirectory(const fs::path& path) {
    if (fs::create_directory(path)) {
        std::cout << "Directory created: " << path << std::endl;
    } else {
        std::cout << "Failed to create directory or it already exists: " << path << std::endl;
    }
}

void copyFile(const fs::path& source, const fs::path& destination) {
    try {
        fs::copy(source, destination);
        std::cout << "File copied from " << source << " to " << destination << std::endl;
    } catch (const fs::filesystem_error& e) {
        std::cout << "Error copying file: " << e.what() << std::endl;
    }
}

void moveFile(const fs::path& source, const fs::path& destination) {
    try {
        fs::rename(source, destination);
        std::cout << "File moved from " << source << " to " << destination << std::endl;
    } catch (const fs::filesystem_error& e) {
        std::cout << "Error moving file: " << e.what() << std::endl;
    }
}

int main() {
    std::string command;
    fs::path currentPath = fs::current_path();

    while (true) {
        std::cout << "Current Directory: " << currentPath << std::endl;
        std::cout << "Enter command (ls, mkdir <name>, cp <source> <destination>, mv <source> <destination>, cd <path>, exit): ";
        std::getline(std::cin, command);

        if (command == "ls") {
            listFiles(currentPath);
        } else if (command.rfind("mkdir ", 0) == 0) {
            std::string dirName = command.substr(6);
            createDirectory(currentPath / dirName);
        } else if (command.rfind("cp ", 0) == 0) {
            std::istringstream iss(command.substr(3));
            std::string source, destination;
            iss >> source >> destination;
            copyFile(currentPath / source, currentPath / destination);
        } else if (command.rfind("mv ", 0) == 0) {
            std::istringstream iss(command.substr(3));
            std::string source, destination;
            iss >> source >> destination;
            moveFile(currentPath / source, currentPath / destination);
        } else if (command.rfind("cd ", 0) == 0) {
            std::string path = command.substr(3);
            fs::path newPath = currentPath / path;
            if (fs::exists(newPath) && fs::is_directory(newPath)) {
                currentPath = newPath;
            } else {
                std::cout << "Directory does not exist: " << newPath << std::endl;
            }
        } else if (command == "exit") {
            break;
        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }

    return 0;
}

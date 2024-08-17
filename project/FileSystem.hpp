#ifndef PROJECT_FILESYSTEM_HPP
#define PROJECT_FILESYSTEM_HPP

#include <iostream>
#include <vector>
#include <string>
#include "Node.hpp"

class FileSystem {
public:
    Node* root;
    Node* currentDir;

    FileSystem() {
        root = new Node("root");
        currentDir = root;
    }

    ~FileSystem() {
        delete root;
    }

    void createFile(const std::string& fileName, int fileSize) const {
        currentDir->addFile(fileName, fileSize);
    }

    void createDirectory(const std::string& dirName) const {
        currentDir->addDirectory(dirName);
    }

    void deleteNode(const std::string& nodeName) const {
        currentDir->deleteNode(nodeName);
    }

    void changeDirectory(const std::string& dirName) {
        if (dirName == "..") {
            if (currentDir->parent != nullptr) {
                currentDir = currentDir->parent;
            }
        } else {
            Node* dir = currentDir->findNode(dirName);
            if (dir != nullptr && !dir->isFile) {
                currentDir = dir;
            } else {
                std::cout << "Directory not found or is a file\n";
            }
        }
    }

    void listContents() const {
        currentDir->listContents();
    }

    void findFile(const std::string& fileName) const {
        Node* file = currentDir->findNode(fileName);
        if (file != nullptr && file->isFile) {
            std::cout << "File found: " << file->name << " (" << file->size << " bytes)\n";
        } else {
            std::cout << "File not found\n";
        }
    }

    void printTree() const {
        root->printTree();
    }
};

#endif //PROJECT_FILESYSTEM_HPP

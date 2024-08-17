#ifndef PROJECT_NODE_HPP
#define PROJECT_NODE_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <string>

class Node {
public:
    std::string name;
    bool isFile;
    int size;
    std::vector<Node*> children;
    Node* parent;

    explicit Node(std::string  name, bool isFile = false, int size = 0)
            : name(std::move(name)), isFile(isFile), size(size), parent(nullptr) {}

    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }

    void addFile(const std::string& fileName, int fileSize) {
        Node* file = new Node(fileName, true, fileSize);
        file->parent = this;
        children.push_back(file);
    }

    void addDirectory(const std::string& dirName) {
        Node* dir = new Node(dirName, false);
        dir->parent = this;
        children.push_back(dir);
    }

    void deleteNode(const std::string& nodeName) {
        auto it = std::find_if(children.begin(), children.end(), [&](Node* child) {
            return child->name == nodeName;
        });
        if (it != children.end()) {
            delete *it;
            children.erase(it);
        }
    }

    Node* findNode(const std::string& nodeName) {
        auto it = std::find_if(children.begin(), children.end(), [&](Node* child) {
            return child->name == nodeName;
        });
        if (it != children.end()) {
            return *it;
        }
        return nullptr;
    }

    void listContents() {
        for (Node* child : children) {
            if (child->isFile) {
                std::cout << "File: " << child->name << " (" << child->size << " bytes)\n";
            } else {
                std::cout << "Dir: " << child->name << "\n";
            }
        }
    }

    void printTree(int depth = 0) {
        for (int i = 0; i < depth; ++i) {
            std::cout << "--";
        }
        std::cout << (isFile ? "File: " : "Dir: ") << name;
        if (isFile) {
            std::cout << " (" << size << " bytes)";
        }
        std::cout << "\n";
        for (Node* child : children) {
            child->printTree(depth + 1);
        }
    }
};

#endif //PROJECT_NODE_HPP

#include "FileSystem.hpp"

class FileSystemTest {
public:
    static void runTests() {
        FileSystem fs;

        fs.createDirectory("home");
        fs.createDirectory("etc");
        fs.createDirectory("var");

        fs.changeDirectory("home");
        fs.createFile("user1.txt", 1000);
        fs.createDirectory("docs");
        fs.changeDirectory("docs");
        fs.createFile("doc1.txt", 2000);
        fs.createFile("doc2.txt", 1500);
        fs.changeDirectory("..");
        fs.changeDirectory("..");

        fs.changeDirectory("etc");
        fs.createFile("config.txt", 500);
        fs.changeDirectory("..");

        fs.changeDirectory("var");
        fs.createDirectory("log");
        fs.changeDirectory("log");
        fs.createFile("logfile.txt", 3000);
        fs.changeDirectory("..");

        fs.changeDirectory("..");

        fs.printTree();
    }
};

void displayMenu() {
    std::cout << "\nMenu:\n";
    std::cout << "1. Create file\n";
    std::cout << "2. Create directory\n";
    std::cout << "3. Delete file/directory\n";
    std::cout << "4. Change directory\n";
    std::cout << "5. List contents\n";
    std::cout << "6. Find file\n";
    std::cout << "7. Print entire file tree\n";
    std::cout << "8. Run test\n";
    std::cout << "9. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    FileSystem fs;
    int choice;
    std::string name;
    int size;

    while (true) {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter file name: ";
                std::cin >> name;
                std::cout << "Enter file size: ";
                std::cin >> size;
                fs.createFile(name, size);
                break;

            case 2:
                std::cout << "Enter directory name: ";
                std::cin >> name;
                fs.createDirectory(name);
                break;

            case 3:
                std::cout << "Enter file/directory name to delete: ";
                std::cin >> name;
                fs.deleteNode(name);
                break;

            case 4:
                std::cout << "Enter directory name to change to or '..' to go up: ";
                std::cin >> name;
                fs.changeDirectory(name);
                break;

            case 5:
                fs.listContents();
                break;

            case 6:
                std::cout << "Enter file name to find: ";
                std::cin >> name;
                fs.findFile(name);
                break;

            case 7:
                fs.printTree();
                break;

            case 8:
                FileSystemTest::runTests();
                break;

            case 9:
                std::cout << "Exiting...\n";
                return 0;

            default:
                std::cout << "Invalid choice, try again.\n";
        }
    }
}

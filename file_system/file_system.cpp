#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>


/*
	Purpose: Represents a file with a name and content.
	Constructor initializes the file’s name.
*/

class File {
public:
    std::string name;
    std::string content;

    File(const std::string& name) : name(name) {}
};

/*
name: Name of the directory (e.g., "docs").
files: Maps file names to File objects.
subDirs: Maps directory names to child Directory objects .
parent: Pointer to parent directory. 
Constructor initializes name and sets parent (for backtracking).
*/

class Directory {
public:
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<File>> files;
    std::unordered_map<std::string, std::shared_ptr<Directory>> subDirs;
    Directory* parent;

    Directory(const std::string& name, Directory* parent = nullptr)
        : name(name), parent(parent) {}
};



class FileSystem {
private:
    std::shared_ptr<Directory> root;   //Since no Directory object is created, no constructor of the Directory class is called.
    Directory* current;

public:
    FileSystem() {
        root = std::make_shared<Directory>("/");  //// Calls Directory constructor
        current = root.get();  //Returns the raw pointer
    }

    void mkdir(const std::string& dirName) {
        if (current->subDirs.count(dirName) == 0) {
            current->subDirs[dirName] = std::make_shared<Directory>(dirName, current);
        }
    }

    void createFile(const std::string& fileName, const std::string& content) {
        current->files[fileName] = std::make_shared<File>(fileName);
        current->files[fileName]->content = content;
    }

    void readFile(const std::string& fileName) {
        if (current->files.count(fileName)) {
            std::cout << current->files[fileName]->content << std::endl;
        } else {
            std::cout << "File not found\n";
        }
    }

    void ls() {
        for (auto& d : current->subDirs) std::cout << " - " << d.first << "\n";
        std::cout << "Files:\n";
        for (auto& f : current->files) std::cout << " - " << f.first << "\n";
    }

    void cd(const std::string& dirName) {
        if (dirName == "..") {
            if (current->parent) current = current->parent;
        } else if (current->subDirs.count(dirName)) {
            current = current->subDirs[dirName].get();
        } else {
            std::cout << "Directory not found\n";
        }
    }

    void pwd() {
        std::vector<std::string> path;
        Directory* temp = current;
        while (temp != nullptr) {
            path.push_back(temp->name);
            temp = temp->parent;
        }
        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            std::cout << *it;
            if (it + 1 != path.rend()) std::cout << "/";
        }
        std::cout << "\n";
    }
};


int main() {
    FileSystem fs;

    fs.mkdir("docs");
    fs.cd("docs");
    fs.ls();
    fs.createFile("hello.txt", "Hello, world!");
    fs.readFile("hello.txt");

    fs.ls();
    fs.pwd();

    fs.cd("..");
    fs.pwd();
}



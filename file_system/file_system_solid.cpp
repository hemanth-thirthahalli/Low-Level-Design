#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Abstract base class for file system entities (Open/Closed Principle)

// The FileSystemEntity abstract base class defines an interface for all file system entities (files, directories, etc.).

class FileSystemEntity {
public:
    virtual std::string getName() const = 0;
    virtual size_t getSize() const = 0;
    virtual void list(int depth = 0) const = 0;
	
    virtual ~FileSystemEntity() = default;
};

// Concrete class for File (Single Responsibility Principle)
class File : public FileSystemEntity {
public:
    File(const std::string& name, size_t size) : name_(name), size_(size) {}

    std::string getName() const override {
        return name_;
    }

    size_t getSize() const override {
        return size_;
    }

    void list(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "File: " << name_ << " (" << size_ << " bytes)" << std::endl;
    }

private:
    std::string name_;
    size_t size_;
};

// Concrete class for Directory (Single Responsibility Principle)
class Directory : public FileSystemEntity {
public:
    Directory(const std::string& name) : name_(name) {}

    std::string getName() const override {
        return name_;
    }

    size_t getSize() const override {
        size_t total = 0;
        for (const auto& entity : entities_) {
            total += entity->getSize();
        }
        return total;
    }

    void list(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "Directory: " << name_ << std::endl;
        for (const auto& entity : entities_) {
            entity->list(depth + 1);
        }
    }

    void addEntity(std::unique_ptr<FileSystemEntity> entity) {
        entities_.push_back(std::move(entity));
    }

private:
    std::string name_;
    std::vector<std::unique_ptr<FileSystemEntity>> entities_;
};

// Interface for file system operations (Interface Segregation Principle)
class IFileSystemOperations {
public:
    virtual void saveToStorage(const FileSystemEntity& entity) = 0;
    virtual ~IFileSystemOperations() = default;
};

// Concrete class for storage operations (Dependency Inversion Principle)
class FileSystemStorage : public IFileSystemOperations {
public:
    void saveToStorage(const FileSystemEntity& entity) override {
        std::cout << "Saving " << entity.getName() << " to storage." << std::endl;
    }
};

// High-level class for managing file system (Dependency Inversion Principle)
class FileSystemManager {
public:
    FileSystemManager(std::unique_ptr<IFileSystemOperations> storage)
        : storage_(std::move(storage)) {}

    void saveEntity(const FileSystemEntity& entity) {
        storage_->saveToStorage(entity);
    }

private:
    std::unique_ptr<IFileSystemOperations> storage_;
};

// Main function to demonstrate usage
int main() {
    // Create files and directories
    auto file1 = std::make_unique<File>("document.txt", 100);
    auto file2 = std::make_unique<File>("image.png", 200);
    auto dir1 = std::make_unique<Directory>("docs");
    auto dir2 = std::make_unique<Directory>("root");

    // Build directory structure
    dir1->addEntity(std::move(file1));
    dir1->addEntity(std::move(file2));
    dir2->addEntity(std::move(dir1));

    // List contents
    dir2->list();

    // Calculate total size
    std::cout << "Total size of " << dir2->getName() << ": " << dir2->getSize() << " bytes" << std::endl;

    // Save to storage
    auto storage = std::make_unique<FileSystemStorage>();
    FileSystemManager manager(std::move(storage));
    manager.saveEntity(*dir2);

    return 0;
}


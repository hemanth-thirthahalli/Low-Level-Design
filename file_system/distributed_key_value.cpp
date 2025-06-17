#include <iostream>
#include <unordered_map>        //Provides a hash table for storing key-value pairs in each storage node. Chosen over std::map because it offers 	O(1) average-case complexity for lookups, which is critical for a key-value store's performance.
#include <vector>     //Used to store multiple StorageNode instances in the distributed store. Chosen over other containers like std::array because the number of shards is dynamic and std::vector handles dynamic resizing efficiently.
#include <string>
#include <mutex>    
#include <thread>
#include <functional>    //Provides std::hash for the sharding strategy. Chosen because it offers a standard, efficient way to hash strings for shard distribution.
#include <stdexcept>
#include <memory>    //Provides std::unique_ptr for managing StorageNode objects. Chosen over raw pointers to ensure automatic resource management and prevent memory leaks.
#include <shared_mutex>    //Used for thread-safety. std::shared_mutex allows multiple readers or one writer, which is ideal for a key-value store where reads (GET) are more frequent than writes (SET/DELETE). Chosen over std::mutex alone because it optimizes concurrent read access.

//The headers are selected to balance performance, safety, and modern C++ practices. For example, std::unique_ptr (from <memory>) ensures RAII (Resource Acquisition Is Initialization) for memory management, avoiding manual delete calls, which could lead to errors in a distributed system.

// Abstract base class for key-value storage    Abstraction (hiding implementation details)   Polymorphism (allowing derived classes to implement these methods differently).

class KeyValueStore {
public:
    virtual ~KeyValueStore() = default;
    virtual void set(const std::string& key, const std::string& value) = 0;
    virtual std::string get(const std::string& key) const = 0;
    virtual bool del(const std::string& key) = 0;
};

// Node class representing a single shard in the distributed system

class StorageNode : public KeyValueStore {
private:
    std::unordered_map<std::string, std::string> store_;
    mutable std::shared_mutex mutex_; // For thread-safe read/write

public:
    void set(const std::string& key, const std::string& value) override {
        std::unique_lock lock(mutex_);
        store_[key] = value;
    }
	
	
	//   The const outside the parameter list in ''' std::string get(const std::string& key) const override ''' ensures that the method is read-only, aligning with its purpose of retrieving values without modifying the StorageNode’s state. It enables thread-safe concurrent reads (with proper locking), supports const-correctness, and allows the method to be called on const objects.

    std::string get(const std::string& key) const override {
        std::shared_lock lock(mutex_);
        auto it = store_.find(key);
        if (it == store_.end()) {
            throw std::runtime_error("Key not found: " + key);
        }
        return it->second;
    }

    bool del(const std::string& key) override {
        std::unique_lock lock(mutex_);
        return store_.erase(key) > 0;
    }
};

// Sharding strategy interface
class ShardingStrategy {
public:
    virtual ~ShardingStrategy() = default;
    virtual size_t getShardIndex(const std::string& key, size_t numShards) const = 0;
};

// Simple hash-based sharding strategy
class HashShardingStrategy : public ShardingStrategy {
public:
    size_t getShardIndex(const std::string& key, size_t numShards) const override {
        std::hash<std::string> hasher;
        return hasher(key) % numShards;
    }
};

// Distributed key-value store manager
class DistributedKeyValueStore {
private:
    std::vector<std::unique_ptr<StorageNode>> nodes_;
    std::unique_ptr<ShardingStrategy> shardingStrategy_;
    size_t numShards_;

public:
    DistributedKeyValueStore(size_t numShards)
        : numShards_(numShards), shardingStrategy_(std::make_unique<HashShardingStrategy>()) {
        for (size_t i = 0; i < numShards; ++i) {
            nodes_.push_back(std::make_unique<StorageNode>());
        }
    }

    void set(const std::string& key, const std::string& value) {
        size_t shardIndex = shardingStrategy_->getShardIndex(key, numShards_);
        nodes_[shardIndex]->set(key, value);
    }

    std::string get(const std::string& key) const {
        size_t shardIndex = shardingStrategy_->getShardIndex(key, numShards_);
        return nodes_[shardIndex]->get(key);
    }

    bool del(const std::string& key) {
        size_t shardIndex = shardingStrategy_->getShardIndex(key, numShards_);
        return nodes_[shardIndex]->del(key);
    }

    // For testing purposes, get node for inspection
    StorageNode& getNode(size_t index) {
        if (index >= numShards_) {
            throw std::out_of_range("Invalid shard index");
        }
        return *nodes_[index];
    }
};

// Client interface
class KVClient {
private:
    DistributedKeyValueStore& store_;

public:
    KVClient(DistributedKeyValueStore& store) : store_(store) {}

    void set(const std::string& key, const std::string& value) {
        try {
            store_.set(key, value);
        } catch (const std::exception& e) {
            std::cerr << "Error setting key: " << e.what() << std::endl;
            throw;
        }
    }

    std::string get(const std::string& key) const {
        try {
            return store_.get(key);
        } catch (const std::exception& e) {
            std::cerr << "Error getting key: " << e.what() << std::endl;
            throw;
        }
    }

    bool del(const std::string& key) {
        try {
            return store_.del(key);
        } catch (const std::exception& e) {
            std::cerr << "Error deleting key: " << e.what() << std::endl;
            throw;
        }
    }
};

// Example usage
int main() {
    try {
        // Initialize distributed store with 3 shards
        DistributedKeyValueStore store(3);
        KVClient client(store);

        // Test operations
        client.set("key1", "value1");
        client.set("key2", "value2");
        client.set("key3", "value3");

        std::cout << "Get key1: " << client.get("key1") << std::endl;
        std::cout << "Get key2: " << client.get("key2") << std::endl;

        if (client.del("key1")) {
            std::cout << "Key1 deleted successfully" << std::endl;
        }

        // This should throw an exception
        try {
            client.get("key1");
        } catch (const std::exception& e) {
            std::cout << "Expected error: " << e.what() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}






/*  example for auto 

		#include <iostream>
		#include <unordered_map>
		#include <string>

		class MyCache {
			std::unordered_map<std::string, std::string> store;

		public:
			MyCache() {
				store["apple"] = "fruit";
				store["carrot"] = "vegetable";
			}

			// Using auto for simplicity
			std::string getWithAuto(const std::string& key) const {
				auto it = store.find(key);  // auto deduces const_iterator
				return (it != store.end()) ? it->second : "Not found";
			}

			// Without auto: fully qualified type
			std::string getWithoutAuto(const std::string& key) const {
				std::unordered_map<std::string, std::string>::const_iterator it = store.find(key);
				return (it != store.end()) ? it->second : "Not found";
			}
		};

		int main() {
			MyCache cache;

			std::cout << "Using auto: " << cache.getWithAuto("apple") << std::endl;
			std::cout << "Without auto: " << cache.getWithoutAuto("carrot") << std::endl;
			std::cout << "Missing key: " << cache.getWithAuto("banana") << std::endl;

			return 0;
		}
*/
#include <iostream>
#include <pthread.h>
#include <stdexcept>

class Singleton {
protected:
    static pthread_mutex_t mutex;
    
    Singleton() {
        if (pthread_mutex_init(&mutex, NULL) != 0) {
            throw std::runtime_error("Mutex initialization failed");
        }
    }
    
    virtual ~Singleton() {
        pthread_mutex_destroy(&mutex);
    }

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static void lock() {
        pthread_mutex_lock(&mutex);
    }

    static void unlock() {
        pthread_mutex_unlock(&mutex);
    }

    // Public method to access the mutex
    static pthread_mutex_t& getMutex() {
        return mutex;
    }

    virtual void doSomething() = 0;
};

pthread_mutex_t Singleton::mutex;

class Guard {
private:
    Singleton& singleton;

public:
    explicit Guard(Singleton& s) : singleton(s) {
        singleton.lock();
    }

    ~Guard() {
        singleton.unlock();
    }

    Guard(const Guard&) = delete;
    Guard& operator=(const Guard&) = delete;
};

class ConcreteSingleton : public Singleton {
private:
    static ConcreteSingleton* instance;

    ConcreteSingleton() : Singleton() {}

public:
    static ConcreteSingleton* getInstance() {
        if (instance == nullptr) {
            Guard guard(*instance);
            if (instance == nullptr) {
                instance = new ConcreteSingleton();
            }
        }
        return instance;
    }

    void doSomething() override {
        std::cout << "ConcreteSingleton doing something" << std::endl;
    }
};

ConcreteSingleton* ConcreteSingleton::instance = nullptr;

int main() {
    ConcreteSingleton* singleton = ConcreteSingleton::getInstance();
    singleton->doSomething();

    // Example of using the Guard class
    {
        Guard guard(*singleton);
        // Critical section
        std::cout << "Inside critical section" << std::endl;
    }
    // Mutex is automatically released when guard goes out of scope

    return 0;
}
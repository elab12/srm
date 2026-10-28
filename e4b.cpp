// Experiment 4 - Part B: Mutual exclusion using a monitor
// SRM Institute of Science and Technology - OS Lab (21CSC202J)
//
// Compile: g++ e4b.cpp -o e4b -pthread
// Run:     ./e4b

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

class NinjaMonitor {
public:
    // Constructor to initialize the shared resource and synchronization variables
    NinjaMonitor() : SharedResource(0) {}

    // Method to access the shared resource
    int AccessSharedResource() {
        std::unique_lock<std::mutex> lock(mtx_);
        while (SharedResource == 0) {
            CondVar.wait(lock);
        }
        int value = SharedResource;
        SharedResource = 0;
        return value;
    }

    // Method to update the shared resource
    void UpdateSharedResource(int new_value) {
        std::unique_lock<std::mutex> lock(mtx_);
        SharedResource = new_value;
        CondVar.notify_one();
    }

private:
    int SharedResource;
    std::mutex mtx_;
    std::condition_variable CondVar;
};

// Function that uses the monitor to access the shared resource
void NinjaThread(NinjaMonitor& monitor) {
    int value = monitor.AccessSharedResource();
    std::cout << "Ninja thread read shared resource: " << value << std::endl;
}

int main() {
    NinjaMonitor monitor;
    std::thread t(NinjaThread, std::ref(monitor));

    // Update the shared resource after some time
    std::this_thread::sleep_for(std::chrono::seconds(1));
    monitor.UpdateSharedResource(67);

    t.join();
    return 0;
}

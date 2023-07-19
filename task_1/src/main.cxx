#include <iostream>
#include <cstdlib>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>

#define ALL_CLIENTS 6

class clients {
private:
    std::atomic_int client;
    std::mutex mx;
public:
    clients() { client.store(0, std::memory_order_release); }

    void add_client() { client.store(get_client() + 1,std::memory_order_release); }
    int get_client() { 
        std::scoped_lock<std::mutex> lm(mx);
        return client.load(std::memory_order_acquire);
    }
    void remove_client() { client.store(get_client() - 1, std::memory_order_release); }
};

void add_client(clients& client) {
    while (client.get_client() != ALL_CLIENTS) {
        client.add_client();
        std::cout << "\x1b[0;32mAdd client:\x1b[0m " << client.get_client() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void remove_client(clients& client) {
    while (client.get_client()) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        client.remove_client();
        std::cout << "\x1b[0;31mRemove client:\x1b[0m " << client.get_client() << std::endl;
    }
}

int main(void) {
    clients cl;
    std::thread clients(add_client, std::ref(cl));
    std::thread window(remove_client, std::ref(cl));
    clients.join();
    window.join();
    return EXIT_SUCCESS;
}
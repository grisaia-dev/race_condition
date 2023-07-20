#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <PBL/PBL.hxx>


void do_smtf(const int& n, PBL::ProgressBar& bar) {
    std::scoped_lock<std::mutex> barLock(bar.ret_mutex());
    std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
    for (int i=0; i < n; i++) {
        // doing some...
        bar.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << std::endl;
    bar.reset();
}

int main(void) {
    int n = 100000;
    int threadsN = 2;
    std::vector<std::thread> threads_pool;
    PBL::ProgressBar bar(n);
    PBL::Timer time;

    for (int i=1; i <= threadsN; i++) {
        std::cout << "Thread number: " << i << std::endl;
        time.start("Time");
        threads_pool.emplace_back(std::thread(do_smtf, std::ref(n), std::ref(bar)));
        threads_pool.at(i-1).join();
        time.print();
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
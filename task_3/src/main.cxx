#include <iostream>
#include <cstdlib>
#include <mutex>
#include <thread>

class Data {
public:
    explicit Data(int const var) : m_scalar(var) {}
    inline int get_scalar() const { return this->m_scalar; }
    inline void set_scalar(int const var) { this->m_scalar = var; }
    inline std::mutex& get_mutex() { return this->m_mtx; }
private:
    int m_scalar;
    std::mutex m_mtx;
};

// std::lock
void swap_lock(Data& data1, Data& data2) {
    std::lock(data1.get_mutex(), data2.get_mutex());
    std::lock_guard<std::mutex> mlg1(data1.get_mutex(), std::adopt_lock);
    std::lock_guard<std::mutex> mlg2(data2.get_mutex(), std::adopt_lock);

    std::cout << "1 Before swap:\n\t" << "data1 - " << data1.get_scalar() << "\n\tdata2 - " << data2.get_scalar() << std::endl;

    auto temp = data1.get_scalar();
    data1.set_scalar(data2.get_scalar());
    data2.set_scalar(temp);

    std::cout << "1 After swap:\n\t" << "data1 - " << data1.get_scalar() << "\n\tdata2 - " << data2.get_scalar() << std::endl;
}

// std::scoped_lock
void swap_scoped_lock(Data& data1, Data& data2) {
    std::scoped_lock msl{data1.get_mutex(), data2.get_mutex()};

    std::cout << "2 Before swap:\n\t" << "data1 - " << data1.get_scalar() << "\n\tdata2 - " << data2.get_scalar() << std::endl;

    auto temp = data1.get_scalar();
    data1.set_scalar(data2.get_scalar());
    data2.set_scalar(temp);

    std::cout << "2 After swap:\n\t" << "data1 - " << data1.get_scalar() << "\n\tdata2 - " << data2.get_scalar() << std::endl;
}
// std::unique_lock
void swap_unique_lock(Data& data1, Data& data2) {
    std::unique_lock mul1{data1.get_mutex(), std::defer_lock};
    std::unique_lock mul2{data2.get_mutex(), std::defer_lock};
    std::lock(mul1, mul2);

    std::cout << "3 Before swap:\n\t" << "data1 - " << data1.get_scalar() << "\n\tdata2 - " << data2.get_scalar() << std::endl;

    auto temp = data1.get_scalar();
    data1.set_scalar(data2.get_scalar());
    data2.set_scalar(temp);

    std::cout << "3 After swap:\n\t" << "data1 - " << data1.get_scalar() << "\n\tdata2 - " << data2.get_scalar() << std::endl;
}

int main(void) {
    Data data1{5};
    Data data2{3};

    std::thread t1(swap_lock, std::ref(data1), std::ref(data2));
    std::thread t2(swap_scoped_lock, std::ref(data1), std::ref(data2));
    std::thread t3(swap_unique_lock, std::ref(data1), std::ref(data2));

    t1.detach();
    t2.detach();
    t3.join();
    return EXIT_SUCCESS;
} 
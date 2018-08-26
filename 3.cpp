#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

/* 
компиляция: 
    g++ -std=c++14 -Wall -o app 3.cpp -pthread
*/


//=============================================================================
#define SLEEP_THREAD_MS(val) this_thread::sleep_for(chrono::milliseconds( (val) )) 
#define PRINT_THREAD_ID() "[" << this_thread::get_id() << "]: "

//=============================================================================
class MyClass {
public:
    int Sum(int a, int b) {
        SLEEP_THREAD_MS(2000);
        cout << PRINT_THREAD_ID() << "==========\t" << "       Sum STARTED" << "\t===========" << endl;
        int result = a + b;
        SLEEP_THREAD_MS(5000);
        cout << PRINT_THREAD_ID() << "==========\t" << "       Sum FINISHED" << "\t===========" << endl;
        return result;           
    }
};

//=============================================================================
int main() {
    ///////////////////////////////////////////////////////////////////////////
    setlocale(LC_ALL, "ru");
    cout << "Упражения из цикла уроков: Многопоточность" << endl;
    cout << "#3 Потоки и методы класса" << endl;
    cout << endl;

    ///////////////////////////////////////////////////////////////////////////
    MyClass m;
    int sum = 0;

    cout << "Create thread with link to method" << endl;
    thread th0(&MyClass::Sum, m, 2, 5);

    cout << "Create thread with lambda to method" << endl;
    thread th1([&]() {
        sum = m.Sum(2, 5);
    });

    ///////////////////////////////////////////////////////////////////////////
    for (int i = 1; i <= 10; ++i) {
        cout << PRINT_THREAD_ID() << "main loop " << i << endl;
        SLEEP_THREAD_MS(500);
    }

    ///////////////////////////////////////////////////////////////////////////
    th0.join();
    th1.join();
    cout << "sum result = " << sum << endl;

    return 0;
}
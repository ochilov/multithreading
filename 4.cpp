#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;

/* 
компиляция: 
    g++ -std=c++14 -Wall -o app 4.cpp -pthread

статьи:
    https://habr.com/post/328348/
*/

// ============================================================================
#define TIME_NOW() std::chrono::steady_clock::now()
#define TIME_DIFF_MS(begin, end) std::chrono::duration_cast<std::chrono::milliseconds>((end) - (begin)).count()

// ============================================================================
mutex mtx;
void Print(char c) {
    this_thread::sleep_for(chrono::milliseconds(1000));
    //mtx.lock();
    {
        lock_guard<mutex> lock(mtx);
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 10; ++j) {
                cout << c << flush;
                this_thread::sleep_for(chrono::milliseconds(20));
            }
            cout << endl;
        }
        cout << endl;
    }
    //mtx.unlock();

    this_thread::sleep_for(chrono::milliseconds(1000));
}


int main() {
    ///////////////////////////////////////////////////////////////////////////
    setlocale(LC_ALL, "ru");
    cout << "Упражения из цикла уроков: Многопоточность" << endl;
    cout << "#4 Синхронизация потоков. Mutex" << endl;
    cout << endl;

    cout << "Печать в основном потоке:" << endl;
    auto begin = TIME_NOW();
    Print('*');
    Print('#');
    Print('.');
    auto end = TIME_NOW();
    cout << "Время работы " << TIME_DIFF_MS(begin, end) << " мс" << endl;

    cout << endl << "Печать в разных потоках с синхронизацией по mutex" << endl;
    begin = TIME_NOW();
    thread th1(Print, '*');
    thread th2(Print, '#');
    thread th3(Print, '.');
    th1.join();
    th2.join();
    th3.join();
    end = TIME_NOW();
    cout << "Время работы " << TIME_DIFF_MS(begin, end) << " мс" << endl;

    return 0;
}
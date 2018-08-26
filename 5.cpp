#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;
/*
компиляция:
    g++ -std=c++14 -Wall -o app 5.cpp -pthread
*/

void Print(char c) {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 10; ++j) {
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(20));
        }
        cout << endl;
    }
    cout << endl;
}

mutex mtx1, mtx2;
void Print1() {
    mtx2.lock();
    this_thread::sleep_for(chrono::milliseconds(1));
    mtx1.lock();

    Print('*');

    mtx1.unlock();
    mtx2.unlock();
}
void Print2() {
    mtx1.lock();
    this_thread::sleep_for(chrono::milliseconds(1));
    mtx2.lock();

    Print('#');

    mtx2.unlock();
    mtx1.unlock();
}

int main() {
    setlocale(LC_ALL, "ru");
    cout << "Упражения из цикла уроков: Многопоточность" << endl;
    cout << "#5 Синхронизация потоков. Deadlock" << endl;
    cout << endl;

    thread th1(Print1);
    thread th2(Print2);
    th1.join();
    th2.join();

    return 0;
}
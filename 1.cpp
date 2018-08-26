#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

/* 
компиляция: 
    g++ -std=c++14 -Wall -o app 1.cpp -pthread
работа с курсором в консоли:
    https://www.opennet.ru/base/dev/console_ctl.txt.html
    http://www.programmersforum.ru/showthread.php?t=171984
*/

#define TIME_NOW() std::chrono::steady_clock::now()
#define TIME_DIFF_MS(begin, end) std::chrono::duration_cast<std::chrono::milliseconds>((end) - (begin)).count()

void DoWork() {
    for (int i = 0; i < 3; ++i) {
        cout << "ID потока = " << this_thread::get_id() << "\tDoWork\t" << i  << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main() {
    ///////////////////////////////////////////////////////////////////////////
    setlocale(LC_ALL, "ru");
    cout << "Упражения из цикла уроков: Многопоточность" << endl;
    cout << "#1 Создание потока. Основы" << endl;
    cout << endl;

    ///////////////////////////////////////////////////////////////////////////
    cout << "thread id = " << this_thread::get_id() << endl;

    ///////////////////////////////////////////////////////////////////////////
    // задержка 'sleep for' в текущем потоке.
    // #1. отсчёт 3 секунд точками. (...)
    cout << "#1 sleep for 3s." << flush; //flush нужен, чтобы данные из буфера закинулись в поток
    this_thread::sleep_for(chrono::milliseconds(1000)); cout << "." << flush;
    this_thread::sleep_for(chrono::milliseconds(1000)); cout << "." << flush;
    this_thread::sleep_for(chrono::milliseconds(1000)); cout << " finish" << endl;

    // #2. отсчёт 3 секунд цифрами (3, 2, 1)
    cout << "#2 sleep for 3s" << flush;
    this_thread::sleep_for(chrono::milliseconds(1000)); cout << "\033[2D2s" << flush;
    this_thread::sleep_for(chrono::milliseconds(1000)); cout << "\033[2D1s" << flush;
    this_thread::sleep_for(chrono::milliseconds(1000)); cout << "\033[2Dfinish" << endl;

    ///////////////////////////////////////////////////////////////////////////
    // вызов функций в одном потоке
    cout << endl << "single thread operations:" << endl;
    auto begin = TIME_NOW();
    
    DoWork();
    
    for (int i = 0; i < 3; ++i) {
        cout << "ID потока = " << this_thread::get_id() << "\tmain\t" << i << endl;
        this_thread::sleep_for(chrono::milliseconds(250));
    }
    
    auto end = TIME_NOW();
    cout << "total time is " << TIME_DIFF_MS(begin, end) << "ms" << endl;


    ///////////////////////////////////////////////////////////////////////////
    // вызов функций в другом потоке
    cout << endl << "multi thread operations:" << endl;
    begin = TIME_NOW();
    
    thread th(DoWork);

    for (int i = 0; i < 3; ++i) {
        cout << "ID потока = " << this_thread::get_id() << "\tmain\t" << i << endl;
        this_thread::sleep_for(chrono::milliseconds(250));
    }

    th.join();

    end = TIME_NOW();
    cout << "total time is " << TIME_DIFF_MS(begin, end) << "ms" << endl;

    return 0;
} 
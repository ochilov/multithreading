#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

/* 
компиляция: 
    g++ -std=c++14 -Wall -o app 2.cpp -pthread
*/



//=============================================================================
#define SLEEP_THREAD_MS(val) this_thread::sleep_for(chrono::milliseconds( (val) )) 
#define PRINT_THREAD_ID() "[" << this_thread::get_id() << "]: "

//=============================================================================
void DoWork(int a, int b) {
    SLEEP_THREAD_MS(3000);
    cout << PRINT_THREAD_ID() << "==========\t" << "  DoWork   STARTED" << "\t===========" << endl;
    SLEEP_THREAD_MS(5000);
    cout << PRINT_THREAD_ID() << "a+b = " << a + b << endl;
    SLEEP_THREAD_MS(3000);
    cout << PRINT_THREAD_ID() << "==========\t" << "  DoWork   FINISHED" << "\t===========" << endl;
}

//=============================================================================
void ChangeData(int& a) {
    SLEEP_THREAD_MS(2000);
    cout << PRINT_THREAD_ID() << "==========\t" << "ChangeData STARTED" << "\t===========" << endl;
    SLEEP_THREAD_MS(5000);
    a *= 2;
    SLEEP_THREAD_MS(2000);
    a *= 2;
    SLEEP_THREAD_MS(2000);
    cout << PRINT_THREAD_ID() << "==========\t" << "ChangeData FINISHED" << "\t===========" << endl;
}

//=============================================================================
int Sum(int a, int b) {
    SLEEP_THREAD_MS(2000);
    cout << PRINT_THREAD_ID() << "==========\t" << "       Sum STARTED" << "\t===========" << endl;
    int result = a + b;
    SLEEP_THREAD_MS(5000);
    cout << PRINT_THREAD_ID() << "==========\t" << "       Sum FINISHED" << "\t===========" << endl;
    return result;    
}


//=============================================================================
int main() {
    ///////////////////////////////////////////////////////////////////////////
    setlocale(LC_ALL, "ru");
    cout << "Упражения из цикла уроков: Многопоточность" << endl;
    cout << "#2 Потоки с параметрами" << endl;
    cout << "Синтаксис:" << endl;
    cout << "  a) thread th(func, a, b)           - передеча аргументов по значению" << endl;
    cout << "  b) thread th(func, ref(a), ref(b)) - передача аргументов по ссылке" << endl;
    cout << "  c) thread th( [&r]() {r=func();} ) - получение результата функции через лямбда выражение" << endl;
    cout << endl;

    ///////////////////////////////////////////////////////////////////////////
    cout << "Create thread th(DoWork, 2, 3)" << endl;
    thread th(DoWork, 2, 3);
    // thread th1(DoWork, 2);       // не скомпилируется, т.к. количество аргументов не равно количеству параметров
    // thread th2(DoWork, 2, "3");  // не скомпилируется, т.к. тип последнего аргумента не совпадает с типом параметра 

    ///////////////////////////////////////////////////////////////////////////
    int a = 8;
    int prev_a = a;
    cout << "Create thread th(ChangeData, a), whith a = " << a << endl;
    thread th1(ChangeData, std::ref(a));

    ///////////////////////////////////////////////////////////////////////////
    cout << "Create thread with Anonymous function thread([]() {})" << endl;
    int sum = 0;
    int prev_sum = 0;
    thread th2([&sum]() { 
        sum = Sum(2, 37); 
    });

    ///////////////////////////////////////////////////////////////////////////
    cout << "Start main loop" << endl; 
    for (int i = 0; i < 20; ++i) {
        cout << PRINT_THREAD_ID() << "main loop " << i;
        SLEEP_THREAD_MS(500);
        
        if (a != prev_a) {
            cout << ", a is changed (" << prev_a << " -> " << a << ")";
            prev_a = a;
        }
        if (sum != prev_sum) {
            cout << ", sum is changed (" << prev_sum << " -> " << sum << ")";
            prev_sum = sum;
        }
        cout << endl;
    }

    ///////////////////////////////////////////////////////////////////////////
    th.join();
    th1.join();

    return 0;
} 
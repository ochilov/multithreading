# Многопоточное программирование в C++
Упражнения из цикла уроков "Многопоточное программирование | C++" на канале \#SimpleCode
Также, для дополнительной информации имеются полезные ссылки:
1. [Playlist on Youtube](https://www.youtube.com/playlist?list=PLQOaTSbfxUtAc_RpyDiWCHq0YTzLtVSD0)
2. [Циклы статей от автора Aleksander Alekseev](https://eax.me/cpp-multithreading/)
3. [И ещё статьи на habr.com](https://habr.com/post/182610/)
4. [О Deadlock](https://habr.com/post/119438/)

## \#1 Создание потока. Основы. ( [1.cpp](./1.cpp) )
- Потоки создаются инструментом thread. Синтаксис:
  ```cpp
  std::thread th(Func, arg1, arg2);
  ```
- После создания потока нужно отвязать его от основного потока ***thread.detach()*** или в конце функции дождаться его окончания ***thread.join()***

- Для работы с текущим потоком имеются следующие инструменты
  + this_thread::get_id() - id текущего потока
  + this_thread::sleep_for(time) - приостановка потока в режиме ожидания на заданное время 'time'
  + this_thread::sleep_until(time) - приостановка потока в режиме ожидания до заданного времени 'time'
  

## \#2 Потоки с параметрами. Возврат результата ( [2.cpp](./2.cpp) )
- Параметры и аргументы, в чём разница? Параметры - это объявления переменных в функции / методе, т.е. это часть сигнатуры *int func(***int param0, char param1***)*. Аргументы - это реальные данные, передаваемые в функцию при вызове оной *int result = func(***23, A***)*.

- Передача аргументов в поток.

  Аргументы передаются в поток в конструкторе *thread* по маске: ***thread(func, arg0, arg1,..., argN)***

  Количество и типы **АРГУМЕНТОВ** должны соответствовать количеству и типу **ПАРАМЕТРОВ**, иначе мы получим ошибку компиляции

- Получение результатов работы функции в потоке
  + передать аргументы по ссылке через функцию ***std::ref***:
     ```cpp
     thread th(func, std::ref(arg));
     ```
  + передать в поток анонимную функцую, которая в свою очередь вызовет целевую функцию
     ```cpp
     int result;
     thread th( [&result]() {result = func();} );
     ```


## \#3 Потоки и методы класса. ( [3.cpp](./3.cpp) )
- Методы класса передаются следующими способами:
  + в конструктор передаётся указатель на метод класса и аргументом - экземпляр класса (если конечно возвращаемый результат не нужен)
     ```cpp
     MyClass m;
     thread( &MyClass::Method(), m );
     ```
  + через анонимную функцию:
     ```cpp
     MyClass m;
     thread( [&]() {m.Method();} );
     ```

     
## \#4 Синхронизация потоков. Mutex. ( [4.cpp](./4.cpp) )
- При обращении из разных потоков к одному объекту может случиться так, что несколько потоков будут модифицировать его
  или один поток будет модифицировать, а другой читать связанные данные, которые уже не актуальны. 
  Например, *поток1* удаляет элемент из списка, а *поток2* считает размер.
  Поэтому необходимо синхранизировать потоки для защиты ***общих***, читай ***разделяемых данных***

- Мы можем читать и изменять один и тот же объект из разных потоков только в следующих 4-х случаях:

  + **Lock-based** - объект защищён блокировкой: spin-lock, std(mutex, recursive_mutex...)
  
  + **Atomic** - объект имеет тип std::atomic<T>, где T - указатель, bool или интегральный тип и только
     если для типа T существуют атомарные операции на уровне CPU
  
  + **Transaction-safe** - функции, реализованные для работы с объектов, обеспечивают потокобезапасную гарантию
  
  + **Lock-free** - функции для работы с объектом реализованы на основе Lock-free алгоритмов

- Самый распрастранённый и простой способ - это std (mutex, recursive_mutex...)
  ```cpp
  mutex mtx
  void fun() {
    mtx.lock();
    DoSome();
    mtx.unlock();
  ```
  
- Удобный инструмент для работы с mutex - это ***std::lock_guard***. Мьютекс блокируется в конструкторе и разблокируется в деструкторе
  ```cpp
  mutext mtx
  void fun() {
     lock_guard<mutex> lock(mtx);
     DoSome();
  ```
 
 
## \#5 Синхронизация потоков. Deadlock. ( [5.cpp](./5.cpp) )
 - ***Deadlock*** (взаимная блокировка) - ситуация, когда 2 и более потоков блокируют общие ресурсы в разной последовательности, за чего обратная разблокировка  невозможна.
  Т.е. *поток1* заблокировал ресурс *А* и потом пытается заблокировать ресурс *Б*, в то время как *поток2* заблокировал ресурс *Б* и пытается заблокировать *А*,
  но тот уже залочен, тем самым оба потока ждут пока ресурсы освободятся.

- Самый простой способ избежать эту проблемы - во всех блокировках нужно соблюдать один и тот же порядок блокировки и разблокировки, .е. *поток1* блокирует *А*, *Б* и *поток2* блокирует *А*, *Б* 

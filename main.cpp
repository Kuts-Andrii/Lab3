#include <iostream>
#include <thread>
#include <latch>
#include <chrono>
#include <syncstream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// one latch for each set, size = number of actions in it
latch la(7), lb(8), lc(6), ld(7), le(6), lf(8), lg(6), lh(4), li(5), lj(5), lk(3), ln(3);

void f(char x, int i)
{
    this_thread::sleep_for(chrono::seconds(1));
    osyncstream(cout) << "З набору " << x << " виконано дію " << i << "." << endl;
}

// wait for the sets it depends on, do the action, count down
void run(char s, int i)
{
    switch (s) {
    case 'a': f(s, i); la.count_down(); break;
    case 'b': f(s, i); lb.count_down(); break;
    case 'd': f(s, i); ld.count_down(); break;
    case 'c': la.wait(); f(s, i); lc.count_down(); break;
    case 'e': la.wait(); f(s, i); le.count_down(); break;
    case 'g': lb.wait(); ld.wait(); f(s, i); lg.count_down(); break;
    case 'h': lb.wait(); ld.wait(); f(s, i); lh.count_down(); break;
    case 'k': lc.wait(); lg.wait(); f(s, i); lk.count_down(); break;
    case 'j': lc.wait(); lg.wait(); f(s, i); lj.count_down(); break;
    case 'i': le.wait(); lh.wait(); f(s, i); li.count_down(); break;
    case 'f': lk.wait(); f(s, i); lf.count_down(); break;
    case 'n': lj.wait(); li.wait(); f(s, i); ln.count_down(); break;
    }
}

void thread1()
{
    run('a', 1); run('a', 5);
    run('b', 2); run('b', 6);
    run('d', 2); run('d', 6);
    run('g', 1); run('g', 5);
    run('c', 5);
    run('k', 1);
    run('h', 4);
    run('e', 4);
    run('j', 2);
    run('i', 1); run('i', 5);
    run('n', 1);
    run('f', 5);
}

void thread2()
{
    run('a', 2); run('a', 6);
    run('b', 3); run('b', 7);
    run('d', 3); run('d', 7);
    run('g', 2); run('g', 6);
    run('c', 6);
    run('k', 2);
    run('e', 1); run('e', 5);
    run('j', 3);
    run('i', 2);
    run('f', 1);
    run('n', 2);
    run('f', 6);
}

void thread3()
{
    run('a', 3); run('a', 7);
    run('b', 4); run('b', 8);
    run('d', 4);
    run('c', 1);
    run('g', 3);
    run('c', 3);
    run('h', 1);
    run('k', 3);
    run('e', 2); run('e', 6);
    run('j', 4);
    run('i', 3);
    run('f', 2);
    run('n', 3);
    run('f', 7);
}

void thread4()
{
    run('a', 4);
    run('b', 1); run('b', 5);
    run('d', 1); run('d', 5);
    run('c', 2);
    run('g', 4);
    run('c', 4);
    run('h', 2); run('h', 3);
    run('e', 3);
    run('j', 1); run('j', 5);
    run('i', 4);
    run('f', 3); run('f', 4);
    run('f', 8);
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    osyncstream(cout) << "Обчислення розпочато." << endl;
    auto start = chrono::steady_clock::now();

    {
        jthread t1(thread1);
        jthread t2(thread2);
        jthread t3(thread3);
        jthread t4(thread4);
    }

    chrono::duration<double> t = chrono::steady_clock::now() - start;
    osyncstream(cout) << "Обчислення завершено. Тривалість обчислення " << t.count() << " секунд." << endl;
}

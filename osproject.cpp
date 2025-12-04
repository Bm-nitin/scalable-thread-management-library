#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
using namespace std;

class ThreadPool {
private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    bool stop = false;

public:
    ThreadPool(int count) {
        for (int i = 0; i < count; i++) {
            workers.emplace_back([this]() {
                while (true) {
                    function<void()> task;

                    {
                        unique_lock<mutex> lock(mtx);
                        cv.wait(lock, [this]() {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty())
                            return;

                        task = tasks.front();
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    void addTask(function<void()> newTask) {
        {
            lock_guard<mutex> lock(mtx);
            tasks.push(newTask);
        }
        cv.notify_one();
    }

    void shutdown() {
        {
            lock_guard<mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();

        for (auto &t : workers)
            t.join();
    }
};

int main() {
    ThreadPool pool(3);

    for (int i = 1; i <= 5; i++) {
        pool.addTask([i]() {
            cout << "Task " << i
                 << " running in thread "
                 << this_thread::get_id() << endl;
        });
    }

    pool.shutdown();
    return 0;
}

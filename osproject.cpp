#include <iostream>
#include <thread>
using namespace std;

// Simple function for threads
void printTask(int id) {
    cout << "Task " << id 
         << " running in thread " 
         << this_thread::get_id() << endl;
}

int main() {
    // Create two simple threads
    thread t1(printTask, 1);
    thread t2(printTask, 2);

    // Wait for threads to finish
    t1.join();
    t2.join();

    cout << "Initial thread test completed." << endl;
    return 0;
}


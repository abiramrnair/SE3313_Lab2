#include <iostream>
#include "SharedObject.h"
#include "unistd.h"
#include "Semaphore.h"
using namespace std;

struct MyShared{
	int thread_id;
	int report_id;
	double time_passed;
};


int main(void)
{
	std::cout << "I am a reader" << std::endl;
	Shared<MyShared> sharedMemory ("sharedMemory"); // Only read priveleges
	Semaphore* read_guard = new Semaphore("read_guard");
	Semaphore* write_guard = new Semaphore("write_guard");
			
	while (true) {
		
		read_guard->Wait();
		// Reader semaphore waits	
		cout << "Reader Thread: " << sharedMemory->thread_id << " " << sharedMemory->report_id << " " << sharedMemory->time_passed << endl;
		sleep(2);	
		write_guard->Signal();
	}
}



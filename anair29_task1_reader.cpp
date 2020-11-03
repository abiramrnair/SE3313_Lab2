#include <iostream>
#include "SharedObject.h"
#include "unistd.h"
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
		
	while (true) {
				
		cout << "Reader Thread: " << sharedMemory->thread_id << " " << sharedMemory->report_id << " " << sharedMemory->time_passed << endl;
		sleep(2);	
	
	}
}

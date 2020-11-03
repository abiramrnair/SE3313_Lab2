#include <iostream>
#include <string>
#include "Blockable.h"
#include "thread.h"
#include "SharedObject.h"
#include <thread>
#include <vector>
#include "unistd.h"
#include <time.h>
#include "Blockable.h"
#include <ctime>
#include "Semaphore.h"

using namespace std;

struct MyShared{
	int thread_id;
	int report_id;
	double time_passed;
};

class WriterThread : public Thread{
	public:
		int 	threadNum;
		double	threadTime;
		bool	flag;
		int 	report = 0;
		time_t start = time(NULL);
		time_t finish;
		
		WriterThread(int in, int tm, bool f):Thread(8*1000){
			this->threadNum = in; //or whatever you want/need here
			this->threadTime = tm;				
			this->flag = f;	
		}
		virtual long ThreadMain(void) override{
			
			//declare shared memory var so this thread can access it
			Shared<MyShared> sharedMemory ("sharedMemory");
			
			//declare shared semaphore so that thread can access it
			Semaphore* read_guard = new Semaphore("read_guard");
			Semaphore* write_guard = new Semaphore("write_guard");
							
			while(true)
			{	
				sleep(threadTime);
					
				write_guard->Wait();
									
				finish = time(NULL);				
				time(&finish);
				report += 1;
				//write to shared memory				
				sharedMemory->report_id = report;
				sharedMemory->thread_id = threadNum;
				sharedMemory->time_passed = difftime(finish,start);				
				start = finish; // time logging 
				
				read_guard->Signal(); // Signal read semaphore
				
								
							
				if(flag==true){//Exit loop to end the thread
					break;
				}
			}
			
			return 0;
		}
};

int main(void)
{
	std::cout << "I am a Writer" << std::endl;
	string response;
	int count = 0;
	int wait_time;
	Shared<MyShared> shared("sharedMemory", true); //This is the owner of sharedMemory
	vector <WriterThread*> threads; // vector of object pointers
	Semaphore* write_guard = new Semaphore("write_guard", 1, true); // Owner
	Semaphore* read_guard = new Semaphore("read_guard", 0, true); // Owner
		
	while(true){		
		
		cout << "Would you like to create a writer thread?: " << endl;
		cin >> response;
		
		if (response == "n") {
			cout << "Finished. Goodbye!" << endl;
			break;
		} else {
		
			cout << "What is the delay time for this thread?: " << endl;
			cin >> wait_time;		
			count += 1;			
			threads.push_back(new WriterThread(count, wait_time, false));
			cout << "Thread created" << endl;			
		
		}		
		
	}
	
	for (int i = 0; i < count; i++) {	
		threads[i]->flag = true;
		delete threads[i];	
	}	
}




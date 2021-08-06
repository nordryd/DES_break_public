#include <iostream>
#include <thread>  /* Must be imported */

using namespace std;

static const int num_threads = 10;

int add(int x, int y)
{
	return x + y;
}

/*Basically the public void run() in Java
Must be void and have no args*/
void call_from_thread()
{
	cout << add(1, 2) << endl;
}

int main()
{
  /*Creates and runs the thread*/
	thread t1(call_from_thread);
	
  /*Joins the thread to the main method*/
	t1.join();
	
	cout << "Sent from MAIN" << endl;
}

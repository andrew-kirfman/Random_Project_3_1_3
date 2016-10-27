#include "task.h"

class RudderController : public Task {
public:
	RudderController(std::string _name) : Task(_name) {}

	void Run() {
		std::cout << "Rudder Controller [" << name << "] running\n" << std::flush;
		for (int i = 0; i < MAX_TASK_ITERATIONS; i++) {
			std::cout << "\t" << name << " waiting for next sensor input [" << i << "]\n" << std::flush;
			usleep(1000000);
			std::cout << "\t" << name << " issueing rudder control command [" << i << "]\n" << std::flush;
			usleep(10000);
		}
		std::cout << "Rudder Controller [" << name << "] task finished!\n" << std::flush;
	}
};

class AvionicsTask : public Task {
public:
	AvionicsTask(std::string _name) : Task(_name) {}

	void Run() {
		std::cout << "Avionics System [" << name << "] running\n" << std::flush;
		for (int i = 0; i < MAX_TASK_ITERATIONS; i++) {
			std::cout << "\t" << name << " waiting for next refresh interval [" << i << "]\n" << std::flush;
			usleep(700000);
			std::cout << "\t" << name << " refreshing avionics screen [" << i << "]\n" << std::flush;
			usleep(12000);
		}
		std::cout << "Avionics System [" << name << "] task finished!\n" << std::flush;
	}
};


int main(int argc, char * argv[]) {

	/* -- CREATE TASKS */
	Task * task1 = new RudderController("rudder control");
	Task * task2 = new AvionicsTask("avionics task");

	/* -- LAUNCH TASKS */
	task1->Start();
	task2->Start();

	Task::GracefullyExitMainThread();
	std::cout << "Main thread has exited" << std::endl;
}

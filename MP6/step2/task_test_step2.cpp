/* TEST PROGRAM FOR STEP 2 of MP2 */


#include "task.h"
#include "scheduler.h"
#include "schedulable.h"

class RudderController : public Schedulable {
public:
	RudderController(std::string _name, Scheduler * _sched) :
	Schedulable(_name, _sched) {}

	void Run() {
		threadsafe_console_output.println("Rudder Controller [" + name + "] running");
		for (int i = 0; i < DEFAULT_TASK_ITERATIONS; i++) {
			threadsafe_console_output.println(name + " waiting for next sensor input [" + std::to_string(i) + "]");
			if (i % 5 == 1) {
				sched->Resume(this);
				sched->Yield();
			}
			threadsafe_console_output.println(name + " issueing rudder control command [" + std::to_string(i) + "]");
			usleep(10000);
		}
		threadsafe_console_output.println("Rudder Controller [" + name + "] done!");
	}
};

class AvionicsTask : public Schedulable {
public:
	AvionicsTask(std::string _name, Scheduler * _sched) :
	Schedulable(_name, _sched) {}

	void Run() {
		threadsafe_console_output.println("Avionics System [" + name + "] running");
		for (int i = 0; i < DEFAULT_TASK_ITERATIONS; i++) {
			threadsafe_console_output.println(name + " waiting for next refresh interval [" + std::to_string(i) + "]");
			if (i % 10 == 1) {
				sched->Resume(this);
				sched->Yield();
			}
			threadsafe_console_output.println(name + " refreshing avionics screen [" + std::to_string(i) + "]");
			usleep(12000);
		}
		threadsafe_console_output.println("Avionics System [" + name + "] done!");
	}
};

int main(int argc, char * argv[]) {

	/* -- CREATE SCHEDULER */
	Scheduler * system_scheduler = new Scheduler("scheduler");

	/* -- CREATE TASKS */
	/*
	 	I'm not sure how memory management works in C,
	 	but if the code is rewritten in C++ the stack-allocated 
	 	task objects get destructed when main "gracefully" exits, 
	 	but the threads keep going, which is a huge problem. This is 
	 	demonstrated by the name printouts being empty after main exits, but
	 	could manifest in more colorful ways if the program
	 	was more complex. This introduces the considerations
	 	that memory must be de-allocated properly. In this case,
	 	the Task, Scheduler, and Schedulable classes had to
	 	be changed to accommodate this need.
		
	 	In the original solution code, execution hangs after 
	 	the Run functions finish because Yield blocks the task 
	 	regardless of whether the task has finished or not.
	 	This may be good-practice in C, I simply don't know,
	 	but in C++ it's an enormous memory leak because none of 
	 	the destructors get called. Again, this is an issue because
	 	Task objects must be allocated on the heap, in order that threads 
	 	won't try to use stack objects that have gone out of scope.
	 
	 	The solution I have adopted, for the moment, is to 
	 	introduce a new function into Scheduler, called Finish_Task(), 
	 	that's supposed to be called only when a Schedulable's Run()
	 	code has completed (i.e. at the end of CarrierForRun).
	 	It does everything that Yield() does, except that it attempts
	 	to remove from the ready queue any remaining instances
	 	of the finished task (which at that point are garbage), 
	 	it does not call Block() on old_task (so that tfunc can finish
	 	and call delete, btw that line also had to be added to Task::tfunc), 
	 	and if the ready queue is empty it delete-s the calling Scheduler 
	 	(which is presumed to be allocated on the heap) since there are no more 
	 	tasks to be scheduled.
	 
	 	Until feedback is received, this is how the step2 code
	 	will remain. If it proves necessary and feasible, this approach will
	 	be ported to step3. On the other hand, lessons learned from step3 may result
	 	in a return to the original approach. The decision will be
	 	made after those lessons have been learned.
	 */
//	RudderController task1("rudder control", system_scheduler);
//	AvionicsTask     task2("avionics task", system_scheduler);
	Schedulable * task1 = new RudderController("rudder control", system_scheduler);
	Schedulable * task2 = new AvionicsTask("avionics task", system_scheduler);

	/* -- LAUNCH TASKS */
//	task1.Start();
//	task2.Start();
	task1->Start();
	task2->Start();

	threadsafe_console_output.println("end of launch");

	usleep(100000);

	/* -- START SCHEDULING */
	system_scheduler->Kick_Off();

	/* -- Have the parent thread get out of the way. */
	Task::GracefullyExitMainThread();
}

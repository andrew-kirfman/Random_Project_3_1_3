#include <iostream>
#include "task.H"

using namespace std;

class RudderController : public Task {
public:
  RudderController(char _name[]) : Task(_name) {}

  void Run() {
    cout << "Rudder Controller [" << name << "] running\n" << flush;
    for (int i = 0; i < 100; i++) {
      cout << name << " waiting for next sensor input\n" << flush;
      usleep(1000000);
      cout << name << " issueing rudder control command\n" << flush;
      usleep(10000);
    }
  }
};

class AvionicsTask : public Task {
public:
  AvionicsTask(char _name[]) : Task(_name) {}

  void Run() {
    cout << "Avionics System [" << name << "] running\n" << flush;
    for (int i = 0; i < 100; i++) {
      cout << name << " waiting for next refresh interval\n" << flush;
      usleep(700000);
      cout << name << " refreshing avionics screen\n" << flush;
      usleep(12000);
    }
  }
};


int main(int argc, char * argv[]) {

  /* -- CREATE TASKS */
  RudderController task1("rudder control");
  AvionicsTask     task2("avionics task");

  /* -- LAUNCH TASKS */
  task1.Start();
  task2.Start();

  Task::GracefullyExitMainThread();
}

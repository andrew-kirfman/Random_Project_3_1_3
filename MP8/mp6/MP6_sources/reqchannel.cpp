/* 
    File: requestchannel.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2012/07/11

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>

#include "reqchannel.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* PRIVATE METHODS FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

char * RequestChannel::pipe_name(Mode _mode) {
  string pname = "fifo_" + my_name;

  if (my_side == CLIENT_SIDE) {
    if (_mode == READ_MODE) 
      pname += "a";
    else
      pname += "b";
  } else {
    /* SERVER_SIDE */
    if (_mode == READ_MODE) 
      pname += "b";
    else 
      pname += "a";
  }
  char * result = new char[pname.size()+1];
  strncpy(result, pname.c_str(), pname.size()+1);
  return result;
}

void RequestChannel::open_write_pipe(char * _pipe_name) {

    cout << "mkfifo write pipe [" << _pipe_name << "]\n" << flush;

  if (mkfifo(_pipe_name, 0600) < 0) {
    if (errno != EEXIST) {
      perror("Error creating pipe for writing; exit program");
      exit(1);
    }
  }

   cout << "open write pipe [" << _pipe_name << "]\n" << flush;

  wfd = open(_pipe_name, O_WRONLY);
  if (wfd < 0) {
    perror("Error opening pipe for writing; exit program");
    exit(1);
  }

    cout << "done opening write pipe [" << _pipe_name << "]\n" << flush;

}

void RequestChannel::open_read_pipe(char * _pipe_name) {

    cout << "mkfifo read pipe [" << _pipe_name << "]\n" << flush;

  if (mkfifo(_pipe_name, 0600) < 0) {
    if (errno != EEXIST) {
      perror("Error creating pipe for writing; exit program");
      exit(1);
    }
  }

    cout << "open read pipe [" << _pipe_name << "]\n" << flush;

  rfd = open(_pipe_name, O_RDONLY);
  if (rfd < 0) {
    perror("Error opening pipe for reading; exit program");
    exit(1);
  }

    cout << "done opening read pipe [" << _pipe_name << "]\n" << flush;

}

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

RequestChannel::RequestChannel(const string _name, const Side _side) : my_name(_name), my_side(_side) {

  if (_side == SERVER_SIDE) {
    open_write_pipe(pipe_name(WRITE_MODE));
    open_read_pipe(pipe_name(READ_MODE));
  } else {
    open_read_pipe(pipe_name(READ_MODE));
    open_write_pipe(pipe_name(WRITE_MODE));
  }

}

RequestChannel::~RequestChannel() {
  //cout << "close requests channel " << my_name << endl;
  close(wfd);
  close(rfd);
  if (my_side == SERVER_SIDE) {
    //cout << "close IPC mechanisms on server side for channel " << my_name << endl;
    /* Destruct the underlying IPC mechanisms. */
    if (remove(pipe_name(READ_MODE)) != 0) {
      perror(string("Request Channel (" + my_name + ") : Error deleting pipe for reading").c_str());
    }
      
    if (remove(pipe_name(WRITE_MODE)) != 0) {
      perror(string("Request Channel (" + my_name + ") : Error deleting pipe for writing").c_str());
    }
      //cout << "Bye y'all" << flush << endl;
  }
}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

const int MAX_MESSAGE = 255;

string RequestChannel::send_request(string _request) {
  cwrite(_request);
  string s = cread();
  return s;
}

string RequestChannel::cread() {

  char buf[MAX_MESSAGE];

  if (read(rfd, buf, MAX_MESSAGE) < 0) {
    perror(string("Request Channel (" + my_name + "): Error reading from pipe!").c_str());
  }
  
  string s = buf;

  //  cout << "Request Channel (" << my_name << ") reads [" << buf << "]\n";

  return s;

}

int RequestChannel::cwrite(string _msg) {

  if (_msg.length() >= MAX_MESSAGE) {
    cerr << "Message too long for Channel!\n";
    return -1;
  }

  //  cout << "Request Channel (" << my_name << ") writing [" << _msg << "]";

  const char * s = _msg.c_str();

  if (write(wfd, s, strlen(s)+1) < 0) {
    perror(string("Request Channel (" + my_name + ") : Error writing to pipe!").c_str());
  }

  //  cout << "(" << my_name << ") done writing." << endl;
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

string RequestChannel::name() {
  return my_name;
}

/*--------------------------------------------------------------------------*/
/* ACCESS FILE DESCRIPTORS OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

int RequestChannel::read_fd() {
  return rfd;
}

int RequestChannel::write_fd() {
  return wfd;
}




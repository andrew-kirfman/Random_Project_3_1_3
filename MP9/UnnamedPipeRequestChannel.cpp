/*
	UnnamedPipeRequestChannel.cpp

	Created by Joshua Higginbotham on 6/16/16.
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "UnnamedPipeRequestChannel.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

const int MAX_MESSAGE = NAME_MAX;
const int MAX_PENDING = 300; //Most platforms won't support this many threads anyway, I find
const bool VERBOSE = false;
const bool VERBOSE_DEBUG = false;

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* PRIVATE METHODS FOR CLASS   UnnamedPipeRequestChannel  */
/*--------------------------------------------------------------------------*/

int UnnamedPipeRequestChannel::write_fd() {
	return wfd;
}

int UnnamedPipeRequestChannel::read_fd() {
	return rfd;
}

std::string UnnamedPipeRequestChannel::format_file_descriptor_string() {
	
	return std::string(std::to_string(server_wfd) + "-" + std::to_string(server_rfd) + "-" + std::to_string(client_wfd) + "-" + std::to_string(client_rfd));
}

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   UnnamedPipeRequestChannel  */
/*--------------------------------------------------------------------------*/

UnnamedPipeRequestChannel::UnnamedPipeRequestChannel(const std::string _name, const Side _side) :
my_name(_name),
my_side(_side),
side_name((my_side == RequestChannel::SERVER_SIDE) ? "SERVER" : "CLIENT"),
sock_pathname("./" + my_name + "_sock"),
sock_listening("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + "_sock_listening2", 0),
fds_received("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + "_fd_received2", 0)
{
	/*
		Include references to Beej's guide
		and to relevant man pages for this section
	 */
	
	if(sock_pathname.length() + 1 > 108) { //108 - size of the sockaddr_un.sun_path field
		throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": pathname is too long");
	}
	
	//Necessary for proper error handling
	sigset_t sigpipe_set;
	sigemptyset(&sigpipe_set);
	if(sigaddset(&sigpipe_set, SIGPIPE) < 0) {
		if(my_side == RequestChannel::SERVER_SIDE) sock_listening.V();
		else fds_received.V();
		throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on sigaddset(&sigpipe_set, SIGPIPE)");
	}
	
	if((errno = pthread_sigmask(SIG_SETMASK, &sigpipe_set, NULL)) != 0) {
		if(my_side == RequestChannel::SERVER_SIDE) sock_listening.V();
		else fds_received.V();
		throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on pthread_sigmas(SIG_SETMASK, &sigpipe_set, NULL)");
	}
	
	if(my_side == RequestChannel::SERVER_SIDE) {
		int server_sock_fd, client_sock_fd;
		struct sockaddr_un server_sock_info, client_sock_info;
		int server_info_length;
		
		if(VERBOSE_DEBUG) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": creating socket...");
		
		server_sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if(server_sock_fd < 0) {
			sock_listening.V();
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on socket creation");
		}
		
		memset(&server_sock_info, '\0', sizeof(server_sock_info));
		server_sock_info.sun_family = AF_UNIX;
		strcpy(server_sock_info.sun_path, sock_pathname.c_str());
		server_info_length = offsetof(struct sockaddr_un, sun_path) + strlen(server_sock_info.sun_path) + 1;
		unlink(server_sock_info.sun_path);
		if(VERBOSE_DEBUG) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": binding...");
		if(::bind(server_sock_fd, (struct sockaddr *)&server_sock_info, server_info_length) < 0) {
			sock_listening.V();
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed to bind socket");
		}
						 
		if(VERBOSE_DEBUG) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": listening...");
		if(listen(server_sock_fd, MAX_PENDING) < 0) {
			sock_listening.V();
			unlink(sock_pathname.c_str());
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on listen");
		}
		
		if(sock_listening.V() < 0) {
			unlink(sock_pathname.c_str());
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on sem_post for sock_listening");
		}
		
		if(VERBOSE_DEBUG) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": accepting...");
		client_sock_fd = accept(server_sock_fd, NULL, NULL);
		if(client_sock_fd < 0) {
			unlink(sock_pathname.c_str());
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on accept");
		}
		
		if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": creating pipes...");
		int pipe1[2];
		int pipe2[2];
		if(pipe(pipe1) < 0) {
			unlink(sock_pathname.c_str());
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on pipe creation");
		}
		if(pipe(pipe2) < 0) {
			close(pipe1[0]);
			close(pipe1[1]);
			unlink(sock_pathname.c_str());
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on pipe creation");
		}
		server_wfd = pipe1[1];
		server_rfd = pipe2[0];
		client_wfd = pipe2[1];
		client_rfd = pipe1[0];
		wfd = server_wfd;
		rfd = server_rfd;
		const int NUM_CLIENT_FDS = 2;
		int client_pipe_fd_buf[NUM_CLIENT_FDS];
		client_pipe_fd_buf[0] = client_wfd;
		client_pipe_fd_buf[1] = client_rfd;
		
		/*
		 	Code heavily borrowed from "man 3 cmsg" example code
		 */
		struct msghdr fdmsg = { 0 };
		struct iovec iov[1];
		struct cmsghdr *cmsg;
		int * fdptr;
		union {
			/*
			 	Ancillary data buffer, wrapped in a union
			 	in order to ensure that it is suitably aligned.
			 */
			char buf[CMSG_SPACE(sizeof(client_pipe_fd_buf))];
			struct cmsghdr align;
		} u;
		char data[1];
		
		memset(&fdmsg, 0, sizeof(struct msghdr));
		data[0] = ' ';
		iov[0].iov_base = data;
		iov[0].iov_len = sizeof(data);
		fdmsg.msg_name = NULL;
		fdmsg.msg_namelen = 0;
		fdmsg.msg_iov = iov;
		fdmsg.msg_iovlen = 1;
		
		fdmsg.msg_control = u.buf;
		fdmsg.msg_controllen = sizeof(u.buf);
		fdmsg.msg_flags = 0;
		
		cmsg = CMSG_FIRSTHDR(&fdmsg);
		cmsg->cmsg_level = SOL_SOCKET;
		cmsg->cmsg_type = SCM_RIGHTS;
		cmsg->cmsg_len = CMSG_LEN(sizeof(int) * NUM_CLIENT_FDS);
		
		fdptr = (int *) CMSG_DATA(cmsg);	/* Initialize the payload */
		memcpy(fdptr, client_pipe_fd_buf, NUM_CLIENT_FDS * sizeof(int));
		
		if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": sending fds client_wfd == " + std::to_string(client_wfd) + ", client_rfd == " + std::to_string(client_rfd) + "...");
		
		if(sendmsg(client_sock_fd, &fdmsg, 0) < 0) {
			close(server_wfd);
			close(server_rfd);
			close(client_wfd);
			close(client_rfd);
			unlink(sock_pathname.c_str());
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on sendmsg");
		}
		
		if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": waiting for client to receive fds...");
		
		if(fds_received.P() < 0) {
			close(server_wfd);
			close(server_rfd);
			close(client_wfd);
			close(client_rfd);
			unlink(sock_pathname.c_str());
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on sem_wait for fds_received");
		}
		
		if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": finalizing, wfd == " + std::to_string(server_wfd) + ", rfd == " + std::to_string(server_rfd) + "...");
		close(client_wfd);
		close(client_rfd);
		
	}
	else {
		struct sockaddr_un client_sock_info;
		memset(&client_sock_info, 0, sizeof(sockaddr_un));
		
		int client_sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if(VERBOSE_DEBUG) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": creating socket...");
		if(client_sock_fd < 0) {
			fds_received.V();
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on socket creation");
		}
		
		client_sock_info.sun_family = AF_UNIX;
		strcpy(client_sock_info.sun_path, sock_pathname.c_str());
		
		if(sock_listening.P() < 0) {
			fds_received.V();
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on sem_wait for sock listening");
		}
		
		if(VERBOSE_DEBUG) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": connecting...");
		if(connect(client_sock_fd, (struct sockaddr *) &client_sock_info, sizeof(client_sock_info)) < 0) {
			fds_received.V();
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on connection");
		}
		
		if(VERBOSE_DEBUG) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": constructing message receive buffer...");
		
		struct msghdr fdmsg;
		struct iovec iov[1];
		struct cmsghdr *cmsg = NULL;
		char ctrl_buf[2 * CMSG_SPACE(sizeof(int))];
		char data[1];
		const int NUM_CLIENT_FDS = 2;
		int client_pipe_fd_buf[NUM_CLIENT_FDS];
		
		memset(&fdmsg, 0, sizeof(struct msghdr));
		memset(ctrl_buf, 0, CMSG_SPACE(2 * sizeof(int)));
		
		iov[0].iov_base = data;
		iov[0].iov_len = sizeof(data);
		
		fdmsg.msg_name = NULL;
		fdmsg.msg_namelen = 0;
		fdmsg.msg_control = ctrl_buf;
		fdmsg.msg_controllen = CMSG_SPACE(2 * sizeof(int));
		fdmsg.msg_iov = iov;
		fdmsg.msg_iovlen = 1;
		
		if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": recieving message...");
		if(recvmsg(client_sock_fd, &fdmsg, 0) < 0) {
			fds_received.V();
			unlink(sock_pathname.c_str());
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on recvmsg");
		}
		
		if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": parsing message...");
		for(cmsg = CMSG_FIRSTHDR(&fdmsg);
			cmsg != NULL;
			cmsg = CMSG_NXTHDR(&fdmsg, cmsg)) {
			
			if( (cmsg->cmsg_level == SOL_SOCKET) &&
				(cmsg->cmsg_type == SCM_RIGHTS) ) {
				memcpy(client_pipe_fd_buf, (char*) CMSG_DATA(cmsg), NUM_CLIENT_FDS * sizeof(int));
				break;
			}
		}

		client_wfd = client_pipe_fd_buf[0];
		client_rfd = client_pipe_fd_buf[1];
		wfd = client_wfd;
		rfd = client_rfd;
		
		if(wfd < 0 || rfd < 0) {
			if(wfd >= 0) close(wfd);
			if(rfd >= 0) close(rfd);
			fds_received.V();
			unlink(sock_pathname.c_str());
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed to find file descriptors");
		}
		
		if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": signaling fds received...");
		
		if(fds_received.V() < 0) {
			close(wfd);
			close(rfd);
			fds_received.V();
			unlink(sock_pathname.c_str());
			throw sync_lib_exception("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed sem_post for fds_received");
		}
		
		if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": received file descriptors wfd == " + std::to_string(wfd) + ", rfd == " + std::to_string(rfd));
	}
	
	unlink(sock_pathname.c_str());
	
	if((errno = pthread_mutexattr_init(&srl_attr)) != 0) {
		throw sync_lib_exception(my_name + ":" + side_name + ": failed on pthread_mutexattr_init");
	}
	if((errno = pthread_mutexattr_setrobust(&srl_attr, PTHREAD_MUTEX_ROBUST)) != 0) {
		throw sync_lib_exception(my_name + ":" + side_name + ": failed on pthread_mutexattr_setrobust");
	}
	if((errno = pthread_mutexattr_setpshared(&srl_attr, PTHREAD_PROCESS_SHARED)) != 0) {
		throw sync_lib_exception(my_name + ":" + side_name + ": failed on pthread_mutexattr_setpshared");
	}
	if((errno = pthread_mutex_init(&send_request_lock, &srl_attr)) != 0) {
		throw sync_lib_exception(my_name + ":" + side_name + ": failed on pthread_mutex_init");
	}
}

UnnamedPipeRequestChannel::~UnnamedPipeRequestChannel() {
	if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": close requests channel " + my_name);
	pthread_mutexattr_destroy(&srl_attr);
	pthread_mutex_destroy(&read_lock);
	pthread_mutex_destroy(&write_lock);
	pthread_mutex_destroy(&send_request_lock);
	close(wfd);
	close(rfd);
	
	if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": close IPC mechanisms for channel " + my_name);
	/*
	 	In case the constructor is interrupted,
	 	or something weird happens...
	 */
	unlink(sock_pathname.c_str());
}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

std::string UnnamedPipeRequestChannel::send_request(std::string _request) {
	pthread_mutex_lock(&send_request_lock);
	if(cwrite(_request) < 0) {
		pthread_mutex_unlock(&send_request_lock);
		return "ERROR";
	}
	std::string s = cread();
	pthread_mutex_unlock(&send_request_lock);
	return s;
}

std::string UnnamedPipeRequestChannel::cread() {
	
	pthread_mutex_lock(&read_lock);
	
	char read_buf[MAX_MESSAGE];
	memset(read_buf, '\0', MAX_MESSAGE);
	
	if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": attempting read...");

	int read_return_value;
	if ((read_return_value = read(rfd, read_buf, MAX_MESSAGE)) <= 0) {
		if(read_return_value < 0) {
			threadsafe_console_output.perror("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error reading from pipe");
			pthread_mutex_unlock(&read_lock);
			return "ERROR";
		}
		else {
			threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": broken/closed pipe detected, exiting worker thread...");
		}
		close(rfd);
		close(wfd);
		pthread_mutex_unlock(&send_request_lock);
		pthread_mutex_unlock(&read_lock);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&read_lock);
	
	std::string read_result(read_buf);
	
	if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": read [" + read_result + "]");

	return read_result;
}

int UnnamedPipeRequestChannel::cwrite(std::string _msg) {
	if (_msg.length() + 1 >= MAX_MESSAGE) { //+1 for null terminator byte
		std::cerr << "Message too long for Channel!\n";
		return -1;
	}
	
	pthread_mutex_lock(&write_lock);
	
	if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": writing [" + _msg + "]");
	
	char write_buf[MAX_MESSAGE];
	strcpy(write_buf, _msg.c_str());
	int bytes_written = write(wfd, write_buf, strlen(write_buf)+1);
	
	if (bytes_written < 0) {
		if(errno != EPIPE) {
			threadsafe_console_output.perror("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error writing [" + std::string(write_buf) + "] to pipe");
			pthread_mutex_unlock(&write_lock);
			return -1;
		}
		
		else {
			threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": broken/closed pipe detected, exiting worker thread...");
		}
		close(rfd);
		close(wfd);
		pthread_mutex_unlock(&send_request_lock);
		pthread_mutex_unlock(&write_lock);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&write_lock);
	
	if(VERBOSE) threadsafe_console_output.println("UNNAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": finished writing [" + std::string(write_buf) + "]");
	
	return bytes_written;
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

std::string UnnamedPipeRequestChannel::name() {
	return my_name;
}



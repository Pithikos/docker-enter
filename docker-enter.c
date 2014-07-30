/*
 * Author: Johan Hanssen Seferidis
 * 
 * docker-enter
 * 
 * */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sched.h>
#include <dirent.h>
#include <getopt.h>

#define USERNAME_MAX  32+1
#define CONT_ID_MAX 12+1

#define VERSION "docker-enter v0.1"
#define DEFAULT_SHELL "/bin/sh"


static char  dir [PATH_MAX]       = {'\0'};	// Working directory
static char  user[USERNAME_MAX]   = {'\0'};	// User
static char  cont_ID[CONT_ID_MAX] = {'\0'};	// Container name
static pid_t cont_pid             =     -1;	// Container PID



// Table with namespaces info
static struct namespace_info {
	const char *name;
	const char *file;
	int type;
} namespace_table[] = {
	{ "user",  "ns/user",  CLONE_NEWUSER },
	{ "ipc",   "ns/ipc",   CLONE_NEWIPC  },
	{ "uts",   "ns/uts",   CLONE_NEWUTS  },
	{ "net",   "ns/net",   CLONE_NEWNET  },
	{ "pid",   "ns/pid",   CLONE_NEWPID  },
	{ "mnt",   "ns/mnt",   CLONE_NEWNS   },
	{  NULL,    NULL,                -1  }
};




/* 
 * Execute shell
 * 
 * */
static void exec_shell()
{
	const char *shell = getenv("SHELL");
	if (!shell)
		shell = DEFAULT_SHELL;
	execl(shell, shell, NULL);
}




/* 
 * Translates a container ID to a PID (uses docker)
 * 
 * */
static pid_t container_pid(char* cont_ID) {
	
	pid_t cont_pid = -1;
	char output[4096];
	int pip[2];
	pid_t pid;
	
	if (pipe(pip)==-1)
		perror("Pipe");
	
	if ((pid = fork()) == -1)
		perror("Fork");
	
	if(pid == 0) { // child
		dup2 (pip[1], STDOUT_FILENO);
		close(pip[0]);
		execlp( "docker", "docker", "inspect", "--format", "{{.State.Pid}}", cont_ID, (char*)0 );
		exit(1);
	} else {       // parent
		int status=-10;
		wait(&status);
		if (status != 0){
			fprintf(stderr, "Docker terminated with error code %d\n", status);
			exit(1);
		}
		close(pip[1]);
		read(pip[0], output, sizeof(output));
		sscanf(output, "%ld", (long int*)&cont_pid);
	}

	return cont_pid;
}



/* 
 * Prints on screen the usage of command
 * 
 * */
static void usage_print()
{
	printf("Usage: %s [options] <container ID>\n", program_invocation_short_name);
	puts(" -h, --help              display this help and exit");
	puts(" -v, --version           show version");
	puts(" -u, --user=<user>       login as user <user>");
	puts(" -d, --directory=<dir>   change directory to <directory>");
}



/*
 * Parses the command given by the user.
 * In practice it changes global variables.
 * 
 * */
static void parse_command(int argc, char *argv[]){
	
	int opt = 0;
	
	static const struct option long_opts[] = {
		{ "help", 		no_argument, 		0, 'h' },
		{ "version", 	no_argument, 		0, 'v' },
		{ "user", 		required_argument,	0, 'u' },
		{ "directory", 	required_argument, 	0, 'd' },
		{ 0, 0, 0, 0 }
	};

	// Parse flags
	while ((opt = getopt_long(argc, argv, "vhu:d:", long_opts, NULL)) != -1)
	{
		switch (opt)
		{
			case 'h':
				usage_print();
				exit(0);
			case 'v':
				puts(VERSION);
				exit(0);
			case 'u':
				if (optarg) {
					if (strlen(optarg) < 1) {
						fprintf(stderr, "A user must be specified\n");
						exit(1);
					}
					strcpy(user, optarg);
				}
				break;
			case 'd':
				if (optarg) {
					if (strlen(optarg) < 1) {
						fprintf(stderr, "A directory must be specified\n");
						exit(1);
					}
					strcpy(dir, optarg);
				}
				break;
			default:
				usage_print();
				exit(1);
		}
	}
	
	
	// Parse container ID
	strcpy(cont_ID, argv[argc-1]);
	if	( argc==1 || strcmp(cont_ID, dir)==0 || strcmp(cont_ID, user)==0)
	{
		fprintf(stderr, "No container ID given. Use -h for help\n");
		exit(1);
	}
	cont_pid = container_pid(cont_ID);
	if ( cont_pid < 0) {
		exit(1);
	}
	
}




/*
 * Opens a specified namespace
 * 
 * */
static void open_namespace(char* ns_name) {

	struct namespace_info *ns = namespace_table;
	while ( ns->name != NULL ) {
		if (ns->name == ns_name)
		{
			char pathbuf[PATH_MAX];
			int fd = -1;
			snprintf(pathbuf, sizeof(pathbuf), "/proc/%u/%s", cont_pid, ns->file);
			if ((fd = open(pathbuf, O_RDONLY)) == -1) {
				fprintf(stderr, "Trying to open namespace '%s': %s\n", ns->file, strerror(errno));
				exit(1);
			}
			
			if (setns(fd, 0) < 0) {
				fprintf(stderr, "Trying to set namespace '%s': %s\n", ns->file, strerror(errno));
				exit(1);
			}
			return;
		}
		ns++;
	}
	exit(1);
}



int main (int argc, char *argv[]) {

	// Parse command
	parse_command(argc, argv);
	
	// Open namespaces
	//open_namespace("user");
	open_namespace("ipc");
	open_namespace("uts");
	open_namespace("net");
	open_namespace("pid");
	open_namespace("mnt");

	// Change directory
	chdir(dir);
	
	// Change user
	if (strlen(user)>0)
		execlp( "su", "su", user, (char*)0 );

	// Run cell
	exec_shell();
	return 0;
}

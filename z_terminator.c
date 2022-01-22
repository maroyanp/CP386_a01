#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {

	system("./z_creator &");

	system("ps -l");
	system("kill -9 $(ps -e -l | grep -w Z | tr -s ' ' | cut -d ' ' -f5)");
	system("echo");
	system("echo The updated list of processes and their status is:")
	system("ps -l");

	return 0;
}

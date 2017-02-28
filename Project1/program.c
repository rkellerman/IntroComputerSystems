/*
 * program.c
 *
 *  Created on: Feb 27, 2017
 *      Author: RyanMini
 */


/*
 * PartA.c
 *
 *  Created on: Feb 20, 2017
 *      Author: RyanMini
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define SEM_NAME "/sem"

int maxA(char * file){



	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	int max = array[0];
	int j;
	for (j = 0; j < i - 1; j++){
		//printf("%d\n", array[j]);
		if (array[j] > max){
			max = array[j];
		}
	}

	fclose(ptr_file);

	printf("The maximum is %d\n", max);
	return max;
}

int sumA(char * file){

	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	fclose(ptr_file);

	int sum = 0;
	int j;
	for (j = 0; j < i - 1; j++){
		//printf("%d\n", array[j]);
		sum += array[j];
	}

	printf("The sum is %d\n", sum);
	return sum;
}

int minA(char * file){

	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	fclose(ptr_file);

	int min = array[0];
	int j;
	for (j = 0; j < i - 1; j++){
		//printf("%d\n", array[j]);
		if (array[j] < min){
			min = array[j];
		}
	}

	printf("The minimum is %d\n", min);
	return min;
}

int maxB(char * file){

	sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

	const int SIZE = 4096;
	const char * name = "MEM";
	int shm_fd;
	void * ptr;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	fclose(ptr_file);

	// printf("There are %d items in the list\n", i-1);

	// printf("I am the parent, my PID is:  %d\n", getpid());

	// as parent in max function, write the first element to the shared memory as a temp item
	sprintf(ptr, "%d", array[0]);

	int j;
	for (j = 0; j < i-1; j++){

		int status;
		pid_t pid = fork();

		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process
			//printf("I am child number %i, my pid is %d, my parent is %d\n", j+1, getpid(), getppid());
			sem_wait(sem);
			int current = atoi((char*)ptr);
			if (array[j] > current){
				sprintf(ptr, "%d", array[j]);
			}
			else {
				// do nothing
			}
			sem_post(sem);
		}
		else { // parent process
			waitpid(pid, &status, NULL);
			//printf("Reaped...\n");
			exit(0);
		}
	}

	int max = atoi((char*)ptr);
	printf("The maximum is %d\n", max);
	sem_close(sem);
	return max;


}

int minB(char * file){

	sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

	const int SIZE = 4096;
	const char * name = "MEM";
	int shm_fd;
	void * ptr;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	fclose(ptr_file);

	// printf("There are %d items in the list\n", i-1);

	// printf("I am the parent, my PID is:  %d\n", getpid());

	// as parent in max function, write the first element to the shared memory as a temp item
	sprintf(ptr, "%d", array[0]);

	int j;
	for (j = 0; j < i-1; j++){

		int status;
		pid_t pid = fork();

		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process
			// printf("I am child number %i, my pid is %d, my parent is %d\n", j+1, getpid(), getppid());

			sem_wait(sem);
			int current = atoi((char*)ptr);
			if (array[j] < current){
				sprintf(ptr, "%d", array[j]);
			}
			else {
				// do nothing
			}
			sem_post(sem);
		}
		else { // parent process
			waitpid(pid, &status, NULL);
			//printf("Reaped...\n");
			exit(0);
		}
	}

	int min = atoi((char*)ptr);
	printf("The minimum is %d\n", min);
	sem_close(sem);
	return min;
}

int sumB(char * file){

	sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

	const int SIZE = 4096;
	const char * name = "MEM";
	int shm_fd;
	void * ptr;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	fclose(ptr_file);

	// printf("There are %d items in the list\n", i-1);

	// printf("I am the parent, my PID is:  %d\n", getpid());

	// as parent in max function, write the first element to the shared memory as a temp item
	sprintf(ptr, "%d", 0);

	int j;
	for (j = 0; j < i-1; j++){

		int status;
		pid_t pid = fork();

		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process
			// printf("I am child number %i, my pid is %d, my parent is %d\n", j+1, getpid(), getppid());
			sem_wait(sem);
			int current = atoi((char*)ptr);
			current += array[j];
			sprintf(ptr, "%d", current);
			sem_post(sem);
		}
		else { // parent process
			waitpid(pid, &status, NULL);
			// printf("Reaped...\n");
			exit(0);
		}
	}

	int sum = atoi((char*)ptr);
	printf("The sum is %d\n", sum);
	sem_close(sem);

	return sum;
}

int maxC(char * file){

	sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

	const int SIZE = 4096;
	const char * name = "MEM";
	int shm_fd;
	void * ptr;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	fclose(ptr_file);

	// printf("There are %d items in the list\n", i-1);

	// printf("I am the parent, my PID is:  %d\n", getpid());

	sprintf(ptr, "%d", array[0]);

	int j;
	for (j = 1; j < i-1; j++){

		int status;
		pid_t pid = fork();

		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process

			sem_wait(sem);
			if (array[j] > atoi((char*)ptr)){
				sprintf(ptr, "%d", array[j]);
				//printf("%d\n", array[j]);
			}
			else {
				// do nothing
			}
			sem_post(sem);

			// do some work and then exit(0)
			exit(0);
		}
		else { // parent process
			// just spawn all the child processes
		}
	}

	for (j = 0; j < i-1; j++){
		wait(NULL);
	}

	// parent process leaves the loop and then releases the result
	int max = atoi((char*)ptr);
	printf("The max is %d\n", max);
	sem_close(sem);

	return max;
}

int minC(char * file){

	sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

	const int SIZE = 4096;
	const char * name = "MEM";
	int shm_fd;
	void * ptr;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	fclose(ptr_file);

	// printf("There are %d items in the list\n", i-1);

	// printf("I am the parent, my PID is:  %d\n", getpid());

	sprintf(ptr, "%d", array[0]);

	int j;
	for (j = 1; j < i-1; j++){

		int status;
		pid_t pid = fork();

		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process

			sem_wait(sem);
			if (array[j] < atoi((char*)ptr)){
				sprintf(ptr, "%d", array[j]);
				//printf("%d\n", array[j]);
			}
			else {
				// do nothing
			}
			sem_post(sem);

			// do some work and then exit(0)
			exit(0);
		}
		else { // parent process
			// just spawn all the child processes
		}
	}

	for (j = 0; j < i-1; j++){
		wait(NULL);
	}

	// parent process leaves the loop and then releases the result
	int min = atoi((char*)ptr);
	printf("The minimum is %d\n", min);
	sem_close(sem);

	return min;
}

int sumC(char * file){

	sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

	const int SIZE = 4096;
	const char * name = "MEM";
	int shm_fd;
	void * ptr;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	fclose(ptr_file);

	// printf("There are %d items in the list\n", i-1);

	// printf("I am the parent, my PID is:  %d\n", getpid());

	sprintf(ptr, "%d", 0);

	int j;
	for (j = 0; j < i-1; j++){

		int status;
		pid_t pid = fork();

		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process

			sem_wait(sem);
			sprintf(ptr, "%d", array[j] + atoi((char*)ptr));
			sem_post(sem);
			// do some work and then exit(0)
			exit(0);
		}
		else { // parent process
			// just spawn all the child processes
		}
	}

	for (j = 0; j < i-1; j++){
		wait(NULL);
	}

	// parent process leaves the loop and then releases the result
	int sum = atoi((char*)ptr);
	printf("The sum is %d\n", sum);
	sem_close(sem);

	return sum;
}

int maxD(char * file){

	sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

	const int SIZE = 4096;
	const char * name = "MEM";
	const char * name2 = "MEM2";
	int shm_fd, shm_fd2;
	void * ptr;
	void * ptr2;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	shm_fd2 = shm_open(name2, O_CREAT | O_RDWR, 0666);

	ftruncate(shm_fd, SIZE);
	ftruncate(shm_fd2, SIZE);

	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	ptr2 = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd2, 0);

	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	fclose(ptr_file);
	// printf("Boi\n");
	// each child can create at most 2 children, so the structure grows like a binary tree

	sprintf(ptr2, "%d", array[0]);
	// printf("Boi\n");

	int index = 0;
	// sprintf(ptr, "%d", 0);

	// printf("Boi\n");

	//printf("The value of i is %d\n", i);

	while (1){

		int status;
		// int current = atoi((char*)ptr);

		if (2*index + 1 > i - 2){
			// printf("%d\n", index);
			if (index > i - 2){
				// extraneous node, do nothing
				exit(0);
			}
			//printf("I am a node, my index is %d and my value is %d\n", index, array[index]);
			sem_wait(sem);
			if (atoi((char*)ptr2) < array[index]){
				sprintf(ptr2, "%d", array[index]);
			}
			sem_post(sem);
			if (index == i - 2){
				sleep(1);
				int max = atoi((char*)ptr2);
				printf("The maximum is %d\n", max);
				sem_close(sem);
				return max;
			}

			exit(0);


		}
		// sprintf(ptr, "%d", current + 2);

		pid_t pid2;
		pid_t pid1;

		pid1 = fork();

		if (pid1 < 0){
			printf("Forking error...\n");

		}
		else if (pid1 == 0){
			// child, aka left node
			index = 2*index + 1;
		}
		else {  // parent process
			pid2 = fork();

			if (pid2 < 0){
				printf("Forking error...\n");
			}
			else if (pid2 == 0){
				// child, aka right node
				index = 2*index + 2;
			}
			else {

				// printf("%d\n", index);
				// parent process, aka current node
				// do some work, and then break
				//printf("I am a node, my index is %d and my value is %d\n", index, array[index]);
				sem_wait(sem);
				if (atoi((char*)ptr2) < array[index]){
					sprintf(ptr2, "%d", array[index]);
				}
				sem_post(sem);
				if (index == 0){
					int k;
					for (k = 1; k < i - 1; k++){
						wait(NULL);
						// printf("Reaped %d\n", k);
					}
					break;
				}
				exit(0);
			}
		}
	}

	if (index == 0){
		exit(0);
	}
	exit(-1);


}

int minD(char * file){

	sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

	const int SIZE = 4096;
	const char * name = "MEM";
	const char * name2 = "MEM2";
	int shm_fd, shm_fd2;
	void * ptr;
	void * ptr2;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	shm_fd2 = shm_open(name2, O_CREAT | O_RDWR, 0666);

	ftruncate(shm_fd, SIZE);
	ftruncate(shm_fd2, SIZE);

	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	ptr2 = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd2, 0);

	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	fclose(ptr_file);
	// printf("Boi\n");
	// each child can create at most 2 children, so the structure grows like a binary tree

	sprintf(ptr2, "%d", array[0]);
	// printf("Boi\n");

	int index = 0;
	// sprintf(ptr, "%d", 0);

	// printf("Boi\n");

	//printf("The value of i is %d\n", i);

	while (1){

		int status;
		// int current = atoi((char*)ptr);

		if (2*index + 1 > i - 2){
			// printf("%d\n", index);
			if (index > i - 2){
				// extraneous node, do nothing
				exit(0);
			}
			//printf("I am a node, my index is %d and my value is %d\n", index, array[index]);
			sem_wait(sem);
			if (atoi((char*)ptr2) > array[index]){
				sprintf(ptr2, "%d", array[index]);
			}
			sem_post(sem);
			if (index == i - 2){
				sleep(1);
				int min = atoi((char*)ptr2);
				printf("The minimum is %d\n", min);
				sem_close(sem);
				return min;
			}

			exit(0);


		}
		// sprintf(ptr, "%d", current + 2);

		pid_t pid2;
		pid_t pid1;

		pid1 = fork();

		if (pid1 < 0){
			printf("Forking error...\n");

		}
		else if (pid1 == 0){
			// child, aka left node
			index = 2*index + 1;
		}
		else {  // parent process
			pid2 = fork();

			if (pid2 < 0){
				printf("Forking error...\n");
			}
			else if (pid2 == 0){
				// child, aka right node
				index = 2*index + 2;
			}
			else {

				// printf("%d\n", index);
				// parent process, aka current node
				// do some work, and then break
				// printf("I am a node, my index is %d and my value is %d\n", index, array[index]);
				sem_wait(sem);
				if (atoi((char*)ptr2) > array[index]){
					sprintf(ptr2, "%d", array[index]);
				}
				sem_post(sem);
				if (index == 0){
					int k;
					for (k = 1; k < i - 1; k++){
						wait(NULL);
						// printf("Reaped %d\n", k);
					}
					break;
				}
				exit(0);
			}
		}
	}

	if (index == 0){
		exit(0);
	}
	exit(-1);

}

int sumD(char * file){

	sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

	const int SIZE = 4096;
	const char * name = "MEM";
	const char * name2 = "MEM2";
	int shm_fd, shm_fd2;
	void * ptr;
	void * ptr2;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	shm_fd2 = shm_open(name2, O_CREAT | O_RDWR, 0666);

	ftruncate(shm_fd, SIZE);
	ftruncate(shm_fd2, SIZE);

	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	ptr2 = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd2, 0);

	FILE * ptr_file;
	char line[1000];

	ptr_file = fopen(file, "r");

	if (!ptr_file){
		return -1;
	}

	int * array = (int*)malloc(sizeof(int));
	int i = 1;


	while(fgets(line, 1000, ptr_file)){

		if (line[strlen(line) - 1] != '\n'){
			// im stupid
			//line[strlen(line)] == '\0';
		}
		else {
			line[strlen(line)-1] = '\0';
		}

		int value = atoi(line);

		array = (int*)realloc(array, i*sizeof(int));
		array[i-1] = value;
		i++;
	}

	fclose(ptr_file);
	// printf("Boi\n");
	// each child can create at most 2 children, so the structure grows like a binary tree

	sprintf(ptr2, "%d", 0);
	// printf("Boi\n");

	int index = 0;
	// sprintf(ptr, "%d", 0);

	// printf("Boi\n");

	//printf("The value of i is %d\n", i);

	while (1){

		int status;
		// int current = atoi((char*)ptr);

		if (2*index + 1 > i - 2){
			// printf("%d\n", index);
			if (index > i - 2){
				// extraneous node, do nothing
				exit(0);
			}
			//printf("I am a node, my index is %d and my value is %d\n", index, array[index]);
			sem_wait(sem);
			sprintf(ptr2, "%d", array[index] + atoi((char*)ptr2));
			sem_post(sem);

			if (index == i - 2){
				sleep(1);
				int sum = atoi((char*)ptr2);
				printf("The sum is %d\n", sum);
				sem_close(sem);
				return sum;
			}

			exit(0);


		}
		// sprintf(ptr, "%d", current + 2);

		pid_t pid2;
		pid_t pid1;

		pid1 = fork();

		if (pid1 < 0){
			printf("Forking error...\n");

		}
		else if (pid1 == 0){
			// child, aka left node
			index = 2*index + 1;
		}
		else {  // parent process
			pid2 = fork();

			if (pid2 < 0){
				printf("Forking error...\n");
			}
			else if (pid2 == 0){
				// child, aka right node
				index = 2*index + 2;
			}
			else {

				// printf("%d\n", index);
				// parent process, aka current node
				// do some work, and then break
				// printf("I am a node, my index is %d and my value is %d\n", index, array[index]);

				sem_wait(sem);
				sprintf(ptr2, "%d", array[index] + atoi((char*)ptr2));
				sem_post(sem);

				if (index == 0){
					int k;
					for (k = 1; k < i - 1; k++){
						wait(NULL);
						// printf("Reaped %d\n", k);
					}
					break;
				}
				exit(0);
			}
		}
	}

	if (index == 0){
		exit(0);
	}
	exit(-1);


}

int test(){




	return 0;
}

int main(void){



	int max_A = maxA("test2.txt");
	int min_A = minA("test2.txt");
	int sum_A = sumA("test2.txt");

	int max_B = maxB("test2.txt");
	int min_B = minB("test2.txt");
	int sum_B = sumB("test2.txt");

	int max_C = maxC("test2.txt");
	int min_C = minC("test2.txt");
	int sum_C = sumC("test2.txt");


	int max_D = maxD("test2.txt");
	int min_D = minD("test2.txt");
	int sum_D = sumD("test2.txt");

	return 0;
}





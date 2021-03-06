/*
 * program.c
 *
 *  Created on: Feb 21, 2017
 *      Author: RyanMini
 */
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>

#define SEM_NAME "/sem"

int max(int * array, int size){

	int max = array[0];
	int i;
	for (i = 0; i < size; i++){
		//printf("%d, ", array[i]);
		if (array[i] > max){
			max = array[i];
		}
	}
	//printf("\n");
	return max;
}

int min(int * array, int size){

	int min = array[0];
	int i;
	for (i = 0; i < size; i++){
		if (array[i] < min){
			min = array[i];
		}
	}
	return min;
}

int sum(int * array, int size){

	int sum = 0;
	int i;
	for (i = 0; i < size; i++){
		sum += array[i];
	}
	return sum;

}

int maxA(char * file, FILE * output){



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


int sumA(char * file, FILE * output){

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


int minA(char * file, FILE * output){

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


int maxB(char * file, char * buffer){

	// sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

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

	i = i-1;

	// printf("There are %d elements\n", i);

	int iterations = 10;
	int num = i / iterations;

	// printf("num is %d\n", num);

	int rem = i % num;
	if (rem != 0){
		iterations++;
	}

	// printf("%d iterations\n", iterations);
	// printf("%d remaining\n", rem);

	sprintf(ptr, "%d", array[0]);

	int j;
	int last = 0;
	for (j = 0; j < iterations; j++){

		int status;
		pid_t pid = fork();

		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process
			sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
			//printf("I go from %d to %d\n", last, last + num);

			int * subArray;
			int subMax;

			if ((j == iterations - 1) && (rem != 0)){
				subArray = (int*)malloc(rem*sizeof(int));
				memcpy(&subArray[0], &array[last], rem * sizeof(int));
				subMax = max(subArray, rem);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
			}
			else {
				subArray = (int*)malloc(num*sizeof(int));
				memcpy(&subArray[0], &array[last], num * sizeof(int));
				subMax = max(subArray, num);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
			}

			last += num;


			// sem_wait(sem);
			int current = atoi((char*)ptr);
			if (subMax > current){
				sprintf(ptr, "%d", subMax);
			}
			else {
				// do nothing
			}
			// sem_post(sem);
		}
		else { // parent process
			sleep(1);
			exit(0);
		}
	}

	int max = atoi((char*)ptr);
	printf("The maximum is %d\n", max);
	// sem_close(sem);
	return max;
}

int minB(char * file, char * buffer){


	// sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

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

	i = i-1;

	// printf("There are %d elements\n", i);

	int iterations = 10;
	int num = i / iterations;

	// printf("num is %d\n", num);

	int rem = i % num;
	if (rem != 0){
		iterations++;
	}

	// printf("%d iterations\n", iterations);
	// printf("%d remaining\n", rem);

	sprintf(ptr, "%d", array[0]);

	int j;
	int last = 0;
	for (j = 0; j < iterations; j++){

		int status;
		pid_t pid = fork();



		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process
			sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
			//printf("I am child number %i, my pid is %d, my parent is %d\n", j+1, getpid(), getppid());
			//printf("I go from %d to %d\n", last, last + num);

			int * subArray;
			int subMin;

			if (j == iterations - 1 && rem != 0){
				subArray = (int*)malloc(rem*sizeof(int));
				memcpy(&subArray[0], &array[last], rem * sizeof(int));
				subMin = min(subArray, rem);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
			}
			else {
				subArray = (int*)malloc(num*sizeof(int));
				memcpy(&subArray[0], &array[last], num * sizeof(int));
				subMin = min(subArray, num);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
			}

			last += num;


			// sem_wait(sem);
			int current = atoi((char*)ptr);
			if (subMin < current){
				sprintf(ptr, "%d", subMin);
			}
			else {
				// do nothing
			}
			// sem_post(sem);
		}
		else { // parent process
			sleep(1);
			exit(0);
		}
	}

	int min = atoi((char*)ptr);
	printf("The minimum is %d\n", min);
	// sem_close(sem);
	return min;
}

int sumB(char * file, char * buffer){

	// sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

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

	i = i-1;

	// printf("There are %d elements\n", i);

	int iterations = 10;
	int num = i / iterations;

	// printf("num is %d\n", num);

	int rem = i % num;
	if (rem != 0){
		iterations++;
	}

	// printf("%d iterations\n", iterations);
	// printf("%d remaining\n", rem);

	sprintf(ptr, "%d", 0);

	int j;
	int last = 0;
	for (j = 0; j < iterations; j++){

		int status;
		pid_t pid = fork();




		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process

			sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
			//printf("I am child number %i, my pid is %d, my parent is %d\n", j+1, getpid(), getppid());
			//printf("I go from %d to %d\n", last, last + num);

			int * subArray;
			int subSum;

			if (j == iterations - 1 && rem != 0){
				subArray = (int*)malloc(rem*sizeof(int));
				memcpy(&subArray[0], &array[last], rem * sizeof(int));
				subSum = sum(subArray, rem);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
			}
			else {
				subArray = (int*)malloc(num*sizeof(int));
				memcpy(&subArray[0], &array[last], num * sizeof(int));
				subSum = sum(subArray, num);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
			}

			last += num;


			// sem_wait(sem);
			int current = atoi((char*)ptr);
			sprintf(ptr, "%d", subSum + current);

			// sem_post(sem);
		}
		else { // parent process
			sleep(1);
			exit(0);
		}
	}

	int sum = atoi((char*)ptr);
	printf("The sum is %d\n", sum);
	// sem_close(sem);
	return sum;
}

int maxC(char * file, char * buffer){

	// sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

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

	i = i-1;

	// printf("There are %d elements\n", i);

	int iterations = 10;
	int num = i / iterations;

	// printf("num is %d\n", num);

	int rem = i % num;
	if (rem != 0){
		iterations++;
	}

	// printf("%d iterations\n", iterations);
	// printf("%d remaining\n", rem);

	int j;
	int last = 0;

	sprintf(ptr, "%d", array[0]);
	for (j = 0; j < iterations; j++){

		int status;
		pid_t pid = fork();

		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process

			sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
			//printf("Hi, my PID is %d, and my parent PID is %d\n", getpid(), getppid());
			int * subArray;
			int subMax;

			if (j == iterations - 1 && rem != 0){
				subArray = (int*)malloc(rem*sizeof(int));
				memcpy(&subArray[0], &array[last], rem * sizeof(int));
				subMax = max(subArray, rem);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
			}
			else {
				subArray = (int*)malloc(num*sizeof(int));
				memcpy(&subArray[0], &array[last], num * sizeof(int));
				subMax = max(subArray, num);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
			}

			// sem_wait(sem);
			int current = atoi((char*)ptr);
			if (subMax > current){
				sprintf(ptr, "%d", subMax);
			}
			else {
				// do nothing
			}
			// sem_post(sem);

			// do some work and then exit(0)
			exit(0);
		}
		else { // parent process
			last += num;
		}
	}

	for (j = 0; j < iterations; j++){
		wait(NULL);
	}

	// parent process leaves the loop and then releases the result
	int max = atoi((char*)ptr);
	printf("The maximum is %d\n", max);
	//sem_close(sem);

	return max;
}

int minC(char * file, char * buffer){

	// sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

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

	i = i-1;

	// printf("There are %d elements\n", i);

	int iterations = 10;
	int num = i / iterations;

	// printf("num is %d\n", num);

	int rem = i % num;
	if (rem != 0){
		iterations++;
	}

	// printf("%d iterations\n", iterations);
	// printf("%d remaining\n", rem);

	int j;
	int last = 0;

	sprintf(ptr, "%d", array[0]);
	for (j = 0; j < iterations; j++){

		int status;
		pid_t pid = fork();

		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process

			sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
			int * subArray;
			int subMin;

			if (j == iterations - 1 && rem != 0){
				subArray = (int*)malloc(rem*sizeof(int));
				memcpy(&subArray[0], &array[last], rem * sizeof(int));
				subMin = min(subArray, rem);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
			}
			else {
				subArray = (int*)malloc(num*sizeof(int));
				memcpy(&subArray[0], &array[last], num * sizeof(int));
				subMin = min(subArray, num);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
			}

			// sem_wait(sem);
			int current = atoi((char*)ptr);
			if (subMin < current){
				sprintf(ptr, "%d", subMin);
			}
			else {
				// do nothing
			}
			// sem_post(sem);

			// do some work and then exit(0)
			exit(0);
		}
		else { // parent process
			last += num;
		}
	}

	for (j = 0; j < i-1; j++){
		wait(NULL);
	}

	// parent process leaves the loop and then releases the result
	int min = atoi((char*)ptr);
	printf("The minimum is %d\n", min);
	//sem_close(sem);

	return min;
}

int sumC(char * file, char * buffer){

	// sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

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

	i = i-1;

	// printf("There are %d elements\n", i);

	int iterations = 10;
	int num = i / iterations;

	// printf("num is %d\n", num);

	int rem = i % num;
	if (rem != 0){
		iterations++;
	}

	// printf("%d iterations\n", iterations);
	// printf("%d remaining\n", rem);

	int j;
	int last = 0;

	sprintf(ptr, "%d", 0);
	for (j = 0; j < iterations; j++){

		int status;
		pid_t pid = fork();

		if (pid < 0){
			printf("Error forking\n");
			exit(-1);
		}
		else if (pid == 0){  // child process

			sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
			int * subArray;
			int subSum;

			if (j == iterations - 1 && rem != 0){
				subArray = (int*)malloc(rem*sizeof(int));
				memcpy(&subArray[0], &array[last], rem * sizeof(int));
				subSum = sum(subArray, rem);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
			}
			else {
				subArray = (int*)malloc(num*sizeof(int));
				memcpy(&subArray[0], &array[last], num * sizeof(int));
				subSum = sum(subArray, num);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
			}

			// sem_wait(sem);
			int current = atoi((char*)ptr);
			sprintf(ptr, "%d", subSum + current);

			// sem_post(sem);

			// do some work and then exit(0)
			exit(0);
		}
		else { // parent process
			last += num;
		}
	}

	for (j = 0; j < i-1; j++){
		wait(NULL);
	}

	// parent process leaves the loop and then releases the result
	int sum = atoi((char*)ptr);
	printf("The sum is %d\n", sum);
	//sem_close(sem);

	return sum;
}

int maxD(char * file, char * buffer){

	// sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

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
	// each child can create at most 2 children, so the structure grows like a binary tree

	i = i-1;

	// printf("There are %d elements\n", i);

	int iterations = 6;
	int num = i / iterations;

	// printf("num is %d\n", num);

	int rem = i % num;
	if (rem != 0){
		iterations++;
	}

	// printf("%d iterations\n", iterations);
	// printf("%d remaining\n", rem);

	int j;
	int last = 0;
	int index = 0;

	sprintf(ptr2, "%d", array[0]);

	while (1){

		int status;
		// int current = atoi((char*)ptr);

		if (2*index + 1 > iterations ){
			// printf("%d\n", index);
			if (index >= iterations){
				// extraneous node, do nothing
				sleep(1);
				exit(0);
			}


			int * subArray;
			int subMax;
			last = index * num;

			if (index == iterations - 1 && rem != 0){
				subArray = (int*)malloc(rem*sizeof(int));
				memcpy(&subArray[0], &array[last], rem * sizeof(int));
				subMax = max(subArray, rem);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
				// printf("MAX HERE IS %d\n", subMax);
			}
			else {
				subArray = (int*)malloc(num*sizeof(int));
				memcpy(&subArray[0], &array[last], num * sizeof(int));
				subMax = max(subArray, num);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
				// printf("MAX HERE IS %d\n", subMax);
			}

			//sem_wait(sem);
			int current = atoi((char*)ptr2);
			// printf("------ %d or %d -----\n", current, subMax);

			if (current < subMax){
				sprintf(ptr2, "%d", subMax);
			}

			//sem_post(sem);
			if (index == iterations - 1){
				sleep(1);
				int max = atoi((char*)ptr2);
				printf("The maximum is %d\n", max);
				//sem_close(sem);
				return max;
			}
			sleep(1);
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
			sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
		}
		else {  // parent process
			pid2 = fork();

			if (pid2 < 0){
				printf("Forking error...\n");
			}
			else if (pid2 == 0){
				// child, aka right node
				index = 2*index + 2;
				sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
			}
			else {


				// printf("%d\n", index);
				// parent process, aka current node
				// do some work, and then break

				int * subArray;
				int subMax;
				last = index * num;

				if (index == iterations - 1 && rem != 0){
					subArray = (int*)malloc(rem*sizeof(int));
					memcpy(&subArray[0], &array[last], rem * sizeof(int));
					subMax = max(subArray, rem);
					// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
					// printf("MAX HERE IS %d\n", subMax);
				}
				else {
					subArray = (int*)malloc(num*sizeof(int));
					memcpy(&subArray[0], &array[last], num * sizeof(int));
					subMax = max(subArray, num);
					// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
					// printf("MAX HERE IS %d\n", subMax);
				}

				//sem_wait(sem);

				int current = atoi((char*)ptr2);
				// printf("------ %d or %d -----\n", current, subMax);

				if (current < subMax){
					sprintf(ptr2, "%d", subMax);
				}

				if (index == 0){
					int k;
					for (k = 1; k < i - 1; k++){
						wait(NULL);
						// printf("Reaped %d\n", k);
					}
					break;
				}
				sleep(1);
				exit(0);
			}
		}
	}

	if (index == 0){
		exit(0);
	}
	exit(-1);


}

int minD(char * file, char * buffer){

	// sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

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
	// each child can create at most 2 children, so the structure grows like a binary tree

	i = i-1;

	// printf("There are %d elements\n", i);

	int iterations = 6;
	int num = i / iterations;

	// printf("num is %d\n", num);

	int rem = i % num;
	if (rem != 0){
		iterations++;
	}

	// printf("%d iterations\n", iterations);
	// printf("%d remaining\n", rem);

	int j;
	int last = 0;
	int index = 0;

	sprintf(ptr2, "%d", array[0]);

	while (1){

		int status;
		// int current = atoi((char*)ptr);

		if (2*index + 1 > iterations ){
			// printf("%d\n", index);
			if (index >= iterations){
				// extraneous node, do nothing
				sleep(1);
				exit(0);
			}

			int * subArray;
			int subMin;
			last = index * num;

			if (index == iterations - 1 && rem != 0){
				subArray = (int*)malloc(rem*sizeof(int));
				memcpy(&subArray[0], &array[last], rem * sizeof(int));
				subMin = min(subArray, rem);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
				// printf("MAX HERE IS %d\n", subMax);
			}
			else {
				subArray = (int*)malloc(num*sizeof(int));
				memcpy(&subArray[0], &array[last], num * sizeof(int));
				subMin = min(subArray, num);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
				// printf("MAX HERE IS %d\n", subMax);
			}

			//sem_wait(sem);
			int current = atoi((char*)ptr2);
			// printf("------ %d or %d -----\n", current, subMax);

			if (current > subMin){
				sprintf(ptr2, "%d", subMin);
			}

			//sem_post(sem);
			if (index == iterations - 1){
				sleep(1);
				int min = atoi((char*)ptr2);
				printf("The minimum is %d\n", min);
				//sem_close(sem);
				return min;
			}
			sleep(1);
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
			sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
		}
		else {  // parent process
			pid2 = fork();

			if (pid2 < 0){
				printf("Forking error...\n");
			}
			else if (pid2 == 0){
				// child, aka right node
				index = 2*index + 2;
				sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
			}
			else {

				// printf("%d\n", index);
				// parent process, aka current node
				// do some work, and then break

				int * subArray;
				int subMin;
				last = index * num;

				if (index == iterations - 1 && rem != 0){
					subArray = (int*)malloc(rem*sizeof(int));
					memcpy(&subArray[0], &array[last], rem * sizeof(int));
					subMin = min(subArray, rem);
					// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
					// printf("MAX HERE IS %d\n", subMax);
				}
				else {
					subArray = (int*)malloc(num*sizeof(int));
					memcpy(&subArray[0], &array[last], num * sizeof(int));
					subMin = min(subArray, num);
					// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
					// printf("MAX HERE IS %d\n", subMax);
				}

				//sem_wait(sem);

				int current = atoi((char*)ptr2);
				// printf("------ %d or %d -----\n", current, subMax);

				if (current > subMin){
					sprintf(ptr2, "%d", subMin);
				}

				if (index == 0){
					int k;
					for (k = 1; k < i - 1; k++){
						wait(NULL);
						// printf("Reaped %d\n", k);
					}
					break;
				}
				sleep(1);
				exit(0);
			}
		}
	}

	if (index == 0){
		exit(0);
	}
	exit(-1);
}

int sumD(char * file, char * buffer){

	// sem_t * sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

	const int SIZE = 4096;
	const char * name = "MEM";
	const char * name2 = "MEM2";
	int shm_fd, shm_fd2;

	void * ptr2;

	shm_fd2 = shm_open(name2, O_CREAT | O_RDWR, 0666);

	ftruncate(shm_fd2, SIZE);

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
	// each child can create at most 2 children, so the structure grows like a binary tree

	i = i-1;

	// printf("There are %d elements\n", i);

	int iterations = 6;
	int num = i / iterations;

	// printf("num is %d\n", num);

	int rem = i % num;
	if (rem != 0){
		iterations++;
	}

	// printf("%d iterations\n", iterations);
	// printf("%d remaining\n", rem);

	int j;
	int last = 0;
	int index = 0;

	sprintf(ptr2, "%d", 0);

	while (1){

		int status;
		// int current = atoi((char*)ptr);

		if (2*index + 1 > iterations ){
			// printf("%d\n", index);
			if (index >= iterations){
				// extraneous node, do nothing
				sleep(1);
				exit(0);
			}

			int * subArray;
			int subSum;
			last = index * num;

			if (index == iterations - 1 && rem != 0){
				subArray = (int*)malloc(rem*sizeof(int));
				memcpy(&subArray[0], &array[last], rem * sizeof(int));
				subSum = sum(subArray, rem);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
				// printf("MAX HERE IS %d\n", subMax);
			}
			else {
				subArray = (int*)malloc(num*sizeof(int));
				memcpy(&subArray[0], &array[last], num * sizeof(int));
				subSum = sum(subArray, num);
				// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
				// printf("MAX HERE IS %d\n", subMax);
			}

			// sem_wait(sem);
			int current = atoi((char*)ptr2);
			// printf("------ %d or %d -----\n", current, subMax);

			sprintf(ptr2, "%d", subSum + current);

			// sem_post(sem);

			if (index == iterations - 1){
				sleep(1);
				/*
				int k;
				for (k = 1; k < iterations; k++){
					wait(NULL);
					printf("Oi\n");
					// printf("Reaped %d\n", k);
				}
				 */
				int sum = atoi((char*)ptr2);
				printf("The sum is %d\n", sum);
				// sem_close(sem);
				return sum;
			}
			sleep(1);
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
			sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
		}
		else {  // parent process
			pid2 = fork();

			if (pid2 < 0){
				printf("Forking error...\n");
			}
			else if (pid2 == 0){
				// child, aka right node
				index = 2*index + 2;
				sprintf(buffer, "%sHi, my PID is %d, and my parent PID is %d\n", buffer, getpid(), getppid());
			}
			else {

				// printf("%d\n", index);
				// parent process, aka current node
				// do some work, and then break

				int * subArray;
				int subSum;
				last = index * num;

				if (index == iterations - 1 && rem != 0){
					subArray = (int*)malloc(rem*sizeof(int));
					memcpy(&subArray[0], &array[last], rem * sizeof(int));
					subSum = sum(subArray, rem);
					// printf("I go from %d to %d, which is %d to %d\n", last, last + rem - 1, array[last], array[last + rem - 1]);
					// printf("MAX HERE IS %d\n", subMax);
				}
				else {
					subArray = (int*)malloc(num*sizeof(int));
					memcpy(&subArray[0], &array[last], num * sizeof(int));
					subSum = sum(subArray, num);
					// printf("I go from %d to %d, which is %d to %d\n", last, last + num - 1, array[last], array[last+num - 1]);
					// printf("MAX HERE IS %d\n", subMax);
				}

				// sem_wait(sem);

				int current = atoi((char*)ptr2);
				// printf("------ %d or %d -----\n", current, subMax);

				sprintf(ptr2, "%d", subSum + current);

				// sem_post(sem);

				sleep(1);
				exit(0);
			}
		}
	}

	if (index == 0){
		exit(0);
	}
	exit(-1);
}

int A(char * file){

	FILE * output = fopen("resultA.txt", "w");


	int max_A = maxA(file, output);
	int min_A = minA(file, output);
	int sum_A = sumA(file, output);

	fprintf(output, "Max = %d\nMin = %d\nSum = %d\n", max_A, min_A, sum_A);
	fclose(output);




}

int B(char * file){

	FILE * output = fopen("resultB.txt", "w");
	char buffer[10000];

	sprintf(buffer, "");


	int max_B = maxB(file, buffer);

	sprintf(buffer, "%s\nMax = %d\n\n", buffer, max_B);

	int min_B = minB(file, buffer);

	sprintf(buffer, "%s\nMin = %d\n\n", buffer, min_B);

	int sum_B = sumB(file, buffer);

	sprintf(buffer, "%s\nSum = %d\n\n", buffer, sum_B);

	fprintf(output, buffer);


	fclose(output);




}


int C(char * file){

	int shm_fd;
	char * buffer;

	shm_fd = shm_open("TEMP", O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, 10000);
	buffer = mmap(0, 10000, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	FILE * output = fopen("resultC.txt", "w");

	sprintf(buffer, "");

	int max_C = maxC(file, buffer);

	sprintf(buffer, "%s\nMax = %d\n\n", buffer, max_C);

	int min_C = minC(file, buffer);

	sprintf(buffer, "%s\nMin = %d\n\n", buffer, min_C);

	int sum_C = sumC(file, buffer);

	sprintf(buffer, "%s\nSum = %d\n\n", buffer, sum_C);

	fprintf(output, buffer);

	fclose(output);
}

int D(char * file){

	int shm_fd;
	char * buffer;

	shm_fd = shm_open("TEMP", O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, 10000);
	buffer = mmap(0, 10000, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	FILE * output = fopen("resultD.txt", "w");

	sprintf(buffer, "");

	int max_D = maxD(file, buffer);

	sprintf(buffer, "%s\nMax = %d\n\n", buffer, max_D);

	int min_D = minD(file, buffer);

	sprintf(buffer, "%s\nMin = %d\n\n", buffer, min_D);

	int sum_D = sumD(file, buffer);

	sprintf(buffer, "%s\nSum = %d\n\n", buffer, sum_D);

	fprintf(output, buffer);

	fclose(output);

}

int main(void){

	struct timeval t0, t1, t2, t3, t4;
	time_t curtime;

	gettimeofday(&t0, 0);

	A("test3.txt");

	gettimeofday(&t1, 0);

	B("test3.txt");

	gettimeofday(&t2, 0);

	C("test3.txt");

	gettimeofday(&t3, 0);

	D("test3.txt");

	gettimeofday(&t4, 0);

	long elapsedA = ((t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec)/1000;
	long elapsedB = ((t2.tv_sec-t1.tv_sec)*1000000 + t2.tv_usec-t1.tv_usec)/1000;
	long elapsedC = ((t3.tv_sec-t2.tv_sec)*1000000 + t3.tv_usec-t2.tv_usec)/1000;
	long elapsedD = ((t4.tv_sec-t3.tv_sec)*1000000 + t4.tv_usec-t3.tv_usec)/1000 - 3000;

	printf("A took %d milliseconds\n", elapsedA);
	printf("B took %d milliseconds\n", elapsedB);
	printf("C took %d milliseconds\n", elapsedC);
	printf("D took %d milliseconds\n", elapsedD);



	return 0;


}




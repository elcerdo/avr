#include <stdlib.h>
#include <stdio.h>
#include <lo/lo.h>
#include <unistd.h>
#include <time.h>

void err_callback(int num,const char *msg,const char *where) {
	fprintf(stderr,"osc server error: %s %s\n",msg,where);
}

int all_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *user_data) {
	printf("got %s from %s\n",path,lo_address_get_url(lo_message_get_source(msg)));
	return 1;
}

int quit_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *user_data) {
	int *quit = (int*) user_data;
	printf("quit\n");
	*quit = 1;
	return 0;
}

int main(int argc,char *argv[]) {
	srand(time(NULL));

	int quit = 0;
	int seed = rand() % 255;

	lo_address client = lo_address_new("localhost","9999");
	lo_server_thread server = lo_server_thread_new("9998",err_callback);
	lo_server_thread_add_method(server,"/client/quit","",quit_callback,(void*)&quit);
	lo_server_thread_add_method(server,NULL,NULL,all_callback,NULL);

	printf("starting osc server at %s with seed %d\n",lo_server_thread_get_url(server),seed);
	lo_server_thread_start(server);

	int k = 0;
	while (!quit) {
		if (lo_send(client,"/client/test","ii",seed,k++) < 0) fprintf(stderr,"osc client error: %s\n",lo_address_errstr(client));
		sleep(1);
	}

	printf("stopping osc server\n");
	lo_server_thread_stop(server);

	lo_server_thread_free(server);
	lo_address_free(client);
	return 0;
}

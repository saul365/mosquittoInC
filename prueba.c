
#include <mosquitto.h>
#include <stdio.h>

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)//keep edit
{
	if(message->payloadlen){
		printf("%s %s lol1\n", message->topic, message->payload);
	}else{
		printf("%s lol (null)\n", message->topic);
	}
	fflush(stdout);
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	int i;
	if(!result){
		/* Subscribe to broker information topics on successful connect. */
		mosquitto_subscribe(mosq, NULL, "$SYS/#", 2);
	}else{
		fprintf(stderr, "Connect failed\n");
	}
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
	int i;

	printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for(i=1; i<qos_count; i++){
		printf(", %d", granted_qos[i]);
	}
	printf("\n");
}


void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)//elim
{
	/* Pring all log messages regardless of level. */
	printf("%s lol\n", str);
}

int main(int argc, char *argv[])
{
	int i;
	char *host = "localhost";
	int port = 1884;
	int keepalive = 60;
	bool clean_session = true;
	struct mosquitto *mosq = NULL;
	int mid1=57;
	int* mid= & mid1;

	mosquitto_lib_init();
	mosq = mosquitto_new("C", clean_session, NULL);
	if(!mosq){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}
	mosquitto_log_callback_set(mosq, my_log_callback);
	mosquitto_connect_callback_set(mosq, my_connect_callback);
	mosquitto_message_callback_set(mosq, my_message_callback);
	mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);

	if(mosquitto_connect(mosq, host, port, keepalive)){
		fprintf(stderr, "Unable to connect.\n");
		return 1;
	}

	mosquitto_subscribe(mosq,mid,"algo",2);

	mosquitto_loop_forever(mosq, -1, 1);

	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return 0;
}
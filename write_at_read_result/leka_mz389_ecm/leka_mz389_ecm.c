#include "write_at_read_result.h"
#include <stdlib.h>

#define at_device "/dev/ttyUSB0"

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
char *ecm_at_cmd[] = {"at+cfun=1","at+cgact=1,1","at+zgact=1,1"};

	
int main()
{
	int fd;
	int error_number= 0;
	Pthread_Arg *pthread_arg;
	pthread_t tid;

	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	printf("This program updates last time at %s   %s\n",__TIME__,__DATE__);

	fd = open(at_device, O_RDWR | O_NOCTTY | O_NONBLOCK);

        if(fd < 0 )
        {
                fprintf(stderr,"open %s failed !",ttyname(fd));
                exit(1);
        }


        set_speed(fd,115200);
        if(set_parity(fd,8,1,'N') < 0)
        {
                printf("Set parity error");
                close(fd);
                exit(0);
        }

	pthread_arg = (Pthread_Arg *)malloc(sizeof(Pthread_Arg));
	pthread_arg->fd = fd;
	pthread_arg->cmd_number = 0;
	pthread_arg->mutex1 = &mutex1;
	pthread_arg->mutex2 = &mutex2;

	pthread_mutex_lock(pthread_arg->mutex1);
	pthread_mutex_lock(pthread_arg->mutex2);	
	
	if(0 != pthread_create(&tid, NULL, read_thread, pthread_arg))
        {
                perror("create_pthread fail");
                close(fd);
                exit(0);
        }
	sleep(3);	
	while(1)
	{
		if(error_number == -1)
			break;
		if(5 == error_number++)
                {
			printf("Send the at command process error!");
			pthread_arg->cmd_number = 0;
			//exit(1);
		}
		
		switch (pthread_arg->cmd_number)
		{
			case 0:
				pthread_mutex_unlock(pthread_arg->mutex1);
				at_cmd(fd,ecm_at_cmd[0]);
				printf("send at+cfun=1\n");
				pthread_mutex_lock(pthread_arg->mutex2);
				if(pthread_arg->cmd_number == 0 || pthread_arg->cmd_number == -1)
					continue;

			case 1: 
				pthread_mutex_unlock(pthread_arg->mutex1);
				printf("send at+cgact=1,1\n");
				at_cmd(fd,ecm_at_cmd[1]);
				pthread_mutex_lock(pthread_arg->mutex2);
				if(pthread_arg->cmd_number == 1 || pthread_arg->cmd_number == -1)
					continue;

			case 2: 
				pthread_mutex_unlock(pthread_arg->mutex1);
				printf("send at+zgact=1,1\n");
				at_cmd(fd,ecm_at_cmd[2]);
				pthread_mutex_lock(pthread_arg->mutex2);
				if(pthread_arg->cmd_number == 2 || pthread_arg->cmd_number == -1)
					continue;
				else
				{
					error_number = -1;
					break;
				}
			default:
				pthread_arg->cmd_number =0;
				printf("send again\n");
				continue;
		}
	}
	system("udhcpc -i eth2\n");
	sleep(3);
	printf("dialing success!\n");
	pthread_cancel(tid);
	close(fd);
	free(pthread_arg);
	pthread_mutex_destroy(mutex1);
	pthread_mutex_destroy(mutex2);
	return 0;
}


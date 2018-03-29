#include "write_at_read_result.h"
#define FALSE	-1
#define TRUE	0
#define UART_DEVICE     "/dev/ttyUSB2"

int speed_arr[] ={ B115200,B38400, B19200, B9600, B4800, B2400, B1200, B300, B115200,B38400, B19200, B9600,B4800, B2400, B1200, B300, };
int name_arr[] ={ 115200,38400, 19200, 9600, 4800, 2400, 1200, 300,115200, 38400, 19200, 9600, 4800, 2400,1200, 300, };
int set_parity(int fd,int databits,int stopbits,int parity)
{
	struct termios options;
	if (tcgetattr (fd, &options) != 0){
		perror ("SetupSerial 1");
		return (FALSE);
	}
	options.c_cflag &= ~CSIZE;
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input ->changed by kong*/     
	options.c_oflag  &= ~OPOST;   /*Output  ->changed by kong */
	switch (databits){
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag |= CS8;
			break;
		default:
			fprintf (stderr, "Unsupported data size\n");
			return (FALSE);
	}
	switch(parity)
	{
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;   /* Clear parity enable */
			options.c_iflag &= ~INPCK;    /* Enable parity checking */
			break;
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB);
			options.c_iflag |= INPCK;     /* Disnable parity checking */
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;	  /* Enable parity */
			options.c_cflag &= ~PARODD;
			options.c_iflag |= INPCK;     /* Disnable parity checking */
			break;
		case 'S':
		case 's':
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		default:
			fprintf (stderr, "Unsupported parity\n");
			return (FALSE);
	}
	switch(stopbits)
	{
		case 1:
			options.c_cflag &= ~CSTOPB;
			break;
		case 2:
			options.c_cflag |= CSTOPB;
			break;
		default:
			fprintf (stderr, "Unsupported stop bits\n");
			return (FALSE);
	}
	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;
	tcflush (fd, TCIFLUSH);
	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 1;     /* Update the options and do it NOW */
	if (tcsetattr (fd, TCSANOW, &options) != 0)
	{
		perror ("SetupSerial 3");
		return (FALSE);
	}
	return (TRUE);
}

void set_speed(int fd,int speed)
{
	int i;
	int status;
	struct termios opt;
	tcgetattr(fd,&opt);
	for(i=0;i<sizeof(speed_arr)/sizeof(int);i++)
	{
		if(speed == name_arr[i])
		{
			tcflush(fd,TCIOFLUSH);	
			cfsetispeed(&opt,speed_arr[i]);
			cfsetospeed(&opt,speed_arr[i]);
			status = tcsetattr(fd,TCSANOW,&opt);
			if(status != 0)
			{
				perror("tcsetattr fd failed!");
				return;
			}
			tcflush(fd,TCIOFLUSH);
		}
	}
}

void send_at(int fd,char *cmd, int send_len)
{
	int ret_len = 0;
	ret_len = write(fd, cmd, send_len);
	if(ret_len != send_len)
		perror("send cmd failed !");
}

void *read_thread(Pthread_Arg *arg)
{
	int len = 0;
	int time_number=0;
	int ip_check=0;
	char recv_msg[1024] = {0};
	char *check;	
	Pthread_Arg *pthread_arg = arg;
	//printf("read pthread...\n");
	while(1)
	{
		pthread_mutex_lock(pthread_arg->mutex1);
		time_number = 0;
		while(time_number <= 3)
		{
		
			memset(recv_msg, 0, sizeof(recv_msg));
			if((len = read(pthread_arg->fd, recv_msg, sizeof(recv_msg)-1)) > 0)
			{
				recv_msg[len] = '\0';
				printf("recv msg>>%s\n", recv_msg);
				if(strstr(recv_msg,"ERROR"))
				{	
					printf("An error is detected!\n");
					fflush(stdout);
					break;
				}	

				if(strstr(recv_msg,"OK") && (pthread_arg->cmd_number != 1))
				{	
					printf("check OK!\n");
					(pthread_arg->cmd_number)++;
					fflush(stdout);
					break;
				}

				if(pthread_arg->cmd_number == 1)
				{	
					if(strstr(recv_msg,"IP"))
					{
						ip_check=1;
						printf("check IP\n");
					}
					if(strstr(recv_msg,"OK") && (ip_check == 1))
					{
						(pthread_arg->cmd_number)++;
						ip_check = 0;
						fflush(stdout);
						break;
					}
				}
				
				fflush(stdout);
			}
			sleep(1);
			time_number++;
		}
		pthread_mutex_unlock(pthread_arg->mutex2);
		sleep(2);
	}
}



int at_cmd(int fd, char *at_cmd){
	char * dev = NULL;
	int len_cmd = 0;

	char cmd[255] = {0};
	if((len_cmd = strlen(at_cmd)) >= 250){
		printf("AT CMD is too long!");
		exit(0);
	}
	strncpy(cmd, at_cmd, len_cmd);
	

	//printf("device=%s\n",argv[1]);
	cmd[len_cmd] = '\r';
	cmd[len_cmd+1] = '\n';
	cmd[len_cmd+2] = '\0';
	len_cmd += 2;
		
	
	send_at(fd,cmd, len_cmd);
	

	sleep(1);
	return 0;
}

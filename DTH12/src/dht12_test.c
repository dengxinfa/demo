#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
	int fd = open("/dev/dht12.0",O_RDONLY);
	if(fd<0)
		printf("open error\n");
	char buf[5];
	int number=100,i;
	if(argc != 2 || (strncmp(argv[1],"all",3) && strncmp(argv[1],"hum",3) && strncmp(argv[1],"tem",3))){
		printf("please input: get_env <all | hum | tem>!\n");
		goto end;
	}
	while(number--)
	{
		i=read(fd,buf,5);
		if(i == 5 && buf[4] == (buf[0]+buf[1]+buf[2]+buf[3])){
			if(!strncmp(argv[1],"all",3)){
			printf( "Humidity:%d.%d%% Temperature:%d.%dC\n",buf[0],buf[1],buf[2],buf[3]);
			break;
			}
			else if(!strncmp(argv[1],"hum",3)){
                        printf( "Humidity:%d.%d%%\n",buf[0],buf[1]);
                        break;
                	}
			else if(!strncmp(argv[1],"tem",3)){
                        printf( "Temperature:%d.%dC\n",buf[2],buf[3]);
                        break;
                	}
		}
		usleep(1000);
	}
	if(number <= 0)
		printf("get env failed!\n");
end:	close(fd);
	return 0;

}

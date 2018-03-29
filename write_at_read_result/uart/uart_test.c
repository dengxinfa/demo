    #include <stdio.h>    //标准输入输出定义  
    #include <stdlib.h>   //标准函数库定义  
    #include <unistd.h>   //Unix标准函数定义  
    #include <sys/types.h>  
    #include <sys/stat.h>  
    #include <fcntl.h>    //文件控制定义  
    #include <termios.h>  //POSIX中断控制定义  
    #include <errno.h>    //错误号定义  
       
       
    //定义波特率数组  
    int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300};  
    int name_arr[] = {115200,38400, 19200, 9600, 4800, 2400, 1200, 300};  
       
    //打开串口  
    int open_port(void)  
    {  
        int fd;     //串口的标识符  
        //O_NOCTTY用来告诉Linux这个程序不会成为“控制终端”  
        //O_NDELAY用来告诉Linux这个程序不关心DCD信号  
        fd=open("/dev/ttyS0",O_RDWR | O_NOCTTY | O_NDELAY);  
        if(fd == -1)  
        {  
            //不能打开串口  
            perror("open_port: Unable to open /dev/ttyS1 -");  
            return(fd);  
        }  
        else  
        {  
            fcntl(fd, F_SETFL, 0);  
            printf("open ttys1 .....\n");  
            return(fd);  
        }  
    }  
       
    //设置波特率  
    void set_speed_and_parity(int fd, int speed)  
    {  
        int i=0;        //设置循环标志——注意不要在for内设置，否则会出错  
        struct termios Opt; //定义termios结构  
        if(tcgetattr(fd,&Opt)!=0)  
        {  
            perror("tcgetattr fd");  
            return;  
        }  
        for(i = 0; i < 8 ; i++)  
            {  
                if(speed == name_arr[i])  
                {     
                        tcflush(fd, TCIOFLUSH);  
                        cfsetispeed(&Opt, speed_arr[i]);  
                        cfsetospeed(&Opt, speed_arr[i]);  
                /*tcsetattr函数标志： 
                TCSANOW：立即执行而不等待数据发送或者接受完成。 
                TCSADRAIN：等待所有数据传递完成后执行。 
                TCSAFLUSH：Flush input and output buffers and make the change 
                */  
                        if(tcsetattr(fd, TCSANOW, &Opt) != 0)  
                        {     
                            perror("tcsetattr fd");  
                            return;  
                        }  
                        tcflush(fd, TCIOFLUSH);  
                }  
            }  
        //设置奇偶校验——默认8个数据位、没有校验位  
            Opt.c_cflag &= ~PARENB;  
            Opt.c_cflag &= ~CSTOPB;  
            Opt.c_cflag &= ~CSIZE;  
            Opt.c_cflag |= CS8;  
    }  
       
    /* 
    //设置奇偶校验——默认8个数据位、没有校验位 
    int set_parity() 
    { 
        Opt.c_options.c_cflag &= ~PARENB 
        options.c_cflag &= ~CSTOPB 
        options.c_cflag &= ~CSIZE; 
        Opt.c_cflag |= CS8; 
    } 
    */  
       
    int main(void)  
    {  
        int fd;  
        int nread,i;  
        char buff[]="Hello\n";  
        //打开串口  
        if((fd=open_port())<0)  
            {  
                perror("open_port error");  
                return 0;  
            }  
        //设置波特率和校验位  
        set_speed_and_parity(fd,115200);  
        //设置校验位  
        //set_parity();  
       
        printf("fd=%d\n",fd);  
       
        //尝试先写内容  
        for(int i=0;i<100;i++)  
        {  
            nread=write(fd,buff,8);  
            //nread=read(fd,buff,8);  
            printf("nread=%d,%s\n",nread,buff);  
        }  
        //关闭串口  
        close(fd);  
        return 0;  
    }  

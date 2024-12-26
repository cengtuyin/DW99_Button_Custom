#include<stdio.h>
#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<sys/fcntl.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<errno.h>
#include<sys/types.h>
#include <arpa/inet.h>
#include<string>
#include<map>
#include<stdlib.h>
#include <sys/timeb.h>
#include <thread>
#include <pthread.h>
/**
 *@用与获取cmd指令运算的结果，并将cmd执行后的输出结果保存到code中
 *@param[0] cmd 需要运行的指令
 *@param[1] code 将指令的输出结果保存在该字符串中
 **/
void aiden_system(const char *cmd, std::string & code)
{
	char result[10240] = { 0 };
	char buf[1024] = { 0 };
	FILE *fp = NULL;

	if ((fp = popen(cmd, "r")) == NULL)
	{
		// printf("popen error!\n");
		return;
	}

	while (fgets(buf, sizeof(buf), fp))
	{
		strcat(result, buf);
	}

	pclose(fp);
	// printf("result: %s\n", result);
	std::string code_ = std::string(result);
	code = std::string(result);
}
int button1;
int button2;
int twostate;
int IFtwobuttonclick(){
	if(twostate>=1){
		twostate=0;
		button1=0;
		button2=0;
		return 1;
	}
	//printf("验证双长按%d %d %d\n",button1,button2,button1 and button2);
    if(button1 and button2){
    	printf("双按键长按事件\n");
		std::string temp;
		aiden_system("sh /data/adb/modules/DW99_Button/脚本/双按键长按.sh",temp);
		twostate++;
		return 1;
    }
    return 0;
}
int click_num=0;
void* click(void* arg){
    usleep(300000);
    if(click_num>=2){
        printf("左键双击事件\n");
		std::string temp;
		aiden_system("sh /data/adb/modules/DW99_Button/脚本/左键双击.sh",temp);
	}else{
	    printf("左键单击事件\n");
		std::string temp;
		aiden_system("sh /data/adb/modules/DW99_Button/脚本/左键单击.sh",temp);
	}
	click_num=0;
}
long clicktime;
void ShiJianPanDuan(int type){
	timeb t;
    ftime(&t);
    long nowtime = t.time * 1000 + t.millitm;
	if(type==1){
		clicktime=nowtime;
		button1=type;
		return;
	}
	if(nowtime-clicktime<=500 and type==0){
		clicktime=0;
		if(click_num==0){
		    click_num++;
		    pthread_t threads;
            pthread_create(&threads, NULL, click, NULL);
		}else{
		    click_num++;
		}
	}else if(nowtime-clicktime>500 and clicktime!=0 and type==0){
		clicktime=0;
		if(IFtwobuttonclick()){
			return;
		}
		printf("左键长按事件\n");
		std::string temp;
		aiden_system("sh /data/adb/modules/DW99_Button/脚本/左键长按.sh",temp);
	}
	button1=type;
}
int KEY_POWER_click_num;
void* KEY_POWER_click(void* arg){
    usleep(300000);
    if(KEY_POWER_click_num>=2){
        printf("电源键双击事件\n");
		std::string temp;
		aiden_system("sh /data/adb/modules/DW99_Button/脚本/电源键双击.sh",temp);
	}else{
	    printf("电源键单击事件\n");
		std::string temp;
		aiden_system("sh /data/adb/modules/DW99_Button/脚本/电源键单击.sh",temp);
	}
	KEY_POWER_click_num=0;
}
long KEY_POWER_clicktime;
void KEY_POWER_ShiJianPanDuan(int type){
	timeb t;
    ftime(&t);
    long KEY_POWER_nowtime = t.time * 1000 + t.millitm;
	if(type==1){
		KEY_POWER_clicktime=KEY_POWER_nowtime;
		button2=type;
		return;
	}
	if(KEY_POWER_nowtime-KEY_POWER_clicktime<=500 and type==0){
	    KEY_POWER_clicktime=0;
		if(KEY_POWER_click_num==0){
		    KEY_POWER_click_num++;
		    pthread_t thread;
            pthread_create(&thread, NULL, KEY_POWER_click, NULL);
		}else{
		    KEY_POWER_click_num++;
		}
	}else if(KEY_POWER_nowtime-KEY_POWER_clicktime>500 and KEY_POWER_clicktime!=0 and type==0){
		KEY_POWER_clicktime=0;
		if(IFtwobuttonclick()){
			return;
		}
		printf("电源键长按事件\n");
		std::string temp;
		aiden_system("sh /data/adb/modules/DW99_Button/脚本/电源键长按.sh",temp);
	}
	button2=type;
}

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <cstring>
int main() {
    const char* device_path = "/dev/input/event6";
    int fd = open(device_path, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error opening device " << device_path << ": " << strerror(errno) << std::endl;
        return -1;
    }

    if (ioctl(fd, EVIOCGRAB, 1) == -1) {
        std::cerr << "Error grabbing device " << device_path << ": " << strerror(errno) << std::endl;
        close(fd);
        return -1;
    }

    struct input_event ie;

    while (true) {
        ssize_t bytes = read(fd, &ie, sizeof(struct input_event));
        if (bytes == (ssize_t)-1) {
            perror("read");
            ioctl(fd, EVIOCGRAB, 0); // 释放设备以便在出错时其他进程可以访问
            close(fd);
            return -1;
        } else if (bytes == sizeof(struct input_event)) {
            if (ie.type == EV_KEY && ie.code == KEY_F9) {
            	ShiJianPanDuan(ie.value);
                if (ie.value == 1) {
                    std::cout << "左键按下" << std::endl;
                } else if (ie.value == 0) {
                    std::cout << "左键抬起" << std::endl;
                }
                
                
                // 注意：这里我们没有对非KEY_F9事件进行任何处理，因此它们会自然传递
            } else if (ie.type == EV_KEY && ie.code == KEY_POWER) {
            	KEY_POWER_ShiJianPanDuan(ie.value);
                if (ie.value == 1) {
                    std::cout << "电源键按下" << std::endl;
                } else if (ie.value == 0) {
                    std::cout << "电源键抬起" << std::endl;
                }
                
                /*std::cout << device_path << ": EV_SYN       SYN_REPORT           " 
                          << std::hex << ie.value << std::dec << std::endl;*/
            }
            // 注意：这里没有处理EV_SYN等其他类型的事件，它们将不被拦截
            //write(fd, &ie, sizeof(ie));
        }
    }

    // 理论上，这个ioctl调用可能永远不会被执行，因为上面的while循环是无限的。
    // 在实际应用中，你可能需要添加一种方式来优雅地退出循环（例如，通过信号处理）。
    ioctl(fd, EVIOCGRAB, 0); // 释放设备
    close(fd);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <string.h> 
#include <unistd.h> 
#include <ncurses.h>
#include <pthread.h>
#include "chatshm.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
WINDOW * sw1, *sw2, *sw3, *sw4;
CHAT_INFO *cf = NULL;

void *read_messages(void *arg)                   //������ �Է� �����带 �����ϴ� �Լ�
{        
    char curmsg[40];
    /** for subwin 2 **/
    if(cf->user_index > 2) {mvwprintw(sw4, 9, 9, "too many users"); exit(0);}
    strcpy(cf->userlist[cf->user_index], (char*) arg);  //�Է¹��� arg�� ������Ͽ� �߰���ŵ�ϴ�
    cf->user_index++;                            //���� ������ �����ϱ� ���� �ε����� ������ŵ�ϴ�

    while (1) 
    {
        /** for subwin 3 **/
        echo();                                             
        mvwgetnstr(sw3, 5, 5, curmsg, 40);      //����� �Է��� ����������3�� ���̵��� echo�� �����մϴ�
        if(strcmp(curmsg, "quit\n") == 0) break;
        wclear(sw3);                            //�Է��� ���� ������ ä�����α׷� Ư���� 
        mvwprintw(sw3, 1, 1, "input line\n");   //������� �ϱ⿡ clear��Ų�� ���������츦 �ٽ� �׷��ݴϴ�
        box(sw3, 0, 0);
        wrefresh(sw3);

        pthread_mutex_lock(&mutex);             //�Է¹��� �޼����� �迭�� ���� �� ���ؽ� ���� �̴ϴ�
        if(cf->message_index >= 10)             //�޼��� 10���� ��� ����
        {
            for(int i = 0; i < 9; i++)          //�� ���Ҹ� ��ĭ�� ������ ����
            {
                strcpy(cf->messages[i], cf->messages[i+1]); 
            }                                    //�迭�� �������� ���� �޼����� �����մϴ�
            snprintf(cf->messages[9], sizeof(char) * 100,   
                    "   [%s]: %s\n", (char *)arg, curmsg);    
        }   
        else 
        {                   //�޼����� 10�� �����̸� messgae_index�� ������Ű�� ���������� �����մϴ�
            snprintf(cf->messages[cf->message_index], sizeof(char) * 100, 
                    "   [%s]: %s\n", (char *)arg, curmsg);
            cf->message_index++;
        }
        pthread_mutex_unlock(&mutex);                   //���� ������ ������ ���� Ǳ�ϴ�
    }
    return NULL;
}

void *display_messages(void *arg)              //��� �����带 �����ϴ� �Լ�
{
    /** for subwin1 and subwin2 **/
    while(1) 
    {
        pthread_mutex_lock(&mutex);            //��½� mutex ���� �̴ϴ�
        for (int i = 0, h = 2; i < cf->message_index; i++, h++) 
        {                                       //����������1�� �޼��� �迭�� ����մϴ�
            mvwprintw(sw1, h, 1, "%s\n", cf->messages[i]);      
        }   
        wrefresh(sw1);
        for (int i = 0, h = 2; i < 3; i++, h++) 
        {                                       //����������2�� ���� ����� ����մϴ�
            if(strcmp(cf->userlist[i],"") == 0) continue;
            mvwprintw(sw2, h, 5, "%s..\n", cf->userlist[i]);   
        }
        wrefresh(sw2);
        usleep(100000);
        pthread_mutex_unlock(&mutex);           //����� ������ unlock �մϴ�
    }
    return NULL;
}

int main(int argc, char *argv[]) {  //argv[1]�� ����id�� �Է� �޽��ϴ�.
    initscr();                      //ncurses �ʱ�ȭ �ܰ��Դϴ�.
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    int height, width;
    getmaxyx(stdscr, height, width);
    int sub_height = height / 2;
    int sub_width = width / 2;
    sw1 = newwin(sub_height, sub_width, 0, 0);          //WINDOW *�� ������ ����Ǿ��� 
    sw2 = newwin(sub_height, sub_width, 0, sub_width);  //����������� 4���� ���� �߽��ϴ�.
    sw3 = newwin(sub_height, sub_width, sub_height, 0);
    sw4 = newwin(sub_height, sub_width, sub_height, sub_width);
    box(sw1, 0, 0);                                     //���������� �� �׵θ��� �����߽��ϴ�.
    box(sw2, 0, 0);
    box(sw3, 0, 0);
    box(sw4,0, 0);
    mvwprintw(sw1, 1, 1, "Chatting..\n");
    mvwprintw(sw2, 1, 1, "Loged in Users\n");
    mvwprintw(sw3, 1, 1, "input line\n");
    mvwprintw(sw4, 1, 1, "Chat Info\n");
    mvwprintw(sw4, 5, 5, "unlucky kakaotalk");
    
    wrefresh(sw1); wrefresh(sw2);wrefresh(sw3);wrefresh(sw4);   //�� ������� �ݿ��ϱ����� refresh

    pthread_t usrthread, display_thread;
    int shmid; 
    bool init_flag = true;          
	void *shmaddr = (void *)0;
    shmid = shmget((key_t)3936, sizeof(CHAT_INFO), 0666|IPC_CREAT|IPC_EXCL);  //�����޸𸮸� �����մϴ�  
    if (shmid < 0)
    {   
        init_flag = false;                      //���� �����ڰ� �ƴϸ� init_flag�� false�� �ٲ��ݴϴ�
        shmid = shmget((key_t)3936, sizeof(CHAT_INFO),0666); 
        shmaddr = shmat(shmid, (void *)0, 0666);
        if (shmaddr < 0)
        {   
            perror("shmat attach is failed : ");
            exit(0);
        }
    }

    shmaddr = shmat(shmid, (void *)0, 0666);
    cf = (CHAT_INFO *)shmaddr;                //����ü cf�� shamaddr�� ���� ��ŵ�ϴ�

    if (init_flag)                     //�����޸𸮸� attach�ϴ� ������ ��� shmid��
    {                                  //����ġ ���� sig fault�� �����ϱ� ���� ��� �������� �ʱ�ȭ�� �����ݴϴ�
        cf->user_index = 0; 
        cf->message_index = 0; 
        for (int i = 0; i < 3; i++) strcpy(cf->userlist[i], "");
        for (int i = 0; i < 10; i++) strcpy(cf->messages[i], "");
    }
                                                //�� ���μ����� usrthread�μ� �޼����� �а�
    pthread_create(&usrthread, NULL, read_messages, (void *) argv[1]);          
    pthread_create(&display_thread, NULL, display_messages, (void *) argv[1]);  
                                                //display_thread�μ� �޼����� ����ϵ��� ��Ƽ�������մϴ�
    pthread_join(usrthread, NULL);
    pthread_join(display_thread, NULL);

    return 0;
}

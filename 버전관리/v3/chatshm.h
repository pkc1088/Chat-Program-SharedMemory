#ifndef __CHAT_SHARE_MEMORY_H__
#define __CHAT_SHARE_MEMORY_H__
#include <pthread.h>
#include <ncurses.h>
typedef struct chatInfo{
    char userlist[3][10];   //������� 
    int user_index;         //���� �ε����� ����   
    char messages[10][100]; //�޼��� ����
    int message_index;      //�޼��� �ε����� ����
} CHAT_INFO;

#endif//__CHAT_SHARE_MEMORY_H__






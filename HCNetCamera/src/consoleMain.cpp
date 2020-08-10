 /*
* Copyright(C) 2010,Hikvision Digital Technology Co., Ltd
*
* File   name��consoleMain.cpp
* Discription��
* Version    ��1.0
* Author     ��panyadong
* Create Date��2010_3_25
* Modification History��
*/

#ifndef __APPLE__

#include <stdio.h>
#include <iostream>
#include "GetStream.h"
#include "public.h"
#include "ConfigParams.h"
#include "Alarm.h"
#include "CapPicture.h"
#include "playback.h"
#include "Voice.h"
#include "tool.h"
#include <string.h>

#include <getopt.h>
#include <stdlib.h>
using namespace std;

#ifndef PROGRAM_NAME
#define PROGRAM_NAME  "HCNetCamera"
#endif

#define DIR_MAX_LEN     128
#define DEFAULT_DIR_NAME        "./pic"

static struct option long_options[] =
{
    {"address", required_argument, 0, 'a'},
    {"port", required_argument, 0, 'p'},
    {"password", required_argument, 0, 'w'},
    {"username", required_argument, 0, 'u'},
    {"time-interval", required_argument, 0, 't'},
    {"dir", required_argument, 0, 'd'},
    {"pic-size", required_argument, 0, 's'},
    {"pic-quality", required_argument, 0, 'q'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0},
};

static void usage(void);
static void run(void);

const char *program_name = PROGRAM_NAME;

int main(int argc, char *argv[])
{
    WORD dev_port                                      = 0;
    char dev_address[NET_DVR_DEV_ADDRESS_MAX_LEN]      = {0};
    char dev_user_name[NET_DVR_LOGIN_USERNAME_MAX_LEN] = {0};
    char dev_password[NET_DVR_LOGIN_PASSWD_MAX_LEN]    = {0};
    char pic_dir[DIR_MAX_LEN]                          = {0};
    int  c = 0;
    int  ret = 0;
    unsigned int  time_interval = 0;
    int pic_size = -1;
    int pic_quality = -1;
    program_name = argv[0];
    while((c = getopt_long(argc, argv, "a:p:w:u:ht:d:s:q:", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'a':
                strncpy(dev_address, optarg, NET_DVR_DEV_ADDRESS_MAX_LEN-1);
                break;

            case 'p':
                dev_port = atoi(optarg);
                break;

            case 'w':
                strncpy(dev_password, optarg, NET_DVR_LOGIN_PASSWD_MAX_LEN-1);
                break;

            case 'u':
                strncpy(dev_user_name, optarg, NET_DVR_LOGIN_USERNAME_MAX_LEN-1);
                break;

            case 't':
                time_interval = atoi(optarg);
                break;

            case 'd':
                strncpy(pic_dir, optarg, DIR_MAX_LEN);
                break;

            case 's':   // pic size
                pic_size = atoi(optarg);
                break;

            case 'q':   // pic quality
                pic_quality = atoi(optarg);
                break;

            case 'h':
                usage();
                break;

            default:
                //LOG_INFO("unhandled param %d\n", (char)c);
                usage();
                break;
        }
    }

    require_action_log(strlen(dev_address) != 0, exit(1), "地址未指定，\"%s -h\"可以获得更多帮助信息。\n", program_name);
    require_action_log(strlen(dev_user_name) != 0, exit(1), "用户名未指定，\"%s -h\"可以获得更多帮助信息。\n", program_name);
    require_action_log(strlen(dev_password) != 0, exit(1), "密码未指定，\"%s -h\"可以获得更多帮助信息。\n", program_name);
    require_action_log(dev_port > 0, exit(1), "端口号未指定或指定错误，\"%s -h\"可以获得更多帮助信息。\n", program_name);

    if (time_interval == 0)
    {
        time_interval = 60;
        LOG_INFO("抓图时间间隔使用默认值 %d\n", time_interval);
    }

    if (strlen(pic_dir) == 0)
    {
        strncpy(pic_dir, DEFAULT_DIR_NAME, strlen(DEFAULT_DIR_NAME));
        LOG_INFO("图片存储在默认的目录：%s\n", pic_dir);
    }

    if (pic_size == -1)
    {
        pic_size = 0;
    }

    if (pic_quality == -1)
    {
        pic_quality = 2;
    }

    NET_DVR_Init();
    Demo_SDK_Version();
    NET_DVR_SetLogToFile(3, "./sdkLog");
    //Login device
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
    struLoginInfo.bUseAsynLogin = false;

    struLoginInfo.wPort = dev_port;
    memcpy(struLoginInfo.sDeviceAddress, dev_address, NET_DVR_DEV_ADDRESS_MAX_LEN);
    memcpy(struLoginInfo.sUserName, dev_user_name, NAME_LEN);
    memcpy(struLoginInfo.sPassword, dev_password, NAME_LEN);

    int lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    require_action(lUserID >= 0, ShowSDK_LastErrMsg(); exit(1));

    CAP_PICTURE_ARGS capPictureArg = {0};
    capPictureArg.user_id = lUserID;
    capPictureArg.device_info = &struDeviceInfoV40;
    capPictureArg.pathname = pic_dir;
    capPictureArg.time_interval = time_interval;
    capPictureArg.pic_size = pic_size;
    capPictureArg.pic_quality = pic_quality;
    ret = CapPictureStart(&capPictureArg);
    require_action(ret == HPR_OK, exit(1));

    run();

    //logout
    NET_DVR_Logout_V30(lUserID);
    NET_DVR_Cleanup();
    return 0;
}

static void usage(void)
{
    printf("Usage: %s OPTIONS\n", program_name);
    printf("长选项的强制性参数对于短选项也是必需的。\n");
    printf("必须选项：\n");
    printf("  -a, --address=地址        网络摄像头的地址\n");
    printf("  -p, --port=端口           网络摄像头的端口\n");
    printf("  -u, --username=用户名     网络摄像头的用户名\n");
    printf("  -w, --password=密码       网络摄像头的密码\n");
    printf("\n");
    printf("可选选项：\n");
    printf("  -t, --time-interval=time  抓图时间间隔，取值需要大于0，默认是60s\n");
    printf("  -d, --dir=dir             图像的存储目录，默认是./pic\n");
    printf("  -q, --pic-quality=quality 图片质量系数，0 最好，1 较好，2 一般。默认值为2。\n"
           "                            若参数非法，则取值为2。\n");
    printf("  -s, --pic-size=size       图片尺寸，详细请参考SDK手册或README。默认值为0。\n"
           "                            如果参数非法，或者摄像头不支持，SDK会自动选择一个默认值。\n");
    printf("  -h, --help                打印帮助信息，并终止程序\n");
    printf("\n");
    printf("\n");
    printf("示例：\n");
    printf("  %s -a 127.0.0.1 -p 12306 -u root -w 123456\n", program_name);
    printf("  %s --address=127.0.0.1 --port=12306 --username=root --password=123456\n", program_name);
    exit(1);
}

static void run(void)
{
    while(1)
    {
        sleep(1);
    }
}

#endif

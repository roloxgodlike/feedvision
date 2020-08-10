 /*
* Copyright(C) 2010,Hikvision Digital Technology Co., Ltd
*
* File   name CapPicture.cpp
* Discription
* Version     1.0
* Author      panyd
* Create Date 2010_3_25
* Modification History
*/

#include "public.h"
#include "CapPicture.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include "tinyxml2.h"

static pthread_t tid = 0;

char* GeneratePictureName(char *buf, int buflen, char *tag, int channel)
{
    require_action(buf != NULL, return NULL);
    if (tag == NULL)
        tag = (char*)"";

    // 文件名格式：yyyymmdd-hhmmss-cccc.jpeg
    // 文件名固定长度 25
    #define FIXED_LENGTH    25
    int taglen = strlen(tag);
    if (taglen + FIXED_LENGTH + 1> buflen)
    {
        LOG_ERR("buf length %d is too short, at least require %d\n", buflen, taglen + FIXED_LENGTH + 1);
        return NULL;
    }

    time_t ntime = time(NULL);
    struct tm *ptm = localtime(&ntime);
    require_action(ptm != NULL, return NULL);

    snprintf(buf, buflen, "%04d%02d%02d-%02d%02d%02d-%s%03d.jpeg",
                ptm->tm_year + 1900, ptm->tm_mon, ptm->tm_mday,
                ptm->tm_hour, ptm->tm_min, ptm->tm_sec,
                tag, channel);
    return buf;
}

int CapPicture(long user_id, LONG channel, const char *picName, int size, int quality)
{
    FUNC_ENTRY();
    NET_DVR_JPEGPARA strPicPara = {0};
    int iRet;
    strPicPara.wPicQuality = quality;
    strPicPara.wPicSize = size;
    iRet = NET_DVR_CaptureJPEGPicture(user_id, channel, &strPicPara, (char *)picName);
    if (!iRet)
    {
        ShowSDK_LastErrMsg();
        LOG_ERR("%s capure fail\n", picName);
        return HPR_ERROR;
    }
    return HPR_OK;
}

/*
抓取图像需要的参数：
1、登录ID
2、摄像头通道（ch）
3、图像的存储路径
4、文件名（此项无需外部提供）
*/
void * CapPictureThread(void *arg)
{
    FUNC_ENTRY();
    CAP_PICTURE_ARGS *parg = (CAP_PICTURE_ARGS*)arg;
    LONG userID    = parg->user_id;
    int StartChan  = parg->device_info->struDeviceV30.byStartChan;
    int EndChan    = StartChan + parg->device_info->struDeviceV30.byChanNum;
    int StartDChan = parg->device_info->struDeviceV30.byStartDChan;
    int EndDChan   = parg->device_info->struDeviceV30.byIPChanNum +
                     parg->device_info->struDeviceV30.byHighDChanNum * 256 +
                     StartDChan;

    LOG_VAR_INT(userID);
    LOG_VAR_INT(StartChan);
    LOG_VAR_INT(StartDChan);
    LOG_VAR_INT(EndChan);
    LOG_VAR_INT(EndDChan);

    unsigned int time_interval = parg->time_interval;
    LOG_VAR_INT(time_interval);

    int pic_size = parg->pic_size;
    int pic_quality = parg->pic_quality;
    LOG_VAR_INT(pic_size);
    LOG_VAR_INT(pic_quality);

    #define PATH_BUFF_LEN        2048
    int channel;
    char full_name[PATH_BUFF_LEN] = {0};
    int  path_len = 0;
    strncpy(full_name, parg->pathname, PATH_BUFF_LEN);
    strncat(full_name, "/", 1);
    LOG_INFO("picture storage path: %s\n", full_name);
    path_len = strlen(full_name);
    LOG_INFO("path_len = %d\n", path_len);

    if (access(full_name, F_OK) != 0)
    {
        int ret = mkdir(full_name, S_IRWXU | S_IRWXG);
        require_action_log(ret == 0, exit(1), "创建文件夹 %s 失败\n", full_name);
    }

    while(1)
    {
        char *ret;
        // 每隔固定时间抓取所有通道的图像
        // 之后加入通道的bitmap，可以指定抓取的通道

        // 抓取模拟通道
        for (channel = StartChan; channel < EndChan; channel++)
        {
            memset(full_name + path_len, 0, PATHNAME_LEN - path_len);
            ret = GeneratePictureName(full_name + path_len, PATHNAME_LEN - path_len, (char*)"Chan", channel);
            require_action_log(ret != NULL, continue, "generate picture name fail, please check\n");
            LOG_INFO("start capture %s\n", full_name);

            CapPicture(userID, channel, full_name, pic_size, pic_quality);
        }

        // 抓取数字通道
        for (channel = StartDChan; channel < EndDChan; channel++)
        {
            memset(full_name + path_len, 0, PATHNAME_LEN - path_len);
            ret = GeneratePictureName(full_name + path_len, PATHNAME_LEN - path_len, (char*)"DChan", channel);
            require_action_log(ret != NULL, continue, "generate picture name fail, please check\n");
            LOG_INFO("start capture %s\n", full_name);

            CapPicture(userID, channel, full_name, pic_size, pic_quality);
        }

        sleep(time_interval);
    }

    return NULL;
}

int CapPictureStart(CAP_PICTURE_ARGS *args)
{
    require_action(args != NULL, return HPR_ERROR);
    require_action(args->device_info != NULL, return HPR_ERROR);
    require_action(args->pathname != NULL, return HPR_ERROR);
    require_action(args->time_interval > 0, return HPR_ERROR);

    int ret = HPR_OK;

    ret = pthread_create(&tid, NULL, CapPictureThread, (void*)args);
    require_action(ret == 0, return HPR_ERROR);
    return HPR_OK;
}

using namespace tinyxml2;

char *get_JpegCaptureAbility_xml_input(int channel, char *buf, int len)
{
    require_action(buf != NULL, return NULL);
    memset(buf, 0, len);
    int buf_len = snprintf(buf, len-1,
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\
        <JpegCaptureAbility version=\"2.0\">\n\
            <channelNO>%d</channelNO>\n\
        </JpegCaptureAbility>", channel);
    if (buf_len == len-1)
    {
        LOG_ERR("buf is too short!!!\n");
    }
    return buf;
}

char *get_JpegCaptureAbility_xml_output(int userid, int channel, char *xml_output, int len)
{
    require_action(userid >= 0, return NULL);
    require_action(xml_output != NULL, return NULL);

    #define XML_INPUT_LEN 512
    char xml_input[XML_INPUT_LEN] = {0};

    char *xml_ret = get_JpegCaptureAbility_xml_input(channel, xml_input, XML_INPUT_LEN);
    require_action(xml_ret != NULL, return NULL);
    //LOG_INFO("xml_input = %s\n", xml_input);

    memset(xml_output, 0, len);
    int ret = NET_DVR_GetDeviceAbility(userid, DEVICE_JPEG_CAP_ABILITY,
                                       xml_input, strlen(xml_input),
                                       xml_output, len);
    require_action(ret, ShowSDK_LastErrMsg(); return NULL);
    return xml_output;
}

int print_JpegCaptureAbility(int user_id, NET_DVR_DEVICEINFO_V40 *device_info)
{
    require_action(user_id >= 0, return HPR_ERROR);
    require_action(device_info != NULL, return HPR_ERROR);

    #define XML_LEN     2048
    char xml[XML_LEN] = {0};
    char *xml_ret;

    int i = 0;
    int StartChan  = device_info->struDeviceV30.byStartChan;
    int EndChan    = StartChan + device_info->struDeviceV30.byChanNum;
    int StartDChan = device_info->struDeviceV30.byStartDChan;
    int EndDChan   = device_info->struDeviceV30.byIPChanNum +
                     device_info->struDeviceV30.byHighDChanNum * 256 +
                     StartDChan;

    XMLDocument doc;

    int chan_max = EndChan > EndDChan ? EndChan : EndDChan;
    int chan_min = StartChan < StartDChan ? StartChan : StartDChan;

#if 0
    LOG_VAR_INT(StartChan);
    LOG_VAR_INT(EndChan);
    LOG_VAR_INT(StartDChan);
    LOG_VAR_INT(EndDChan);
    LOG_VAR_INT(chan_max);
    LOG_VAR_INT(chan_min);
#endif

    for (i = chan_min; i < chan_max; i++)
    {
        if ((i < StartChan || i >= EndChan) && (i < StartDChan || i >= EndDChan))
        {
            continue;
        }

        char *xml_ret = get_JpegCaptureAbility_xml_output(user_id, i, xml, XML_LEN);
        //LOG_INFO("xml_output = %s\n", xml_output);
        require_action_go_through(xml_ret != NULL, continue);

        doc.Parse(xml);

        XMLElement *jpeg_capture_ability = doc.FirstChildElement("JpegCaptureAbility");
        require_action(jpeg_capture_ability != NULL, return HPR_ERROR);

        const char *text = jpeg_capture_ability->FirstChildElement("channelNO")->GetText();
        LOG_INFO("channelNO = %s\n", text);

        XMLElement *manual_capture = jpeg_capture_ability->FirstChildElement("ManualCapture");
        require_action(manual_capture != NULL, return HPR_ERROR);

        XMLElement *resolution_entry = manual_capture->FirstChildElement("ResolutionEntry");
        while (resolution_entry != NULL)
        {
            LOG_INFO("resolutionName = %s\n", resolution_entry->FirstChildElement("resolutionName")->GetText());
            LOG_INFO("index = %s\n", resolution_entry->FirstChildElement("index")->GetText());
            resolution_entry = resolution_entry->NextSiblingElement("ResolutionEntry");
        }
    }


    return HPR_OK;
}


int print_PicCaptureAbility(int user_id, NET_DVR_DEVICEINFO_V40 *device_info)
{
    require_action(user_id >= 0, return HPR_ERROR);
    require_action(device_info != NULL, return HPR_ERROR);

    int i = 0;
    int StartChan  = device_info->struDeviceV30.byStartChan;
    int EndChan    = StartChan + device_info->struDeviceV30.byChanNum;
    int StartDChan = device_info->struDeviceV30.byStartDChan;
    int EndDChan   = device_info->struDeviceV30.byIPChanNum +
                     device_info->struDeviceV30.byHighDChanNum * 256 +
                     StartDChan;

    int chan_max = EndChan > EndDChan ? EndChan : EndDChan;
    int chan_min = StartChan < StartDChan ? StartChan : StartDChan;

    BOOL ret;
    NET_DVR_COMPRESSIONCFG_ABILITY ability = {0};

    for (i = chan_min; i < chan_max; i++)
    {
        if ((i < StartChan || i >= EndChan) && (i < StartDChan || i >= EndDChan))
        {
            continue;
        }
        memset(&ability, 0, sizeof(ability));
        ret = NET_DVR_GetDeviceAbility(user_id, PIC_CAPTURE_ABILITY, (char*)&i, 4, (char*)&ability, sizeof(ability));
        require_action_go_through(ret, continue);

        int j = 0;
        LOG_VAR_INT(ability.dwSize);
        LOG_VAR_INT(ability.dwAbilityNum);
        for (j=0; j<MAX_ABILITYTYPE_NUM; j++)
        {
            LOG_INFO("ability.struAbilityNode[%d].dwAbilityType = %d\n", j, ability.struAbilityNode[j].dwAbilityType);
            if (ability.struAbilityNode[j].dwAbilityType == PIC_QUALITY_ABILITY)
            {
                NET_DVR_ABILITY_LIST pic_ability_list = ability.struAbilityNode[j];
                int z = 0;
                for (z=0; z<pic_ability_list.dwNodeNum; z++)
                {
                    LOG_INFO("struDescNode[%d].iValue = %d\n", z, pic_ability_list.struDescNode[z].iValue);
                    LOG_INFO("struDescNode[%d].byDescribe = %s\n", z, pic_ability_list.struDescNode[z].byDescribe);
                }
            }
        }
    }


    return HPR_OK;
}
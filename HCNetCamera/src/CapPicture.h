/*
* Copyright(C) 2010,Hikvision Digital Technology Co., Ltd
*
* File   name CapPicture.h
* Discription
* Version     1.0
* Author      panyd
* Create Date 2010_3_25
* Modification History
*/

typedef struct
{
    long user_id;
    const NET_DVR_DEVICEINFO_V40 *device_info;
    const char *pathname;
    unsigned int time_interval;
    int pic_size;
    int pic_quality;
}CAP_PICTURE_ARGS;

typedef struct
{
    int channel;
    int *valid_size_arr;    // 支持的图像大小
    int valid_size_len;     // 长度
    int support_capture;    // 1 支持抓图，0 不支持抓图
}JpegCaptureAbility_t;

int CapPicture(long user_id, LONG channel, const char *picName, int size, int quality);
int CapPictureStart(CAP_PICTURE_ARGS *args);
char *get_JpegCaptureAbility_xml_input(int channel, char *buf, int len);
char *get_JpegCaptureAbility_xml_output(int userid, int channel, char *xml_output, int len);
int print_JpegCaptureAbility(int user_id, NET_DVR_DEVICEINFO_V40 *device_info);
int print_PicCaptureAbility(int user_id, NET_DVR_DEVICEINFO_V40 *device_info);
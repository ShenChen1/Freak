
#ifdef __cplusplus       // 新加上的
extern "C" {
#endif


typedef struct RESULT_INFO
{
	int width;
	int heigth;
	int centerx;
	int centery;
	int ID;
}RESULT_INFO;

typedef struct RESULT_BAG
{
	unsigned int obj_num;
	RESULT_INFO  obj[24];
}RESULT_BAG;

#pragma pack (1)
typedef struct msg_pack
{
	unsigned short header;
	unsigned short lenth;
	unsigned char flow_dir;
	unsigned short start_id;
	unsigned short end_id;	
#ifdef __DARWIN_BIG_ENDIAN

	unsigned char ack_flag:1;
	unsigned char ack_need:1;
	unsigned char ack_res:6;
#else
	unsigned char ack_res:6;
	unsigned char ack_need:1;
	unsigned char ack_flag:1;
#endif
	unsigned char data[0]; //包含校验和包尾
	
}msg_pack;

typedef struct msg_report
{
	unsigned char cmd;
	unsigned char ch_type1;
	unsigned char ch_type2;
	unsigned char data[0];
}msg_report;

typedef struct fd_report
{
	unsigned short fd_id;
	unsigned short centryX;
	unsigned short centryY;
	unsigned short width;
	unsigned short height;
}fd_report;

#pragma pack()

#ifdef __cplusplus     // 新加上的
}
#endif


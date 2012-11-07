/*
 *  mtf.h
 */
#ifndef _MTF_H__
#define _MTF_H__
#include "mtftype.h"
#pragma pack(push,1)
#define MIN_TAPE_BLOCK_SIZE             512
#define MAX_TAPE_BLOCK_SIZE             65536
/* pointer to non-fixed length information */
typedef struct
{
	u_int16_t	size;	/* size of referenced field */
	u_int16_t	offset; /* offset to start of field from start of structure */
} MTF_TAPE_ADDRESS;

/* storage of date and time */
typedef u_int8_t MTF_DATE_TIME[5];	/* 14 bit year, 4 bit month, 5 bit day, */
                                /* 5 bit hour, 6 bit minute, 6 bit second */

#define MTF_DATE_TIME_LEN  5

/* common descriptor block header -- 29 */
typedef struct
{
	u_int32_t				type;		/* DBLK type */
	u_int32_t				attr;		/* block attributes */
	u_int16_t				off;		/* offset to first event */
	u_int8_t				osId;		/* OS ID */
	u_int8_t				osVer;		/* OS version */
	u_int64_t				size;		/* displayable size */
	u_int64_t				fla;		/* format logical address */
	u_int16_t 				mbc;		/* reserved for MBC */
	u_int8_t			    rsv1[6];	/* reserved for future use */
	u_int32_t				cbId;		/* control block ID */
	u_int8_t			    rsv2[4];	/* reserved for future use */
	MTF_TAPE_ADDRESS	    osData;		/* OS-specific data */
	u_int8_t				strType;	/* string type */
	u_int8_t				rsv3;		/* reserved for future use */
	u_int16_t				check;		/* header checksum */
} MTF_DB_HDR;

#define MTF_DB_HDR_LEN       52
#define MTF_DB_HDR_LEN_CSUM  25

//BLOCK ID TABLE - DBLK Name -- 30//
#define MTF_TAPE 0x45504154
#define MTF_SSET 0x54455353
#define MTF_VOLB 0x424C4F56
#define MTF_DIRB 0x42524944
#define MTF_FILE 0x454C4946
#define MTF_CFIL 0x4C494643
#define MTF_ESPB 0x42505345
#define MTF_ESET 0x54455345
#define MTF_EOTM 0x4D544F45
#define MTF_SFMB 0x424D4653

/* Span end block */
#define MTF_ENDB 0x42444E45 

/*-------------------------------------*/
#define MTF_CATALOG  0x1f2e4655
#define MTF_SPAN     0x1f2e4656

/* bit masks for MTF_DB_HDR.attr field for all values of MTF_DB_HDR.type -- 31*/
#define MTF_CONTINUATION      0x000001
#define MTF_COMPRESSION       0x000002
#define MTF_EOS_AT_EOM        0x000004

/* bit masks for MTF_DB_HDR.attr field for MTF_DB_HDR.type = MTF_TAPE */
#define MTF_SET_MAP_EXISTS    0x010000
#define MTF_FDD_ALLOWED       0x020000

/* bit masks for MTF_DB_HDR.attr field for MTF_DB_HDR.type = MTF_SSET */
#define MTF_FDD_EXISTS        0x010000
#define MTF_ENCRYPTION        0x020000

/* bit masks for MTF_DB_HDR.attr field for MTF_DB_HDR.type = MTF_ESET */
#define MTF_FDD_ABORTED       0x010000
#define MTF_END_OF_FAMILY     0x020000
#define MTF_ABORTED_SET       0x040000

/* bit masks for MTF_DB_HDR.attr field for MTF_DB_HDR.type = MTF_EOTM */
#define MTF_NO_ESET_PBA       0x010000
#define MTF_INVALID_ESET_PBA  0x020000

/* values for MTF_DB_HDR.osId field */
#define MTF_OS_NETWARE        1
#define MTF_OS_NETWARE_SMS    13
#define MTF_OS_WINDOWS_NT     14
#define MTF_OS_DOS            24
#define MTF_OS_OS2            25
#define MTF_OS_WINDOWS_95     26
#define MTF_OS_MACINTOSH      27
#define MTF_OS_UNIX           28

/* values for MTF_DB_HDR.strType field */
#define MTF_NO_STRINGS        0
#define MTF_ANSI_STR          1
#define MTF_UNICODE_STR       2
#define MTF_UTF8_STR          3

#define MTF_OS_DATA_TYPE_VOLB   10
#define MTF_OS_DATA_TYPE_DIRB   11
#define MTF_OS_DATA_TYPE_FILE   12
#define MTF_OS_DATA_VERSION_V0  0
#define MTF_OS_DATA_VERSION_V1  1
#define MTF_OS_DATA_VERSION_V2  2
#define MTF_OS_DATA_VERSION_V3  3
#define MTF_OS_DATA_VERSION_V4  4
#define MTF_OS_DATA_VERSION_V5  5
/* descriptor block for MTF_DB_HDR.type = MTF_TAPE (tape header) -- 34 */
typedef struct
{
	MTF_DB_HDR			common;		/* common block header */
	u_int32_t			famId;		/* media family ID */
	u_int32_t			attr;		/* TAPE attributes */
	u_int16_t			seq;		/* media sequence number */
	u_int16_t			encrypt;	/* password encryption */
	u_int16_t			sfmSize;	/* soft filemark block size */
	u_int16_t			catType;	/* media-based catalog type */
	MTF_TAPE_ADDRESS	name;		/* media name */
	MTF_TAPE_ADDRESS	desc;		/* media desc./label */
	MTF_TAPE_ADDRESS	passwd;		/* media password */
	MTF_TAPE_ADDRESS	software;	/* software name */
	u_int16_t			flbSize;	/* format logical block size */
	u_int16_t			vendorId;	/* software vendor ID */
	MTF_DATE_TIME		date;		/* media date */
	u_int8_t			ver;		/* MTF major version */
} MTF_TAPE_BLK;

#define MTF_TAPE_BLK_LEN 94
/* descriptor block for MTF_DB_HDR.type = MTF_SFMB (soft filemark) -- 57 */
typedef struct
{
	MTF_DB_HDR	    common;		/* common block header */
	u_int32_t		marks;		/* number of filemark entries */
	u_int32_t		used;		/* filemark entries used */
} MTF_SFMB_BLK;

#define MTF_SFMB_BLK_LEN  60

/* stream header -- */
typedef struct
{
	u_int32_t	ID;			/* stream ID */
	u_int16_t	sysAttr;	/* stream file system attributes */
	u_int16_t	mediaAttr;	/* stream media format attributes */
	u_int64_t	length;		/* stream length */
	u_int16_t	encrypt;	/* data encryption algorithm */
	u_int16_t	compress;	/* data compression algorithm */
	u_int16_t	check;		/* checksum */
} MTF_STREAM_HDR;

#define MTF_STREAM_HDR_LEN       22
#define MTF_STREAM_HDR_LEN_CSUM  10

/* bitmasks for MTF_STREAM_HDR.sysAttr */
#define MTF_STREAM_MODIFIED_FOR_READ      0x00000001
#define MTF_STREAM_CONTAINS_SECURITY      0x00000002
#define MTF_STREAM_IS_NON_PORTABLE        0x00000004
#define MTF_STREAM_IS_SPARSE              0x00000008

/* bitmasks for MTF_STREAM_HDR.mediaAttr */
#define MTF_STREAM_CONTINUE               0x00000001
#define MTF_STREAM_VARIABLE               0x00000002
#define MTF_STREAM_VAR_END                0x00000004
#define MTF_STREAM_ENCRYPTED              0x00000008
#define MTF_STREAM_COMPRESSED             0x00000010
#define MTF_STREAM_CHECKSUMED             0x00000020
#define MTF_STREAM_EMBEDDED_LENGTH        0x00000040

/* platform-independant stream data types -- 61 */
#define MTF_STAN 0x4E415453 /* standard */
#define MTF_PNAM 0x4D414E50 /* path */
#define MTF_FNAM 0x4D414E46 /* file name */
#define MTF_CSUM 0x4D555343 /* checksum */
#define MTF_CRPT 0x54505243 /* corrupt */
#define MTF_SPAD 0x44415053 /* pad */
#define MTF_SPAR 0x52415053 /* sparse */
#define MTF_TSMP 0x504D5354 /* set map, media based catalog, type 1 */
#define MTF_TFDD 0x44444654 /* fdd, media based catalog, type 1 */
#define MTF_MAP2 0x3250414D /* set map, media based catalog, type 2 */
#define MTF_FDD2 0x32444446 /* fdd, media based catalog, type 2 */



/* Customize stream data types */
#define MTF_STPB 0x42505453
#define MTF_STWX 0x58575453

/* Windows NT stream data types */
#define MTF_WINNT_ADAT 0x54414441
#define MTF_WINNT_NTEA 0x4145544E
#define MTF_WINNT_NACL 0x4C43414E
#define MTF_WINNT_NTED 0x4445544E
#define MTF_WINNT_NTQU 0x5551544E
#define MTF_WINNT_NTPR 0x5250544E
#define MTF_WINNT_NTRP 0x5052544e
#define MTF_WINNT_NTOI 0x494F544E

/* Windows 95 stream data types */
#define MTF_WIN95_GERC 0x43524547

/* Netware stream data types */
#define MTF_NETWARE_N386 0x3638334E
#define MTF_NETWARE_NBND 0x444E424E
#define MTF_NETWARE_SMSD 0x44534D53

/* OS/2 stream data types */
#define MTF_OS2_OACL 0x4C43414F
#define MTF_OS2_O2EA 0x4145324f

/* Macintosh stream data types */
#define MTF_MACINTOSH_MRSC 0x4353524D
#define MTF_MACINTOSH_MPRV 0x5652504D
#define MTF_MACINTOSH_MINF 0x464E494D
#endif //


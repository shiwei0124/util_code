/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_FileSystem.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! 
	\file HPR_FileSystem.h
	\brief HPR FileSystem Manager.

	Details.
*/

#ifndef __HPR_FS_H__
#define __HPR_FS_H__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Time.h"

#if defined(OS_WINDOWS)
#if !defined OS_WINCE
	#include <direct.h>
#endif
	typedef PSID HPR_UID_T;
	typedef PSID HPR_GID_T;
	typedef DWORD HPR_FILETYPE_T;
	typedef HPR_INT64 HPR_INODE_T;
	typedef HPR_INT64 HPR_SIZE_T;
	#define HPR_INVALID_FILE INVALID_HANDLE_VALUE
#elif defined(OS_POSIX)
	typedef uid_t HPR_UID_T;
	typedef gid_t HPR_GID_T;
	typedef mode_t HPR_FILETYPE_T;
	typedef HPR_INT64 HPR_INODE_T;
	typedef HPR_INT64 HPR_SIZE_T;
	//typedef HPR_INT64 HPR_TIME_T;
	#define HPR_INVALID_FILE (HPR_HANDLE)-1 
#else
	#error OS Not Implement Yet.
#endif

#define HPR_MAX_PATH	260			/**< windows MAX_PATH */

//(file create flag)
#define HPR_READ		0x0001		/**< (read  file.)*/
#define HPR_WRITE		0x0002		/**< (write  file.)*/
#define HPR_CREATE		0x0004		/**< (create file if not exist.)*/
#define HPR_APPEND		0x0008		/**< (append to the end of file.)*/
#define HPR_TRUNCATE	0x0010		/**< (open file and truncate the length to 0.)*/
#define HPR_BINARY		0x0020		/**< (open file in binary mode.)*/
#define HPR_EXCL		0x0040		/**< (open file will be failed if exist.)*/
#define HPR_BUFFERED	0x0080		/**< (open file of buffered io.)*/
#define HPR_SHARELOCK	0x0100		/**< (share the rwlock.)*/
#define HPR_XTHREAD		0x0200		/**< (multithread.)*/
#define HPR_LARGEFILE	0x0400		/**< (large file.)*/
#define HPR_DIRECT      0x0800      /**< (direct open file. according to the test, this flags should be used with pre-allocate)*/

//(file attribute flag)
#define HPR_ATTR_READONLY	0x1000		/**< (readonly.)*/
#define HPR_ATTR_WRITE		0x2000		/**< (write.)*/
#define HPR_ATTR_EXECUTABLE	0x4000		/**< (exeute.)*/
#define HPR_ATTR_HIDDEN		0x8000		/**< (hidden.)*/
#define HPR_ATTR_SYSTEM		0x0100		/**< (system file, windows only)*/
#define HPR_DELONCLOSE		0x0200		/**< (delete file when close.)*/

#define HPR_USETID      0x8000 /**< Set user id */
#define HPR_UREAD       0x0400 /**< Read by user */
#define HPR_UWRITE      0x0200 /**< Write by user */
#define HPR_UEXECUTE    0x0100 /**< Execute by user */

#define HPR_GSETID      0x4000 /**< Set group id */
#define HPR_GREAD       0x0040 /**< Read by group */
#define HPR_GWRITE      0x0020 /**< Write by group */
#define HPR_GEXECUTE    0x0010 /**< Execute by group */

#define HPR_WSTICKY     0x2000 /**< Sticky bit */
#define HPR_WREAD       0x0004 /**< Read by others */
#define HPR_WWRITE      0x0002 /**< Write by others */
#define HPR_WEXECUTE    0x0001 /**< Execute by others */

#define HPR_OS_DEFAULT  0x0FFF /**< use OS's default permissions */


//(file TYPE)
#define HPR_TYPE_CHAR		0x0001		/**< (char dev)*/
#define HPR_TYPE_DISK		0x0002		/**< (disk dev)*/
#define HPR_TYPE_PIPE		0x0004		/**< (pipe dev)*/
#define HPR_TYPE_UNKNOWN	0x0008		/**< (unknown dev)*/
#define HPR_TYPE_SOCK		0x0010		/**< (sock dev)*/
#define HPR_TYPE_FIFO		0x0020		/**< (fifo)*/
#define HPR_TYPE_LINK		0x0040		/**< (symbolic link)*/
#define HPR_TYPE_BLK		0x0080		/**< (block dev)*/
#define HPR_TYPE_DIR		0x0100		/**< (dir dev)*/
#define HPR_TYPE_REG		0x0200		/**< (reg dev)*/

//(file seek flag)
#define HPR_SEEK_SET	SEEK_SET	/**< (begin.)*/
#define HPR_SEEK_CUR	SEEK_CUR	/**< (current.)*/
#define HPR_SEEK_END	SEEK_END	/**< (end.)*/

/**
 * struct HPR_FINFO_T
 */
typedef struct HPR_FINFO_S
{
	HPR_FILETYPE_T nFileType;	/**< file type*/
	HPR_UID_T	vUID;			/**< user id (posix only)*/
	HPR_GID_T	vGID;			/**< group id (posix only)*/
	HPR_UINT32 nProtection;		/**< acess permission*/
	HPR_UINT32 nDeviceID;		/**< id of the device*/
	HPR_UINT32 nHardLink;		/**< number of hard link*/
	HPR_INODE_T	iINode;			/**< node of the file*/
	HPR_SIZE_T nSize;			/**< size of the file*/
	HPR_TIME_T iAccessTime;		/**< utc time. last access time.*/
	HPR_TIME_T iCreateTime;		/**< utc time. last crate time.*/
	HPR_TIME_T iWriteTime;		/**< utc time. last change time.*/
}HPR_FINFO_T;


typedef struct _FILE_FIND_INFO
{
	char sFileName[HPR_MAX_PATH];   //文件名，以null结尾
	HPR_FINFO_T FileInfo;             //文件信息结构
}HPR_FILEFIND_INFO;

//目录

/**
 * HPR_MakeDir make directory byd irectory name.
 * @param pDir (IN) directory name.
 * @reutrn 0 success, -1 fail
 * @sa  HPR_RemoveDir()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MakeDir(const char* pDir);

/**
 * HPR_RemoveDir remove directory by directory name.
 * @param pDir (IN) directory name.
 * @reutrn 0 success, -1 fail
 * @sa  HPR_MakeDir()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_RemoveDir(const char* pDir);

/**  @fn  HPR_HANDLE HPR_OpenDir(const char *pDirName);
*   @brief 打开文件夹
*   @param (IN) const char *pDirName 文件夹路径 
*   @return 失败-NULL，成功-文件夹句柄
*/
HPR_DECLARE HPR_HANDLE CALLBACK HPR_OpenDir(const char *pDirName);

/**  @fn  HPR_INT32 HPR_FindFileInDir(HPR_HANDLE hDir, HPR_FILEFIND_INFO *pFileInfo);
*   @brief 读取文件夹内的文件（循环调用，直到返回失败）
*   @param (IN) HPR_HANDLE hDir  文件夹句柄
*   @param (OUT) FILE_FIND_INFO *pFileInfo 文件信息  
*   @return 失败-HPR_ERROR，成功-HPR_OK
*/
HPR_DECLARE HPR_INT32 CALLBACK HPR_FindFileInDir(HPR_HANDLE hDir, HPR_FILEFIND_INFO *pFileInfo);

/**  @fn  HPR_INT32 HPR_CloseDir(HPR_HANDLE hDir);
*   @brief 关闭文件夹
*   @param (IN) HPR_HANDLE hDir 文件夹句柄
*   @return 失败-HPR_ERROR，成功-HPR_OK
*/
HPR_DECLARE HPR_INT32 CALLBACK HPR_CloseDir(HPR_HANDLE hDir);

//////////////////////////////////////////////////////////////////////////
//文件
/**
 * HPR_OpenFile flush the data to the disk.
 * @param hFile (IN) file fd.
 * @param nFileAttr (IN) file attr. will be ignore if the file already exist.
 * @reutrn  if fail, return HPR_INVALID_FILE otherwise return success.
 * @sa  HPR_CloseFile(), HPR_DeleteFile()
 */
HPR_DECLARE HPR_HANDLE CALLBACK HPR_OpenFile(const char* pFileName, HPR_UINT32 nFlag, HPR_UINT32 nFileAttr);

/**
 * HPR_CloseFile flush the data to the disk.
 * @param hFile (IN) file fd.
 * @reutrn 0 success, -1 fail
 * @sa  HPR_OpenFile()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CloseFile(HPR_HANDLE hFile);

/**
 * HPR_DeleteFile delete file
 * @param pFileName (IN) file name.
 * @reutrn 0 success, -1 fail
 * @sa  HPR_OpenFile()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_DeleteFile(const char* pFileName);

/**
 * HPR_ReadFile read data from disk.
 * @param hFile (IN) file fd.
 * @param pBuf (IN) buffer to store data read from disk.
 * @param nNumberOfBytesToRead (IN) number of bytes want to read.
 * @param pNumberOfBytesRead (OUT) number of bytes really read.
 * @reutrn 0 success, -1 fail
 * @sa  HPR_OpenFile(),HPR_WriteFile()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_ReadFile(HPR_HANDLE hFile, void* pBuf, HPR_UINT32 nNumberOfBytesToRead, HPR_UINT32* pNumberOfBytesRead);

/**
 * HPR_WriteFile write data to the disk.
 * @param hFile (IN) file fd.
 * @param pBuf (IN) date want to write to file.
 * @param nNumberOfBytesToWrite (IN) number of bytes want to write.
 * @param pNumberOfBytesWrite (OUT) number of bytes really write to file.
 * @reutrn 0 success, -1 fail
 * @sa  HPR_OpenFile(),HPR_ReadFile()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_WriteFile(HPR_HANDLE hFile, void* pBuf, HPR_UINT32 nNumberOfBytesToWrite, HPR_UINT32* pNumberOfBytesWrite);

/**
 * HPR_FileSeek set the file pointer.
 * @param hFile (IN) file fd.
 * @param iOffset (IN) the offset of the base, negtive data is the opposite direction.
 * @param nWhence (IN) the base position of offset.
 * @param iCurOffset (OUT) the new file pointer.
 * @reutrn 0 success, -1 fail
 * @sa  HPR_OpenFile()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_FileSeek(HPR_HANDLE hFile, HPR_INT64 iOffset, HPR_UINT32 nWhence, HPR_INT64* iCurOffset);

/**
 * HPR_FileIsEOF judge if reach the end of file.
 * @param hFile (IN) file fd.
 * @reutrn 0 success, -1 fail
 * @sa  HPR_OpenFile()
 */
HPR_DECLARE HPR_BOOL CALLBACK HPR_FileIsEOF(HPR_HANDLE hFile);

/**
 * HPR_FileStat stat the file info.
 * @param hFile (IN) file fd.
 * @param pFInfo (OUT) structure HPR_FINFO_T
 * @see HPR_FINFO_T
 * @reutrn 0 success, -1 fail
 * @sa  HPR_OpenFile()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_FileStat(HPR_HANDLE hFile, HPR_FINFO_T* pFInfo);

/**
 * HPR_FileFlush flush the data to the disk.
 * @param hFile (IN) file fd.
 * @reutrn 0 success, -1 fail
 * @sa  HPR_OpenFile()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_FileFlush(HPR_HANDLE hFile);

/**
 * HPR_GetCurExePath get the absolute path of the current execute file 
 * @param buf (OUT) execute path.
 * @param size (IN) buf size. 
 * @reutrn 0 success, -1 fail
 * @sa  HPR_GetCurExePath()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetCurExePath(char* buf, HPR_INT32 size);

#define HPR_R_OK    4       /* Test for read permission.  */
#define HPR_W_OK    2       /* Test for write permission.  */
#define HPR_X_OK    1       /* Test for execute permission.  */
#define HPR_F_OK    0       /* Test for existence.  */
HPR_DECLARE HPR_INT32 CALLBACK HPR_AccessFile(const char *path, int mode);

HPR_DECLARE HPR_INT32 CALLBACK HPR_CopyFile(const char* src, const char* dst, HPR_BOOL bFailIfExists);
HPR_DECLARE HPR_INT32 CALLBACK HPR_RenameFile(const char* oldName, const char* newName);

#endif	//#ifndef __HPR_FS_H__


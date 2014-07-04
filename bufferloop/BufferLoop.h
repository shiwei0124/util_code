/**	@file BufferLoop.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 环形存储块的类
 *
 *	@author		shiwei
 *	@date		2013/11/23
 *
 *	@note 实现一个环形内存区的类，该类线程不安全，需要外部加锁保护,但是对于分别只有一个
 *        读和写的线程是安全的
 *	@note 历史记录：
 *	@note V1.0.0  创建文件
 */
#pragma once


class CBufferLoop
{
public:
	CBufferLoop(void);
	virtual ~CBufferLoop(void);

	bool create_buffer(int nsize);
	void destroy_buffer();

	int get_buffer_size();
	int get_used_size();
	int get_rest_size();

	char get_buffer_tmp(char* pbuffer, int nbuffer_size, int* nreal_buffer_size);
	bool append_buffer(const char* pbuffer, int nbuffer_size);
	bool get_buffer(char* pbuffer, int nbuffer_size, int* nreal_buffer_size);
	
private:
	char* m_pbuffer;
	int m_nbuffer_size;

	int m_nhead_pos;			//从0开始
	int m_ntail_pos;			//从0开始
	int m_nused_size;			//使用的量
};

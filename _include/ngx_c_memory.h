
#ifndef __NGX_MEMORY_H__
#define __NGX_MEMORY_H__

#include <stddef.h>  //NULL
class CMemory                     //这是内存管理类的头文件
{
private:
	CMemory() {}  

public:
	~CMemory(){};

private:
	static CMemory *m_instance;//单例指针

public:	
	static CMemory* GetInstance() //单例
	{			
		if(m_instance == NULL)
		{
			//锁
			if(m_instance == NULL)       //双重检查锁定”（Double-Checked Locking） 模式雏形，确保线程安全的单例初始化
			{				
				m_instance = new CMemory();
				static CGarhuishou cl; 
			}
			//放锁
		}
		return m_instance;
	}	
	class CGarhuishou 
	{
	public:				
		~CGarhuishou()
		{
			if (CMemory::m_instance)
			{						
				delete CMemory::m_instance;
				CMemory::m_instance = NULL;				
			}
		}
	};
	//-------

public:
	void *AllocMemory(int memCount,bool ifmemset);
	void FreeMemory(void *point);
	
};

#endif

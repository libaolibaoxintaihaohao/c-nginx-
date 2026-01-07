
#ifndef __NGX_CONF_H__
#define __NGX_CONF_H__

#include <vector>

#include "ngx_global.h"  //一些全局/通用定义

class CConfig//配置类
{
private:
	CConfig();//构造函数私有
public:
	~CConfig();
private:
	static CConfig *m_instance;//静态私有成员指针 m_instance：用于保存唯一的 CConfig 实例地址

public:	
	static CConfig* GetInstance() //公有静态方法 GetInstance()：提供全局访问点以获取唯一实例（单例入口
	{	
		if(m_instance == NULL)//第一次检查：若实例尚未创建，则进入初始化逻辑
		{
			//锁
			if(m_instance == NULL)
			{					
				m_instance = new CConfig();//创建实例
				static CGarhuishou cl; //声明一个 静态局部对象 cl，类型为内部类 CGarhuishou
			}
			//放锁		
		}
		return m_instance;//返回实例
	}	
	class CGarhuishou 
	{
	public:				
		~CGarhuishou()
		{
			if (CConfig::m_instance)//判断时候为空，等价于if (CConfig::m_instance != nullptr)
			{						
				delete CConfig::m_instance;	//删除单例对象			
				CConfig::m_instance = NULL;	//将指针置空			
			}
		}
	};
//---------------------------------------------------
public:
    bool Load(const char *pconfName); //从指定文件名 pconfName 加载配置   总的来说就是把配置文件内容读到内存中
	const char *GetString(const char *p_itemname);//获取字符串类型配置项的值。参数为配置项名称    这个就是根据读到内存的数据，取对应的值返回
	int  GetIntDefault(const char *p_itemname,const int def);//获取整型配置项的值。参数为配置项名称和默认值   这个也是读取内存数据，取对应的值返回

public:
	std::vector<LPCConfItem> m_ConfigItemList; //存储所有配置项的容器

};

#endif

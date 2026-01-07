
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

//自定义头文件放下边,因为g++中用了-I参数，所以这里用<>也可以
#include "ngx_func.h"     //函数声明
#include "ngx_c_conf.h"   //和配置文件处理相关的类,名字带c_表示和类有关

//静态成员赋值
CConfig *CConfig::m_instance = NULL;

//构造函数
CConfig::CConfig()//构造函数依然为空
{		
}

//析构函数
CConfig::~CConfig()
{    
	std::vector<LPCConfItem>::iterator pos;
	for(pos = m_ConfigItemList.begin(); pos != m_ConfigItemList.end(); ++pos)//遍历 m_ConfigItemList 中每个 LPCConfItem（即 CConfItem*）指针	
	{		
		delete (*pos);//删除的目的是避免内存泄漏
	}//end for
	m_ConfigItemList.clear();//调用 clear() 清空 vector
    return;
}

//装载配置文件
bool CConfig::Load(const char *pconfName)// 
{   
    FILE *fp;
    fp = fopen(pconfName,"r");//只读模式
    if(fp == NULL)
        return false;

    //每一行配置文件读出来都放这里    定义读取缓冲区
    char  linebuf[501];   //每行配置都不要太长，保持<500字符内，防止出现问题
    
    //走到这里，文件打开成功 
    while(!feof(fp))  //检查文件是否结束 ，没有结束则条件成立    这个函数就是检查有没有读完，检查有没有指针到最后
    {    
        //大家要注意老师的写法，注意写法的严密性，商业代码，就是要首先确保代码的严密性
        if(fgets(linebuf,500,fp) == NULL) //从文件中读数据，每次读一行，一行最多不要超过500个字符 
            continue;

        if(linebuf[0] == 0)//如果读到的是空行（首字符就是 \0），直接跳过
            continue;

        //处理注释行
        if(*linebuf==';' || *linebuf==' ' || *linebuf=='#' || *linebuf=='\t'|| *linebuf=='\n')
			continue;
        
    lblprocstring:
        //屁股后边若有换行，回车，空格等都截取掉
		if(strlen(linebuf) > 0)   //避免对空字符串操作
		{   //检查最后一个字符是否为换行/回车/空格  10对应换行，13对应回车，32对应空格
			if(linebuf[strlen(linebuf)-1] == 10 || linebuf[strlen(linebuf)-1] == 13 || linebuf[strlen(linebuf)-1] == 32) 
			{
				linebuf[strlen(linebuf)-1] = 0;//将最后一个字符设为 \0，相当于“删除”它
				goto lblprocstring;          //跳转回去     实现 循环去除多个连续尾部空白（如 "key=value\r\n   " → "key=value"
			}		
		}
        if(linebuf[0] == 0)
            continue;//如果为空字符串，则跳过本次循环
        if(*linebuf=='[') //[开头的也不处理
			continue;

        //这种 “ListenPort = 5678”走下来；
        char *ptmp = strchr(linebuf,'=');//查找等号，判断是否为有效配置行
        if(ptmp != NULL)
        {
            LPCConfItem p_confitem = new CConfItem;                    //注意前边类型带LP，后边new这里的类型不带
            memset(p_confitem,0,sizeof(CConfItem));                    //把整个结构体清零，确保 ItemName 和 ItemContent 以 \0 开头，避免乱码
            strncpy(p_confitem->ItemName,linebuf,(int)(ptmp-linebuf)); //等号左侧的拷贝到p_confitem->ItemName
            strcpy(p_confitem->ItemContent,ptmp+1);                    //等号右侧的拷贝到p_confitem->ItemContent

            Rtrim(p_confitem->ItemName);
			Ltrim(p_confitem->ItemName);
			Rtrim(p_confitem->ItemContent);
			Ltrim(p_confitem->ItemContent);

            m_ConfigItemList.push_back(p_confitem);  //内存要释放，因为这里是new出来的 
        } //end if
    } 

    fclose(fp); //这步不可忘记
    return true;
}

//根据ItemName获取配置信息字符串，不修改不用互斥
const char *CConfig::GetString(const char *p_itemname)//根据相应的名字找到对应的名字的content
{
	std::vector<LPCConfItem>::iterator pos;	
	for(pos = m_ConfigItemList.begin(); pos != m_ConfigItemList.end(); ++pos)
	{	
		if(strcasecmp( (*pos)->ItemName,p_itemname) == 0)  //比较两个字符是否相等
			return (*pos)->ItemContent;//直接返回 ItemContent 数组的首地址
	}//end for
	return NULL;
}
//根据ItemName获取数字类型配置信息，不修改不用互斥
int CConfig::GetIntDefault(const char *p_itemname,const int def)//根据配置项名称，获取对应的整数值；如果没找到，就返回你指定的默认值
{
	std::vector<LPCConfItem>::iterator pos;	
	for(pos = m_ConfigItemList.begin(); pos !=m_ConfigItemList.end(); ++pos)
	{	
		if(strcasecmp( (*pos)->ItemName,p_itemname) == 0)
			return atoi((*pos)->ItemContent);
	}//end for
	return def;
}




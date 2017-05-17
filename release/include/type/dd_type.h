/***************************************************************************
*    File name:        dd_type.h
*    Description:
*    Create date:      2016-11-20 16:04
*    Modified date:    2016-11-21 22:21
*    Version:          V1.0
*    Author:           wangying
***************************************************************************/
#ifndef __dd_type_h__
#define __dd_type_h__

/* --------------------------------------------------------------------------*/
/**
 * @synopsis: 常用类型定义
 */
/* --------------------------------------------------------------------------*/
typedef void					ddVoid;
typedef char					ddChar;
typedef unsigned char			ddUChar;
typedef wchar_t					ddWChar;
typedef unsigned char			ddByte;
typedef unsigned char			ddBool;
typedef char					ddInt8;
typedef unsigned char			ddUInt8;
typedef short					ddInt16;
typedef unsigned short			ddUInt16;
typedef short					ddShort;
typedef unsigned short			ddUShort;
typedef int						ddInt;
typedef unsigned int			ddUInt;
typedef int						ddInt32;
typedef unsigned int			ddUInt32;
typedef long long				ddInt64;
typedef unsigned long long		ddUInt64;
typedef long					ddLong;
typedef unsigned long			ddULong;
typedef float					ddFloat;
typedef double					ddDouble;
typedef char					ddUtf8;
typedef short					ddUtf16;

typedef ddVoid *				ddPointer;
typedef const ddVoid *			ddCPointer;
typedef ddChar *				ddpChar;
typedef const ddChar *			ddpCChar;
typedef ddUChar *				ddpUChar;
typedef const ddUChar *			ddpCUChar;
typedef ddWChar *				ddpWChar;
typedef const ddWChar *			ddpCWChar;
typedef ddByte *				ddpByte;
typedef const ddByte *			ddpCByte;
typedef ddBool *				ddpBool;
typedef const ddBool *			ddpCBool;
typedef ddInt *					ddpInt;
typedef const ddInt *			ddpCInt;
typedef ddUInt *				ddpUInt;
typedef const ddUInt *			ddpCUInt;
typedef ddShort *				ddpShort;
typedef const ddShort *			ddpCShort;
typedef ddLong *				ddpLong;
typedef const ddLong *			ddpCLong;
typedef ddFloat *				ddpFloat;
typedef const ddFloat *			ddpCFloat;
typedef ddDouble *				ddpDouble;
typedef const ddDouble *		ddpCDouble;
typedef ddUtf8 *				ddpUtf8;
typedef const ddUtf8 *			ddpCUtf8;
typedef ddUtf16 *				ddpUtf16;
typedef const ddUtf16 *			ddpCUtf16;

/* --------------------------------------------------------------------------*/
/**
 * @synopsis: 常用宏类型定义
 */
/* --------------------------------------------------------------------------*/
#define dd_msg
#define nil				(0L)
#define yes				(1)
#define no				(0)
#define DD_MAXPATH		(256)
#define DD_MAXDATA		(256)

/* --------------------------------------------------------------------------*/
/**
 * @synopsis: 函数统一宏定义
 */
/* --------------------------------------------------------------------------*/
#define stricmp strcasecmp

class ddPrivateBase { public: ddPrivateBase(){} virtual ~ddPrivateBase(){} };

#define DD_PRIVATE_DECLARE(Class) \
	friend class Class##Private; \
	public: inline Class##Private *dPtr() const { return m_pPrivate; } \
	private: Class##Private *m_pPrivate;

#define DD_PUBLIC_DECLARE(Class) \
	friend class Class; \
	public: inline Class *bPtr() const { return m_pBase; } \
	private: Class *m_pBase;

#define DD_D_NEW(Class) m_pPrivate = new Class(); m_pPrivate->m_pBase = this
#define DD_D_DELETE() if ( m_pPrivate ) delete m_pPrivate

/* --------------------------------------------------------------------------*/
/**
 * @synopsis: 全局模板类
 */
/* --------------------------------------------------------------------------*/
template <class T>
class ddGlobalInstance {
public:
	ddGlobalInstance() {}
	~ddGlobalInstance() { release(); }
	static T *instance(ddBool isOnlyGet = no) {
		if ( !isOnlyGet && !s_pInstance ) {
			s_pInstance = new T();
		}
		return s_pInstance;
	}
	static ddVoid release() {
		if ( s_pInstance ) {
			delete s_pInstance;
			s_pInstance = nil;
		}
	}

private:
	static T *s_pInstance;
};
template <class T> T *ddGlobalInstance<T>::s_pInstance = nil;
#define DD_GLOBAL_INSTANCE_DO(t, f) if ( ddGlobalInstance<t>::instance() ) ddGlobalInstance<t>::instance()->f

/* --------------------------------------------------------------------------*/
/**
 * @synopsis: 实例化interface宏，接收/处理协议
 */
/* --------------------------------------------------------------------------*/
#define dd_service_instance(x) \
	class dd##x##Instance: public ddSrvManager::notifier {\
	public:\
		virtual ddBool isMyCommand(ddUInt16 command) { return command > dd##x##Device::DDENUM_IOCODE_##x##_A && command < dd##x##Device::DDENUM_IOCODE_##x##_B; }\
		virtual ddVoid onProtocol(ddCommand& cmd);\
		ddVoid add(dd##x::interface * interface) { m_listNotifier.push_back(interface); }\
		ddVoid remove(dd##x::interface * interface) {\
			std::list<dd##x::interface *>::iterator it = m_listNotifier.begin();\
			while ( it != m_listNotifier.end() ) {\
				if ( *it == interface ) {\
					m_listNotifier.erase(it);\
					break;\
				}\
				it++;\
			}\
		}\
	private: std::list<dd##x::interface *> m_listNotifier;\
	};\
	dd##x::interface::interface() { ddGlobalInstance<dd##x##Instance>::instance()->add(this); }\
	dd##x::interface::~interface() { ddGlobalInstance<dd##x##Instance>::instance()->remove(this); }
#define dd_service_callback(x, f) do {\
		std::list<dd##x::interface *>::iterator it = m_listNotifier.begin();\
		while ( it != m_listNotifier.end() ) { (*it)->f; it++; }\
	} while(0)
#define dd_switch_start(x) dd_service_instance(x) \
	ddVoid dd##x##Instance::onProtocol(ddCommand& cmd) { switch ( cmd.command() ) {
#define dd_switch_case(x, iocode, f) case dd##x##Device::iocode: dd_service_callback(x, f); break;
#define dd_switch_end()  default: break; } } 
#define dd_switch_case2(x, iocode) case dd##x##Device::iocode: 
#endif //dd_type.h

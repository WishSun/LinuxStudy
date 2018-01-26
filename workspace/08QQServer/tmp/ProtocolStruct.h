#include <sys/socket.h>
#include <netinet/in.h>

/**
  *聊天协议结构
  *
  */
enum ChatFlag
{
    msgFlag,        //消息标记
    LoginFlag,      //登录标记
    registerFlag,   //注册成功标记
    successFlag,    //登陆成功标记
    failFlag,       //登录失败标记
};

/*
 * 消息标记时，标记为msgFlag，聊天结构各个成员功能不变
 * 登录标记时，标记为LoginFlag，使用m_DestID来存储登录密码
 * 注册账号时，标记为registerFlag，使用m_DestID来存储注册密码，m_Msg来存储昵称
*/
typedef struct
{
    char m_SrcID[11];   //我方ID号
    char m_DestID[11];  //对方ID号
    char m_Msg[1024];   //消息内容
    enum ChatFlag m_Flag;//聊天标记
}ChatInfo;


/**
  *用户地址信息结构
  *
  */
typedef struct
{
    char                 m_ClientID[11];    //客户ID
    struct sockaddr_in   m_UdpAddressdd;      //客户UDP地址结构
    int                  m_TcpSocketFd;     //客户TCP套接字描述符
}UserAddressInfo;

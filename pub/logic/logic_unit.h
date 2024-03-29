//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_LOGIC_LOGIC_UNIT_H_
#define SWP_LOGIC_LOGIC_UNIT_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include "logic/base_values.h"
#include "basic/basictypes.h"
#include "basic/native_library.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_comm.h"
#include "basic/basic_util.h"
#include "share/data_share_mgr.h"

namespace logic {
/*class SendUtils {
 public:
  SendUtils();
  virtual ~SendUtils();
 private:
  static SendUtils* instance_;
 public:
  static SendUtils* GetInstance();
  static void FreeInstance();

 public:
  int32 SendFull(int socket, const char* buffer, size_t bytes);
  bool SendBytes(int socket, const void* bytes, int32 len, const char* file,
                 int32 line);
  bool SendMessage(int socket, struct PacketHead* packet, const char* file,
                   int32 line);
 private:
  struct threadrw_t* socket_lock_;
};*/

class BaseValue {
 public:
  static base_logic::Value* Deserialize(std::string* str);

  static bool GetBigInteger(base_logic::DictionaryValue* dict_value,
                            const std::wstring& path, int64* out_value);

  static bool GetReal(base_logic::DictionaryValue* dict_value,
                      const std::wstring& path, double* out_value);

  static bool GetString(base_logic::DictionaryValue* dict_value,
                        const std::wstring& path, std::string* out_value);

  static bool GetString(base_logic::DictionaryValue* dict_value,
                        const std::wstring& path, std::wstring* out_value);

  static bool GetReal(base_logic::Value* value, double* out_value);

  static bool GetBigInteger(base_logic::Value* value, int64* out_value);

  static bool GetString(base_logic::Value* value, std::string* out_value);

 private:
  static bool StringToInt64(base_logic::Value* value, int64* out);
  static bool StringToDouble(base_logic::Value* value, double* out);

};

class SomeUtils {
 public:

  static void* GetLibraryFunction(const std::string& library_name,
                                  const std::string& func_name);

  static void CreateToken(const int64 uid, const std::string& password,
                          std::string* token);

  static share::DataShareMgr* GetShareDataMgr();

  static bool VerifyToken(PacketHead* packet);

  static bool VerifyToken(const int64 uid, const std::string& token);

  static int32 SplitString(const std::string &in, const char ch,
                           std::vector<std::string> &out);

  static inline int8 StringToIntChar(const char* str) {
    int8 intvalue = 0;
    base::BasicUtil::StringUtil::StringToCharInt(std::string(str), &intvalue);
    return intvalue;
  }
  static inline int16 StringToIntShort(const char* str) {
    int16 intvalue = 0;
    base::BasicUtil::StringUtil::StringToShortInt(std::string(str), &intvalue);
    return intvalue;
  }

  static inline int Random(int min, int max) {
    return min + rand() % (max - min + 1);
  }

  static inline std::string RandomString(int length) {
    std::string ran;
    while (length > 0) {
      int n = Random(65, 122);
      if (n > 90 && n < 97)
        continue;
      char p = (char) n;
      ran.push_back(p);
      length--;
    }
    return ran;
  }

  static inline std::string StringReplace(std::string& str,
                                          const std::string& old_value,
                                          const std::string& new_value) {
    for (std::string::size_type pos(0); pos != std::string::npos; pos +=
        new_value.length()) {

      if ((pos = str.find(old_value, pos)) != std::string::npos)
        str.replace(pos, old_value.length(), new_value);
      else
        break;
    }
    return str;
  }

  static inline std::string StringReplaceUnit(std::string& str,
                                              const std::string& old_value,
                                              const std::string& new_value) {
    for (std::string::size_type pos(0); pos != std::string::npos; pos +=
        new_value.length()) {

      if ((pos = str.find(old_value, pos)) != std::string::npos) {
        str.replace(pos, old_value.length(), new_value);
        break;
      } else
        break;
    }
    return str;
  }

  static inline bool GetIPAddress(const int socket, std::string& ip,
                                  int& port) {
    struct sockaddr_in sa;
    socklen_t len;
    len = sizeof(sa);
    if (!getpeername(socket, (struct sockaddr *) &sa, &len)) {
      ip = inet_ntoa(sa.sin_addr);
      port = ntohs(sa.sin_port);
      return true;
    }
    return false;
  }
};

extern std::map<int, const char*> error_code_msgs;


class SendUtils {
 public:
   static int32 SendFull(int socket, const char* buffer, size_t bytes);
       
   static bool SendBytes(int socket, const void* bytes, int32 len,
                         const char* file, int32 line);
   
   static bool SendMessage(int socket, struct PacketHead* packet,
                           const char* file, int32 line);
       
   static bool SendHeadMessage(int socket, int32 operate_code, int32 type,
                               int32 is_zip_encrypt, int64 session,
                               int32 reserved, const char* file, int32 line);
        
   static bool SendErronMessage(int socket, int32 operate_code, int32 type,
                                int32 is_zip_encrypt, int64 session,
                                int32 reserved, int32 error, const char* file,
                                int32 line);
     
  static struct threadrw_t* socket_lock_;
};

}

#define send_message(socket, packet) \
  logic::SendUtils::SendMessage(socket, packet, __FILE__, __LINE__)

#define send_full(socket, buffer, len) \
  logic::SendUtils::SendBytes(socket, buffer, len, __FILE__, __LINE__)


#define send_error(socket, type, error_code, session) \
  do { \
    struct PacketControl packet_control; \
    MAKE_HEAD(packet_control, ERROR_TYPE, type, 0, session, 0); \
    base_logic::DictionaryValue dic; \
    dic.SetInteger(L"errorCode", error_code); \
    packet_control.body_ = &dic; \
    send_message(socket, &packet_control); \
  } while(0)

#define closelockconnect(socket) \
    shutdown(socket, SHUT_RDWR);
#endif /* LOGIC_UNIT_H_ */

//
// Created by 周飞宇 on 2018/11/20.
//

#include "JavaMsgGroup.h"
#include "JavaMsgDefine.h"
#include "Common.h"

std::shared_ptr<BaseMsgDefine> JavaMsgGroup::new_msg_define() {
  return std::make_shared<JavaMsgDefine>();
}

string JavaMsgGroup::create_code_string() {
  string rst;
  rst += "package com.rokid.caps." + ns + "\n";
  rst += "import com.rokid.utils.Caps;\n"
         "import com.rokid.utils.CapsException;\n"
         "import java.util.ArrayList;\n"
         "import java.util.List;\n";

  //pre-defined message class, and message enum
  string enum_content;
  size_t i = 0;
  for (; i < msg_define.size(); ++i) {
    enum_content += TB2"public static final int TYPE_";
    enum_content += msg_define[i]->get_msg_name_upper() + " = " + std::to_string(1111 + i) + ";\n";
  }

  enum_content += TB2"public static final int UNKNOWN = " + std::to_string(1111 + i) + ";\n";
  enum_content += TB2"public static final int getMessageType(byte[] buf) throws CapsException {\n"
                  TB3"Caps caps = Caps.parse(buf, 0, buf.length);\n"
                  TB3"int msgType = caps.readInt();\n"
                  TB3"return (msgType >= 1111 && msgType < " + std::to_string(1111 + i) + ") ?  msgType : UNKNOWN;\n"
                  TB2"}\n";
  enum_content += TB2"private MessageType(){}\n";
  rst += "\n"
         TB
         "//enum of message type\n";
  rst += TB"public final class MessageType {\n";
  rst += enum_content + TB"};\n";

  rst += TB"public final class ByteOrder {\n"
         TB2"public static final int NetworkByteOrder = 0x80;\n"
         TB2"public static final int HostByteOrder = 0x00;\n"
         TB2"private ByteOrder(){}\n"
         TB"}\n";


  for (auto &msg : msg_define)
    rst += msg->create_code_string(TB);

  return rst;
}

//
// Created by 周飞宇 on 2018/11/20.
//
#include <sys/stat.h>
#include "JavaMsgGroup.h"
#include "JavaMsgDefine.h"
#include "Common.h"

std::shared_ptr<BaseMsgDefine> JavaMsgGroup::new_msg_define() {
  return std::make_shared<JavaMsgDefine>();
}

void JavaMsgGroup::create_code_file(const string &file_path) {
  string common_class;
  string package = "package com.rokid.caps." + ns + "\n";
  string import = "import com.rokid.utils.Caps;\n"
         "import com.rokid.utils.CapsException;\n"
         "import java.util.ArrayList;\n"
         "import java.util.List;\n";
  common_class = package + import;
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
  common_class += "\n"
         TB
         "//enum of message type\n";
  common_class += TB"public final class MessageType {\n";
  common_class += enum_content + TB"};\n";

  common_class += TB"public final class ByteOrder {\n"
         TB2"public static final int NetworkByteOrder = 0x80;\n"
         TB2"public static final int HostByteOrder = 0x00;\n"
         TB2"private ByteOrder(){}\n"
         TB"}\n";
  Common::write_file(file_path + "java/MessageType.java", common_class);
  mkdir("./java/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  for (auto &msg : msg_define) {
    Common::write_file(file_path + "java/" + msg->get_msg_name() + ".java", package + import +
            msg->create_code_string(TB, CodeType::SOURCE));
  }
}

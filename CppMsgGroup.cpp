//
// Created by 周飞宇 on 2018/11/17.
//

#include "CppMsgGroup.h"
#include "Common.h"


void CppMsgGroup::create_code_file(const string &file_path) {
  static const char *const TemplateStr = "MessageType get_msg_type(const unsigned char * buff, int32_t buff_len, std::shared_ptr<Caps> &caps) {\n"
                                         TB"if (Caps::parse(buff, buff_len, caps) != CAPS_SUCCESS)\n"
                                         TB2"goto ERROR;\n"
                                         TB"int32_t msg_type;\n"
                                         TB"if (caps->read(msg_type) != CAPS_SUCCESS)\n"
                                         TB2"goto ERROR;\n"
                                         TB"return static_cast<MessageType>(msg_type);\n"
                                         "ERROR:\n"
                                         TB"return MessageType::TYPE_UNKNOWN;\n"
                                         "}\n"
                                         "MessageType get_msg_type(std::shared_ptr<Caps> &caps) {\n"
                                         TB"int32_t msg_type;\n"
                                         TB"if (caps->read(msg_type) != CAPS_SUCCESS)\n"
                                         TB2"goto ERROR;\n"
                                         TB"return static_cast<MessageType>(msg_type);\n"
                                         "ERROR:\n"
                                         TB"return MessageType::TYPE_UNKNOWN;\n"
                                         "}\n";

  static const char *const TemplateStr_H = "\n/*\n * you should call this function when you got message package,\n"
                                           " * [in] buff: the packgae buffer\n"
                                           " * [in] buffer_len: then buffer length\n"
                                           " * [out] caps: the caps object contain message, but without message type\n"
                                           " * return message type if success, else return MessageType::TYPE_UNKNOWN\n"
                                           " */\n"
                                           "static MessageType get_msg_type(const unsigned char * buff, int32_t buff_len, std::shared_ptr<Caps> &caps);\n"
                                           "static MessageType get_msg_type(std::shared_ptr<Caps> &caps);\n";
  string message_common;
  string head = "#ifndef _MESSAGECOMMON_H\n";
  head += "#define _CAPSMESSAGE_H\n";
  head += "#include \"caps.h\"\n";
  head += "#include <vector>\n";
  head += "#include <string>\n";
  string end;
  if (ns.length() > 0) {
    if (!comment.empty())
      head += "\n/*\n* " + comment + "\n*/\n";
    head += "namespace ";
    head += ns + "{\n";
  }

  if (ns.length() > 0) {

    end = "}";
  }
  end += "\n#endif //_MESSAGECOMMON_H\n";
  //pre-defined message class, and message enum
  string enum_content;
  enum_content =
                  TB
                 "\n" TB "/*\n" TB "* enum of message type\n" TB "*/\n"
                  TB"enum class MessageType: int32_t {\n";
  for (size_t i = 0; i < msg_define.size(); ++i) {
    auto msg = msg_define[i];
    if (i == 0) {
      enum_content += TB2 "TYPE_";
      enum_content += msg->get_msg_name_upper() + " = 1111";
    } else {
      enum_content += ",\n" TB2 "TYPE_";
      enum_content += msg->get_msg_name_upper();
    }
  }
  enum_content += ",\n" TB2"TYPE_UNKNOWN\n" TB "};\n";

  string pre_def_class = "\n" TB "/*\n" TB "* pre-define message class\n" TB "*/\n";
  for (auto &msg : msg_define) {
    pre_def_class += TB"class " + msg->get_msg_name() + ";\n";
  }

  string def_str =  "\n" TB "/*\n" TB "* type define for shared_ptr\n" TB "*/\n";
  for (auto &msg : msg_define) {
    def_str += TB"typedef std::shared_ptr<" + msg->get_msg_name() + "> " + msg->get_msg_name() + "Ptr;\n";
  }

  message_common = Common::add_tab(TemplateStr_H, TB);
  Common::write_file("./Cpp/MessageCommon.h", head + enum_content + pre_def_class + def_str + message_common + end);
  //cpp
  head = "#include \"MessageCommon.h\"\n\n";

  Common::write_file("./Cpp/MessageCommon.cc", head + TemplateStr);


  for (auto &msg : msg_define) {
    string h_file_content = msg->create_code_string("", CodeType::HEAD);
    Common::write_file("./Cpp/" + msg->get_msg_name() + ".h", h_file_content);
    string cc_file_content = msg->create_code_string("", CodeType::SOURCE);
    Common::write_file("./Cpp/" + msg->get_msg_name() + ".cc", cc_file_content);

  }
}

std::shared_ptr<BaseMsgDefine> CppMsgGroup::new_msg_define() {
  return std::make_shared<CppMsgDefine>();
}


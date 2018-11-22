//
// Created by 周飞宇 on 2018/11/17.
//

#include "CppMsgGroup.h"
#include "Common.h"








string CppMsgGroup::create_code_string() {
    static const char* const TemplateStr = "MessageType get_msg_type(const unsigned char * buff, int32_t buff_len, std::shared_ptr<Caps> &caps) {\n"
                                    "  if (Caps::parse(buff, buff_len, caps) != CAPS_SUCCESS)\n"
                                    "    goto ERROR;\n"
                                    "  int32_t msg_type;\n"
                                    "  if (caps->read(msg_type) != CAPS_SUCCESS)\n"
                                    "    goto ERROR;\n"
                                    "  return static_cast<MessageType>(msg_type);\n"
                                    "ERROR:\n"
                                    "  return MessageType::TYPE_UNKNOWN;\n"
                                    "}\n";
    string rst;
    rst += "#ifndef _CAPSMESSAGE_H\n";
    rst += "#define _CAPSMESSAGE_H\n";
    rst += "#include \"caps.h\"\n";
    rst += "#include <vector>\n";
    rst += "#include <string>\n";

    if (ns.length() > 0)
    {
        rst += "namespace ";
        rst += ns + "{\n";
    }
    //pre-defined message class, and message enum
    string enum_content;

    for(auto &msg : msg_define)
    {
        if (enum_content.length() == 0)
        {
            enum_content = "TYPE_";
            enum_content += msg->get_msg_name_upper() + " = 1111";
        }
        else
        {
            enum_content += ", TYPE_";
            enum_content += msg->get_msg_name_upper();
        }
    }
    enum_content += ", TYPE_UNKNOWN";
    rst += "\n"
            TB
            "//enum of message type\n";
    rst += "  enum class MessageType: int32_t {";
    rst += enum_content + "};\n";



    for(auto &msg : msg_define)
        rst += msg->create_code_string(TB);


    rst += Common::add_tab(TemplateStr, TB);
    if (ns.length() > 0)
    {

        rst += "}";
    }
    rst += "\n#endif //_CAPSMESSAGE_H\n";
    return rst;
}

std::shared_ptr<BaseMsgDefine> CppMsgGroup::new_msg_define()
{
    return std::make_shared<CppMsgDefine>();
}


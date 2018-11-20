//
// Created by 周飞宇 on 2018/11/17.
//

#include "MsgGroup.h"
#include "Common.h"


const char* const TemplateStr = "MessageType get_msg_type(const unsigned char * buff, int32_t buff_len, std::shared_ptr<Caps> &caps) {\n"
                                 "  if (Caps::parse(buff, buff_len, caps) != CAPS_SUCCESS)\n"
                                 "    goto ERROR;\n"
                                 "  int32_t msg_type;\n"
                                 "  if (caps->read(msg_type) != CAPS_SUCCESS)\n"
                                 "    goto ERROR;\n"
                                 "  return static_cast<MessageType>(msg_type);\n"
                                 "ERROR:\n"
                                 "  return MessageType::TYPE_UNKNOWN;\n"
                                 "}\n";


bool MsgGroup::Parse(cJSON *root) {
    if (root->type != cJSON_Object)
        return false;
    cJSON * ns = cJSON_GetObjectItem(root, "NameSpace");
    if (ns && ns->type == cJSON_String)
        this->ns = ns->valuestring;
    else
        this->ns = "";

    cJSON * msgs = cJSON_GetObjectItem(root, "Msg");
    if (msgs && msgs->type == cJSON_Array)
    {
        for(int i = 0; i < cJSON_GetArraySize(msgs); ++i)
        {
            cJSON *msg = cJSON_GetArrayItem(msgs, i);
            msg_define.emplace_back();
            if (!msg_define.back().Parse(msg))
                return false;
        }
    } else
        return false;
    return true;
}

string MsgGroup::CreateCpp() {
    string rst;
    rst += "#ifndef _CAPSMESSAGE_H\n";
    rst += "#define _CAPSMESSAGE_H\n";
    rst += "#include \"caps.h\"\n";
    rst += "#include <vector>\n\n";
    rst += "#include <string>\n\n";

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
            enum_content += msg.getMsg_name() + " = 1111";
        }
        else
        {
            enum_content += ", TYPE_";
            enum_content += msg.getMsg_name();
        }
    }
    enum_content += ", TYPE_UNKNOWN";
    rst += "\n"
            TB
            "//enum of message type\n";
    rst += "  enum class MessageType: int32_t {";
    rst += enum_content + "};\n";



    for(auto &msg : msg_define)
        rst += msg.CreateCpp(TB);


    rst += Common::add_tab(TemplateStr, TB);
    if (ns.length() > 0)
    {

        rst += "}";
    }
    rst += "\n#endif //_CAPSMESSAGE_H\n";
    return rst;
}

//
// Created by 周飞宇 on 2018/11/21.
//

#include "JSMsgGroup.h"
#include "JSMsgDefine.h"
#include "Common.h"

JSMsgGroup::~JSMsgGroup() {

}


string JSMsgGroup::create_code_string() {
    static const char* const Template_All = "namespace %s {\n"
                                            "%s\n"
                                            "%s\n"
                                            "}\n";
    string rst;

    if (ns.length() > 0)
    {
        rst += "namespace ";
        rst += ns + "{\n";
    }
    //pre-defined message class, and message enum
    string enum_content;

    int index_start = 1111;
    int i = 0;
    for(; i < msg_define.size(); ++i)
    {
        if (enum_content.length() == 0)
        {
            enum_content = "TYPE_";
            enum_content += msg_define[i]->get_msg_name() + " = 1111";
        }
        else
        {
            enum_content += ", TYPE_";
            enum_content += msg_define[i]->get_msg_name() + " = ";
            enum_content += std::to_string(1111 + i);
        }
    }
    enum_content += ", TYPE_UNKNOWN = ";
    enum_content += std::to_string(1111 + i);
    rst += "\n"
    TB "//enum of message type\n";
    rst += "  var MessageType {";
    rst += enum_content + "};\n";



    for(auto &msg : msg_define)
        rst += msg->create_code_string(TB);

    if (ns.length() > 0)
    {

        rst += "}";
    }
    return rst;
}

shared_ptr<BaseMsgDefine> JSMsgGroup::new_msg_define() {
    return std::make_shared<JSMsgDefine>();
}

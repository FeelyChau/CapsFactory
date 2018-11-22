//
// Created by 周飞宇 on 2018/11/21.
//

#include "JSMsgGroup.h"
#include "JSMsgDefine.h"
#include "Common.h"

JSMsgGroup::~JSMsgGroup() {

}


string JSMsgGroup::create_code_string() {
    static const char* const Template_All = "\"use strict\"\n"
                                            "%s\n"
                                            "%s\n"
                                            "\n";
    string rst;

    //pre-defined message class, and message enum
    string enum_content;

    int index_start = 1111;
    int i = 0;
    for(; i < msg_define.size(); ++i)
    {
        if (enum_content.length() == 0)
        {
            enum_content = TB "TYPE_";
            enum_content += msg_define[i]->get_msg_name_upper() + " : 1111";
        }
        else
        {
            enum_content += ",\n" TB "TYPE_";
            enum_content += msg_define[i]->get_msg_name_upper() + " : ";
            enum_content += std::to_string(1111 + i);
        }
    }
    enum_content += ",\n" TB "TYPE_UNKNOWN : ";
    enum_content += std::to_string(1111 + i);
    enum_content += "\n";
    rst += "\n//enum of message type\n";
    rst += "const MessageType = {\n";
    rst += enum_content + "};\n";



    for(auto &msg : msg_define)
        rst += msg->create_code_string("");

    return rst;
}

shared_ptr<BaseMsgDefine> JSMsgGroup::new_msg_define() {
    return std::make_shared<JSMsgDefine>();
}

//
// Created by 周飞宇 on 2018/11/21.
//

#include "JSMsgGroup.h"
#include "JSMsgDefine.h"
#include "Common.h"

JSMsgGroup::~JSMsgGroup() {

}


void JSMsgGroup::create_code_file(const string &file_path) {
    static const char* const Template_All = "\"use strict\"\n"
                                            "%s\n"
                                            "%s\n"
                                            " \n"
                                            "%s"
                                            "\n";
    string rst;

    //pre-defined message class, and message enum
    string enum_define;
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
    enum_define += "\n//enum of message type\n";
    enum_define += "const MessageType = {\n";
    enum_define += enum_content + "};\n";

    string class_define;

    for(auto &msg : msg_define)
        class_define += msg->create_code_string("", CodeType::SOURCE);

    string exports;
    for(auto &msg : msg_define)
        exports += "exports." + msg->get_msg_name() + " = " + msg->get_msg_name() + "\n";
    exports += "exports.MessageType = MessageType;\n";
    string content;
    CODEFORMAT(content, "", Template_All, enum_define.c_str(), class_define.c_str(), exports.c_str());
    Common::write_file("", content);
    if (ns == "")
      Common::write_file(file_path + "js/CapsMsg.js", content);
    else
      Common::write_file(file_path + "js/" + ns + ".js", content);
}

shared_ptr<BaseMsgDefine> JSMsgGroup::new_msg_define() {
    return std::make_shared<JSMsgDefine>();
}

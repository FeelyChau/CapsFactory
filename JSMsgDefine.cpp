//
// Created by 周飞宇 on 2018/11/21.
//

#include "JSMsgDefine.h"
#include "JSFieldDefine.h"
#include "Common.h"

JSMsgDefine::~JSMsgDefine() {

}

string JSMsgDefine::create_code_string(const string &tab, CodeType ct) {
    string class_comment = comment.length() == 0 ? "" : "// " + comment;
    static const char* const Template_Msg_Class = "\n%s\n"
                                                  "class %s {\n"
                                                  TB"constructor (){\n"
                                                  "%s\n"
                                                  TB"}\n\n"
                                                  "%s"
                                                  "}\n\n";
    //members
    string members;
    for(auto &field : fields)
        members += field->create_class_member(TB2, CodeType::SOURCE);
    string getter;
    //field getter
    for(auto &field : fields)
        getter += field->create_get_function(TB, CodeType::SOURCE);
    //field setter
    string setter;
    for(auto &field : fields)
        setter += field->create_set_function(TB, CodeType::SOURCE);
    //serialize
    string serialize = create_serialize(TB, CodeType::SOURCE);
    //deserialize
    string deserialize = create_deserialize(TB, CodeType::SOURCE);
    //serialize for caps obj
    string serialize_for_caps_obj = create_serialize_for_caps_obj(TB, CodeType::SOURCE);
    //deserialize for caps obj
    string deserialize_for_caps_obj = create_deserialize_for_caps_obj(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Msg_Class, class_comment.c_str(), msg_name.c_str(), members.c_str(), (getter + setter + serialize + deserialize + serialize_for_caps_obj + deserialize_for_caps_obj).c_str());
}

string JSMsgDefine::create_serialize(const string &tab, CodeType ct) {
    static const char* const Template_Serialize = "\n/*\n"
                                                  " * serialize this object as caps\n"
                                                  "*/\n"
                                                  "serialize() {\n"
                                                  TB"var caps = new Caps();\n"
                                                  TB"caps.writeInt32(MessageType.TYPE_%s)\n"
                                                  "%s\n"
                                                  TB"return caps;\n"
                                                  "}\n\n";
    string field_function_str;
    for(auto &field : fields)
        field_function_str += field->create_serialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Serialize, msg_name_upper.c_str(), field_function_str.c_str());
}

string JSMsgDefine::create_deserialize(const string &tab, CodeType ct) {
    static const char* const Template_Deserialize =
            "\n/*\n"
            " * deserialize this object from caps\n"
            "*/\n"
            "deserialize(caps) {\n"
            TB"var t = caps.readInt32()\n"
            TB"if (t != MessageType.TYPE_%s)\n"
            TB2"return false\n"
            "%s\n"
            TB"return true;\n"
            "}\n\n";
    string field_function_str;
    for(auto &field : fields)
        field_function_str += field->create_deserialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize, msg_name_upper.c_str(), field_function_str.c_str());
}

string JSMsgDefine::create_serialize_for_caps_obj(const string &tab, CodeType ct) {
    static const char* const Template_Serialize = "\n/*\n"
                                                  " * serialize this object as caps (without message type)\n"
                                                  "*/\n"
                                                  "serializeForCapsObj() {\n"
                                                  TB"var caps = new Caps();\n"
                                                  "%s\n"
                                                  TB"return caps;\n"
                                                  "}\n\n";
    string field_function_str;
    for(auto &field : fields)
        field_function_str += field->create_serialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Serialize, field_function_str.c_str());
}

string JSMsgDefine::create_deserialize_for_caps_obj(const string &tab, CodeType ct) {
    static const char* const Template_Deserialize =
            "\n/*\n"
            " * deserialize this object from caps (without message type)\n"
            "*/\n"
            "deserializeForCapsObj(caps) {\n"
            "%s\n"
            TB"return true;\n"
            "}\n\n";
    string field_function_str;
    for(auto &field : fields)
        field_function_str += field->create_deserialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize, field_function_str.c_str());
}

shared_ptr<BaseFieldDefine> JSMsgDefine::new_field_define() {
    return std::make_shared<JSFieldDefine>();
}

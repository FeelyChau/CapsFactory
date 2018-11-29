//
// Created by 周飞宇 on 2018/11/17.
//

#include "CppMsgDefine.h"
#include "Common.h"



string CppMsgDefine::create_code_string(const string &tab) {
    static const char* const Template_Msg_Class = "class %s {\n"
                                                  "private:\n"
                                                  "%s"
                                                  "public:\n"
                                                  TB"static std::shared_ptr<%s> create() {\n"
                                                  TB2"return std::make_shared<%s>();\n"
                                                  TB"}\n"
                                                  "%s"
                                                  "};\n\n";

    //members
    string members;
    for(auto &field : fields)
        members += field->create_class_member(TAB_VALUE);
    string getter;
    //field getter
    for(auto &field : fields)
        getter += field->create_get_function(TAB_VALUE);
    //field setter
    string setter;
    for(auto &field : fields)
        setter += field->create_set_function(TAB_VALUE);
    //serialize
    string serialize = create_serialize(TAB_VALUE);
    //deserialize
    string deserialize = create_deserialize(TAB_VALUE);
    //serialize for caps obj
    string serialize_for_caps_obj = create_serialize_for_caps_obj(TAB_VALUE);
    //deserialize for caps obj
    string deserialize_for_caps_obj = create_deserialize_for_caps_obj(TAB_VALUE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Msg_Class, msg_name.c_str(), members.c_str(), msg_name.c_str(),
            msg_name.c_str(), (getter + setter + serialize + deserialize + serialize_for_caps_obj + deserialize_for_caps_obj).c_str());
}


std::shared_ptr<BaseFieldDefine> CppMsgDefine::new_field_define()
{
    return std::make_shared<CppFieldDefine>();
}

string CppMsgDefine::create_serialize(const string &tab) {
    static const char* const Template_Serialize = "\n/*\n"
                                                  " * serialize this object as buffer\n"
                                                  "*/\n"
                                                  "int32_t serialize(void* buf, uint32_t bufsize) const {\n"
                                                  TB"std::shared_ptr<Caps> caps = Caps::new_instance();\n"
                                                  TB"caps->write(static_cast<int32_t>(MessageType::TYPE_%s));\n"
                                                  "%s"
                                                  TB"return caps->serialize(buf, bufsize);\n"
                                                  "}\n\n"
                                                  "\n/*\n * deserialize this object as caps (with message type)\n */\nint32_t serialize(std::shared_ptr<Caps> &caps) const {\n"
                                                  TB"if (!caps)\n"
                                                  TB2"caps = Caps::new_instance();\n"
                                                  TB"caps->write(static_cast<int32_t>(MessageType::TYPE_%s));\n"
                                                  "%s"
                                                  TB"return CAPS_SUCCESS;\n"
                                                  "}\n\n";
    string field_serialize;

    for(auto &field : fields)
        field_serialize += field->create_serialize_function(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_Serialize, Common::to_upper(msg_name.c_str()).c_str(), field_serialize.c_str(), Common::to_upper(msg_name.c_str()).c_str(), field_serialize.c_str());
}

string CppMsgDefine::create_deserialize(const string &tab) {
    static const char* const Template_Deserialize = "\n/*\n * deserialize this object from buffer\n */\n"
                                                    "int32_t deserialize(void* buf, uint32_t bufSize) {\n"
                                                    TB"std::shared_ptr<Caps> caps;\n"
                                                    TB"int32_t pRst = Caps::parse(buf, bufSize, caps);\n"
                                                    TB"if(pRst != CAPS_SUCCESS) return pRst;\n"
                                                    "%s"
                                                    TB"return CAPS_SUCCESS;\n"
                                                    "}\n\n"
                                                    "\n/*\n * deserialize this object from caps (with message type)\n */\n"
                                                    "int32_t deserialize(std::shared_ptr<Caps> &caps) {\n"
                                                    "%s"
                                                    TB"return CAPS_SUCCESS;\n"
                                                    "}\n\n";
    string field_deserialize;
    for(auto &field : fields)
        field_deserialize += field->create_deserialize_function(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize, field_deserialize.c_str(), field_deserialize.c_str());
}


string CppMsgDefine::create_serialize_for_caps_obj(const string &tab)
{
    static const char * const Template_SerializeForCapsObj = "\n/*\n * serialize this object as caps (without message type)\n */\n"
                                                             "int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {\n"
                                                             TB"caps = Caps::new_instance();\n"
                                                             "%s"
                                                             TB"return CAPS_SUCCESS;\n"
                                                             "}\n\n";
    string field_function_str;
    for(auto &field : fields)
        field_function_str += field->create_serialize_function("  ");
    RETURN_CODEFORMAT(tab.c_str(), Template_SerializeForCapsObj, field_function_str.c_str());
}

string CppMsgDefine::create_deserialize_for_caps_obj(const string &tab)
{
    static const char * const Template_DeserializeForCapsObj = "\n/*\n * deserialize this object from caps (without message type)\n */\n"
                                                               "int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {\n"
                                                               "%s"
                                                               TB"return CAPS_SUCCESS;\n"
                                                               "}\n\n";
    string field_deserialize;
    for(auto &field : fields)
        field_deserialize += field->create_deserialize_function(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_DeserializeForCapsObj, field_deserialize.c_str());
}

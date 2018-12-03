//
// Created by 周飞宇 on 2018/11/20.
//

#include "JavaMsgDefine.h"
#include "JavaFieldDefine.h"
#include "Common.h"

JavaMsgDefine::~JavaMsgDefine() {

}

string JavaMsgDefine::create_code_string(const string &tab, CodeType ct) {
    static const char* const Template_Msg_Class = "//%s\n"
                                                  "public class %s {\n"
                                                  "%s"
                                                  "%s"
                                                  "};\n\n";

    //members
    string members;
    for(auto &field : fields)
        members += field->create_class_member(TB, CodeType::SOURCE);
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
    RETURN_CODEFORMAT(tab.c_str(), Template_Msg_Class, comment.c_str(), msg_name.c_str(), members.c_str(), (getter + setter + serialize + deserialize + serialize_for_caps_obj + deserialize_for_caps_obj).c_str());
}

string JavaMsgDefine::create_serialize(const string &tab, CodeType ct) {
    static const char* const Template_Serialize = "\n/*\n * serialize this object as buffer\n */\n"
                                                  "public byte[] serialize(int byteOder) throws CapsException {\n"
                                                  TB"Caps caps = Caps.create();\n"
                                                  TB"caps.write(MessageType.TYPE_%s);\n"
                                                  "%s"
                                                  TB"return caps.serialize(byteOder);\n"
                                                  "}\n\n";
    string field_serialize;
    for(auto &field : fields)
        field_serialize += field->create_serialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Serialize, Common::to_upper(msg_name.c_str()).c_str(), field_serialize.c_str());
}

string JavaMsgDefine::create_deserialize(const string &tab, CodeType ct) {
    static const char* const Template_Deserialize = "\n/*\n * deserialize this object from buffer\n */\n"
                                                    "public void deserialize(byte[] buf) throws CapsException {\n"
                                                    TB"Caps caps = Caps.parse(buf, 0, buf.length);\n"
                                                    TB"int msgType = caps.readInt();\n"
                                                    TB"if (msgType != MessageType.TYPE_%s) return;\n"
                                                    "%s"
                                                    "}\n\n";
    string field_deserialize;
    for(auto &field : fields)
        field_deserialize += field->create_deserialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize, msg_name_upper.c_str(), field_deserialize.c_str());
}


string JavaMsgDefine::create_serialize_for_caps_obj(const string &tab, CodeType ct)
{
    static const char * const Template_SerializeForCapsObj = "\n/*\n * serialize this object as caps (without message type)\n */\n"
                                                             "public Caps serializeForCapsObj() throws CapsException {\n"
                                                             TB"Caps caps = Caps.create();\n"
                                                             "%s"
                                                             TB"return caps;\n"
                                                             "}\n\n";

    string field_function_str;
    for(auto &field : fields)
        field_function_str += field->create_serialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_SerializeForCapsObj, field_function_str.c_str());
}

string JavaMsgDefine::create_deserialize_for_caps_obj(const string &tab, CodeType ct)
{
    static const char * const Template_DeserializeForCapsObj = "\n/*\n * deserialize this object from caps (without message type)\n */\n"
                                                               "public void deserializeForCapsObj(Caps caps) throws CapsException {\n"
                                                               "%s"
                                                               "}\n\n";
    string field_deserialize;
    for(auto &field : fields)
        field_deserialize += field->create_deserialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_DeserializeForCapsObj, field_deserialize.c_str());
}

shared_ptr<BaseFieldDefine> JavaMsgDefine::new_field_define() {
    return make_shared<JavaFieldDefine>();
}

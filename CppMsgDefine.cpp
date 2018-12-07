//
// Created by 周飞宇 on 2018/11/17.
//

#include "CppMsgDefine.h"
#include "Common.h"


string CppMsgDefine::create_code_string(const string &tab, CodeType ct) {
  if (ct == CodeType::HEAD) {

    static const char *const Template_Msg_Class_Head = "#ifndef %s\n"
                                                       "#define %s\n"
                                                       "#include <vector>\n"
                                                       "#include <string>\n"
                                                       "#include <memory>\n"
                                                       "#include \"caps.h\"\n"
                                                       "%s";

    static const char *const Template_Msg_Class_Namespcae ="namespace %s {\n";
    static const char *const Template_Msg_Class_Body ="\n/*\n * %s\n */\n"
                                                  "\nclass %s {\n"
                                                  "private:\n"
                                                  "%s"
                                                  "public:\n"
                                                  TB"inline static std::shared_ptr<%s> create() {\n"
                                                  TB2"return std::make_shared<%s>();\n"
                                                  TB"}\n"
                                                  "%s"
                                                  "};\n\n";

    static const char *const Template_Msg_Class_Namespcae_End ="}\n";
    static const char *const Template_Msg_Class_End = "#endif // %s";
    string incs;
    //members
    string members;
    for (auto &field : fields) {
      members += field->create_class_member(TB, CodeType::HEAD);
      if (field->is_userdefine_data()) {
        incs += "#include \"" + field->get_user_define_type_name() + ".h\"\n";
      }

    }
    string getter;
    //field getter
    for (auto &field : fields)
      getter += field->create_get_function(TB, CodeType::HEAD);
    //field setter
    string setter;
    for (auto &field : fields)
      setter += field->create_set_function(TB, CodeType::HEAD);
    //serialize
    string serialize = create_serialize(TB, CodeType::HEAD);
    //deserialize
    string deserialize = create_deserialize(TB, CodeType::HEAD);
    //serialize for caps obj
    string serialize_for_caps_obj = create_serialize_for_caps_obj(TB, CodeType::HEAD);
    //deserialize for caps obj
    string deserialize_for_caps_obj = create_deserialize_for_caps_obj(TB, CodeType::HEAD);
    //to json
    string to_json = "";//create_to_json(TB, CodeType::HEAD);

    string head;
    CODEFORMAT(head, tab.c_str(), Template_Msg_Class_Head, ("_" + msg_name_upper + "_H").c_str(),
                      ("_" + msg_name_upper + "_H").c_str(), incs.c_str());
    string ns;
    CODEFORMAT(ns, tab.c_str(), Template_Msg_Class_Namespcae, msg_group->get_ns().c_str());

    string body;
    CODEFORMAT(body, tab.c_str(), Template_Msg_Class_Body, comment.c_str(),
                      msg_name.c_str(), members.c_str(), msg_name.c_str(),
                      msg_name.c_str(), (getter + setter + serialize + deserialize + serialize_for_caps_obj +
                                         deserialize_for_caps_obj + to_json).c_str());
    string end;
    CODEFORMAT(end, tab.c_str(), Template_Msg_Class_End, ("_" + msg_name_upper + "_H").c_str());
    if (msg_group->get_ns().length() == 0)
      return head + body + end;
    else {
      body = Common::add_tab(body.c_str() ,TB);
      return head + ns + body + Template_Msg_Class_Namespcae_End + end;
    }

  } else {
    static const char *const Template_Msg_Class = "#include \"%s.h\"\n"
                                                  "#include \"MessageDefine.h\"\n"
                                                  "%s"
                                                  "%s";
    string ns = msg_group->get_ns().length() == 0 ? "\n" : "using namespace " + msg_group->get_ns() + ";\n";
    string incList;
    string getter;
    //field getter
    for (auto &field : fields) {
      getter += field->create_get_function("", CodeType::SOURCE);
    }
    //field setter
    string setter;
    for (auto &field : fields)
      setter += field->create_set_function("", CodeType::SOURCE);
    //serialize
    string serialize = create_serialize("", CodeType::SOURCE);
    //deserialize
    string deserialize = create_deserialize("", CodeType::SOURCE);
    //serialize for caps obj
    string serialize_for_caps_obj = create_serialize_for_caps_obj("", CodeType::SOURCE);
    //deserialize for caps obj
    string deserialize_for_caps_obj = create_deserialize_for_caps_obj("", CodeType::SOURCE);
    //to json
    string to_json = "";//create_to_json("", CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Msg_Class, msg_name.c_str(), ns.c_str(),
                      (getter + setter + serialize + deserialize + serialize_for_caps_obj +
                       deserialize_for_caps_obj + to_json).c_str());
  }
}


std::shared_ptr<BaseFieldDefine> CppMsgDefine::new_field_define() {
  return std::make_shared<CppFieldDefine>();
}

string CppMsgDefine::create_serialize(const string &tab, CodeType ct) {
  if (ct == CodeType::SOURCE) {

    static const char *const Template_Serialize = "\n/*\n"
                                                  " * serialize this object as buffer\n"
                                                  "*/\n"
                                                  "int32_t %s::serialize(void* buf, uint32_t bufsize) const {\n"
                                                  TB"std::shared_ptr<Caps> caps = Caps::new_instance();\n"
                                                  TB"caps->write(static_cast<int32_t>(MessageType::TYPE_%s));\n"
                                                  "%s"
                                                  TB"return caps->serialize(buf, bufsize);\n"
                                                  "}\n\n"
                                                  "\n/*\n * deserialize this object as caps (with message type)\n */\n"
                                                  "int32_t %s::serialize(std::shared_ptr<Caps> &caps) const {\n"
                                                  TB"if (!caps)\n"
                                                  TB2"caps = Caps::new_instance();\n"
                                                  TB"caps->write(static_cast<int32_t>(MessageType::TYPE_%s));\n"
                                                  "%s"
                                                  TB"return CAPS_SUCCESS;\n"
                                                  "}\n\n";
    string field_serialize;

    for (auto &field : fields)
      field_serialize += field->create_serialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Serialize, msg_name.c_str(), Common::to_upper(msg_name.c_str()).c_str(),
                      field_serialize.c_str(), msg_name.c_str(), Common::to_upper(msg_name.c_str()).c_str(),
                      field_serialize.c_str());
  } else {
    static const char *const Template_Serialize = "\n/*\n"
                                                  " * serialize this object as buffer\n"
                                                  "*/\n"
                                                  "int32_t serialize(void* buf, uint32_t bufsize) const;\n"
                                                  "\n/*\n * deserialize this object as caps (with message type)\n */\n"
                                                  "int32_t serialize(std::shared_ptr<Caps> &caps) const;\n";
    return Common::add_tab(Template_Serialize, tab.c_str());
  }
}

string CppMsgDefine::create_deserialize(const string &tab, CodeType ct) {
  if (ct == CodeType::SOURCE) {
    static const char *const Template_Deserialize = "\n/*\n * deserialize this object from buffer\n */\n"
                                                    "int32_t %s::deserialize(void* buf, uint32_t bufSize) {\n"
                                                    TB"std::shared_ptr<Caps> caps;\n"
                                                    TB"int32_t pRst = Caps::parse(buf, bufSize, caps);\n"
                                                    TB"if(pRst != CAPS_SUCCESS) return pRst;\n"
                                                    "%s"
                                                    TB"return CAPS_SUCCESS;\n"
                                                    "}\n\n"
                                                    "\n/*\n * deserialize this object from caps (with message type)\n */\n"
                                                    "int32_t %s::deserialize(std::shared_ptr<Caps> &caps) {\n"
                                                    "%s"
                                                    TB"return CAPS_SUCCESS;\n"
                                                    "}\n\n";
    string field_deserialize;
    for (auto &field : fields)
      field_deserialize += field->create_deserialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize, msg_name.c_str(), field_deserialize.c_str(),
                      msg_name.c_str(), field_deserialize.c_str());
  } else {
    static const char *const Template_Deserialize = "\n/*\n * deserialize this object from buffer\n */\n"
                                                    "int32_t deserialize(void* buf, uint32_t bufSize);\n"
                                                    "\n/*\n * deserialize this object from caps (with message type)\n */\n"
                                                    "int32_t deserialize(std::shared_ptr<Caps> &caps);\n";

    return Common::add_tab(Template_Deserialize, tab.c_str());
  }
}


string CppMsgDefine::create_serialize_for_caps_obj(const string &tab, CodeType ct) {
  if (ct == CodeType::SOURCE) {
    static const char *const Template_SerializeForCapsObj = "\n/*\n * serialize this object as caps (without message type)\n */\n"
                                                            "int32_t %s::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {\n"
                                                            TB"caps = Caps::new_instance();\n"
                                                            "%s"
                                                            TB"return CAPS_SUCCESS;\n"
                                                            "}\n\n";
    string field_function_str;
    for (auto &field : fields)
      field_function_str += field->create_serialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_SerializeForCapsObj, msg_name.c_str(), field_function_str.c_str());
  } else {
    static const char *const Template_SerializeForCapsObj = "\n/*\n * serialize this object as caps (without message type)\n */\n"
                                                            "int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const;\n";
    return Common::add_tab(Template_SerializeForCapsObj, tab.c_str());
  }
}

string CppMsgDefine::create_deserialize_for_caps_obj(const string &tab, CodeType ct) {
  if (ct == CodeType::SOURCE) {
    static const char *const Template_DeserializeForCapsObj = "\n/*\n * deserialize this object from caps (without message type)\n */\n"
                                                              "int32_t %s::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {\n"
                                                              "%s"
                                                              TB"return CAPS_SUCCESS;\n"
                                                              "}\n\n";
    string field_deserialize;
    for (auto &field : fields)
      field_deserialize += field->create_deserialize_function(TB, CodeType::SOURCE);
    RETURN_CODEFORMAT(tab.c_str(), Template_DeserializeForCapsObj, msg_name.c_str(), field_deserialize.c_str());
  } else {
    static const char *const Template_DeserializeForCapsObj = "\n/*\n * deserialize this object from caps (without message type)\n */\n"
                                                              "int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps);\n";
    return Common::add_tab(Template_DeserializeForCapsObj, tab.c_str());
  }
}

string CppMsgDefine::create_to_json(const string &tab, CodeType ct) {
  if (ct == CodeType::SOURCE) {
    static const char *const Template_DeserializeForCapsObj = "\n/*\n * deserialize this object from caps (without message type)\n */\n"
                                                              "std::string %s::toJsonString() {"
                                                              TB"std::ostringstream stringStream;\n"
                                                              TB"stringStream << \"{\"<<"
                                                              "%s\"}\";"
                                                              TB"return stringStream.str();\n"
                                                              "}\n\n";
    string field_deserialize;
    for (auto &field : fields)
      field_deserialize += field->create_to_json_function(TB, CodeType::SOURCE) + "<<";
    RETURN_CODEFORMAT(tab.c_str(), Template_DeserializeForCapsObj, msg_name.c_str(), field_deserialize.c_str());
  } else {
    static const char *const Template_DeserializeForCapsObj = "\n/*\n * format object to json string\n */\n"
                                                              "std::string toJsonString();\n";
    return Common::add_tab(Template_DeserializeForCapsObj, tab.c_str());
  }
}

//
// Created by 周飞宇 on 2018/11/17.
//

#include <iostream>
#include <algorithm>
#include "CppFieldDefine.h"
#include "Common.h"


const string CppFieldDefine::create_get_function(const string &tab) {
  string getter_commont;
  getter_commont = comment.length() == 0 ? "" : "//getter " + comment;
  if (repeated) {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_TypeName_Array_User_Define = "\n%s\nconst std::shared_ptr<std::vector<%s>> get%s() const { return %s; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_User_Define, getter_commont.c_str(),
                        user_define_type_name.c_str(),
                        head_up_name.c_str(), camel_name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_TypeName_Array_String = "\n%s\nconst std::shared_ptr<std::vector<std::string>> get%s() const { return %s; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_String, getter_commont.c_str(), head_up_name.c_str(),
                        camel_name.c_str());
    } else {
      static const char *const Template_TypeName_Array_Inner = "\n%s\nconst std::shared_ptr<std::vector<%s>> get%s() const { return %s; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_Inner, getter_commont.c_str(),
                        FieldTypeStr[static_cast<int>(field_type)],
                        head_up_name.c_str(), camel_name.c_str());
    }
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_TypeName_User_Define = "\n%s\nconst std::shared_ptr<%s> & get%s() const { return %s; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_User_Define, getter_commont.c_str(),
                        user_define_type_name.c_str(), head_up_name.c_str(),
                        camel_name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_TypeName_String = "\n%s\nconst std::shared_ptr<std::string> get%s() const { return %s; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_String, getter_commont.c_str(), head_up_name.c_str(),
                        camel_name.c_str());
    } else {
      static const char *const Template_TypeName_Inner = "\n%s\n%s get%s() const { return %s; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Inner, getter_commont.c_str(),
                        FieldTypeStr[static_cast<int>(field_type)],
                        head_up_name.c_str(), camel_name.c_str());
    }
  }
}

const string CppFieldDefine::create_set_function(const string &tab) {
  string setter_commont;
  setter_commont = comment.length() == 0 ? "" : "//setter " + comment;
  if (repeated) {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_TypeName_Array_User_Define_Set = "\n%s\nvoid set%s(const std::shared_ptr<std::vector<%s>> &v) { this->%s = v; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_User_Define_Set, setter_commont.c_str(),
                        head_up_name.c_str(),
                        user_define_type_name.c_str(), camel_name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_TypeName_Array_String_Set = "\n%s\nvoid set%s(const std::shared_ptr<std::vector<std::string>> &v) { %s = v; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_String_Set, setter_commont.c_str(), head_up_name.c_str(),
                        camel_name.c_str());
    } else {
      static const char *const Template_TypeName_Array_Inner_Set = "\n%s\nvoid set%s(const std::shared_ptr<std::vector<%s>> &v) { %s = v; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_Inner_Set, setter_commont.c_str(), head_up_name.c_str(),
                        FieldTypeStr[static_cast<int>(field_type)], camel_name.c_str());
    }
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_TypeName_User_Define_Set = "\n%s\nvoid set%s(const std::shared_ptr<%s> &v) { %s = v; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_User_Define_Set, setter_commont.c_str(), head_up_name.c_str(),
                        user_define_type_name.c_str(), camel_name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_TypeName_String_Set = "\n%s\nvoid set%s(const std::shared_ptr<std::string> &v) { %s = v; }\n"
                                                              "\n%s\nvoid set%s(const char* v) { if (!%s) %s = std::make_shared<std::string>(); *%s = v; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_String_Set, setter_commont.c_str(), head_up_name.c_str(),
                        camel_name.c_str(),
                        setter_commont.c_str(), head_up_name.c_str(), camel_name.c_str(), camel_name.c_str(),
                        camel_name.c_str());
    } else {
      static const char *const Template_TypeName_Inner_Set = "\n%s\nvoid set%s(%s v) { %s = v; }\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Inner_Set, setter_commont.c_str(), head_up_name.c_str(),
                        FieldTypeStr[static_cast<int>(field_type)], camel_name.c_str());
    }
  }
}

const string CppFieldDefine::create_serialize_function(const string &tab) {
  if (repeated) {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Serialize_Array_User_Define = "if (!%s)\n"
                                                                      TB"caps->write((int32_t)0);\n"
                                                                      "else {\n"
                                                                      TB"caps->write((int32_t)%s->size());\n"
                                                                      TB"for(auto &v : *%s) {\n"
                                                                      TB2"std::shared_ptr<Caps> c;\n"
                                                                      TB2"int32_t sRst = v.serializeForCapsObj(c);\n"
                                                                      TB2"if (sRst != CAPS_SUCCESS)\n"
                                                                      TB2"return sRst;\n"
                                                                      TB2"else {\n"
                                                                      TB3"int32_t wRst = caps->write(c);\n"
                                                                      TB3"if (wRst != CAPS_SUCCESS) return wRst;\n"
                                                                      TB2"}\n"
                                                                      TB"}\n"
                                                                      "}\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_User_Define, camel_name.c_str(), camel_name.c_str(),
                        camel_name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_Serialize_Array_String = "if (!%s)\n"
                                                                 TB"caps->write((int32_t)0);\n"
                                                                 "else {\n"
                                                                 TB"caps->write((int32_t)%s->size());\n"
                                                                 TB"for(auto &v : *%s) {\n"
                                                                 TB2"int32_t wRst = caps->write(v.c_str());\n"
                                                                 TB2"if (wRst != CAPS_SUCCESS) return wRst;\n"
                                                                 TB"}\n"
                                                                 "}\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_String, camel_name.c_str(), camel_name.c_str(),
                        camel_name.c_str());
    } else {
      static const char *const Template_Serialize_Array_Inner = "if (!%s)\n"
                                                                TB"caps->write((int32_t)0);\n"
                                                                "else {\n"
                                                                TB"caps->write((int32_t)%s->size());\n"
                                                                TB"for(auto &v : *%s) {\n"
                                                                TB2"int32_t wRst = caps->write((%s)v);\n"
                                                                TB2"if (wRst != CAPS_SUCCESS) return wRst;\n"
                                                                TB"}\n"
                                                                "}\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_Inner, camel_name.c_str(), camel_name.c_str(),
                        camel_name.c_str(),
                        FieldTypeStr[static_cast<int>(field_type)]);
    }
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Serialize_User_Define = "std::shared_ptr<Caps> caps%s;\n"
                                                                "assert(!%s);\n"
                                                                "int32_t sRst%s = %s->serializeForCapsObj(caps%s);\n"
                                                                "if (sRst%s != CAPS_SUCCESS)\n"
                                                                TB"return sRst%s;\n"
                                                                "else {\n"
                                                                TB2"int32_t wRst = caps->write(caps%s);\n"
                                                                TB2"if (wRst != CAPS_SUCCESS) return wRst;\n"
                                                                "}\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_User_Define, head_up_name.c_str(), camel_name.c_str(),
                        head_up_name.c_str(), camel_name.c_str(),
                        head_up_name.c_str(), head_up_name.c_str(), head_up_name.c_str(), head_up_name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_Serialize_String = "int32_t wRst%s;\n"
                                                           "assert(%s);\n"
                                                           "wRst%s = caps->write(%s->c_str());\n"
                                                           "if (wRst%s != CAPS_SUCCESS) return wRst%s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_String, head_up_name.c_str(), camel_name.c_str(),
                        head_up_name.c_str(), camel_name.c_str(),
                        head_up_name.c_str(), head_up_name.c_str());
    } else {
      static const char *const Template_Serialize_Inner = "int32_t wRst%s = caps->write((%s)%s);\n"
                                                          "if (wRst%s != CAPS_SUCCESS) return wRst%s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner, head_up_name.c_str(),
                        FieldTypeStr[static_cast<int>(field_type)],
                        camel_name.c_str(), head_up_name.c_str(), head_up_name.c_str());
    }
  }
}

const string CppFieldDefine::create_deserialize_function(const string &tab) {
  if (repeated) {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Deserialize_Array_User_Define = "int32_t arraySize%s = 0;\n"
                                                                        "int32_t rRst%s = caps->read(arraySize%s);\n"
                                                                        "if (rRst%s != CAPS_SUCCESS) return rRst%s;\n"
                                                                        "%s->clear();\n"
                                                                        "for(int32_t i = 0; i < arraySize%s;++i) {\n"
                                                                        TB"std::shared_ptr<Caps> c;\n"
                                                                        TB"if (caps->read(c) == CAPS_SUCCESS && c) {\n"
                                                                        TB2"%s->emplace_back();\n"
                                                                        TB2"int32_t dRst = %s->back().deserializeForCapsObj(c);\n"
                                                                        TB2"if (dRst != CAPS_SUCCESS) return dRst;\n"
                                                                        TB"}\n"
                                                                        "}\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_User_Define, head_up_name.c_str(), head_up_name.c_str(),
                        head_up_name.c_str(), head_up_name.c_str(), head_up_name.c_str(),
                        camel_name.c_str(), head_up_name.c_str(), camel_name.c_str(), camel_name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_Deserialize_Array_String = "int32_t arraySize%s = 0;\n"
                                                                   "int32_t rRst%s = caps->read(arraySize%s);\n"
                                                                   "if (rRst%s != CAPS_SUCCESS) return rRst%s;\n"
                                                                   "%s->clear();\n"
                                                                   "for(int32_t i = 0; i < arraySize%s;++i) {\n"
                                                                   TB"%s->emplace_back();\n"
                                                                   TB"int32_t rRst = caps->read_string(%s->back());\n"
                                                                   TB"if (rRst != CAPS_SUCCESS) return rRst;\n"
                                                                   "}\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_String, head_up_name.c_str(), head_up_name.c_str(),
                        head_up_name.c_str(), head_up_name.c_str(), head_up_name.c_str(), camel_name.c_str(),
                        head_up_name.c_str(), camel_name.c_str(),
                        camel_name.c_str());
    } else {
      static const char *const Template_Deserialize_Array_Inner = "int32_t arraySize%s = 0;\n"
                                                                  "int32_t rRst%s = caps->read(arraySize%s);\n"
                                                                  "if (rRst%s != CAPS_SUCCESS) return rRst%s;\n"
                                                                  "%s->clear();\n"
                                                                  "for(int32_t i = 0; i < arraySize%s;++i) {\n"
                                                                  TB"%s->emplace_back();\n"
                                                                  TB"int32_t rRst = caps->read(%s->back());\n"
                                                                  TB"if (rRst != CAPS_SUCCESS) return rRst;\n"
                                                                  "}\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_Inner, head_up_name.c_str(), head_up_name.c_str(),
                        head_up_name.c_str(), head_up_name.c_str(), head_up_name.c_str(), camel_name.c_str(),
                        head_up_name.c_str(), camel_name.c_str(),
                        camel_name.c_str());
    }
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Deserialize_User_Define = "std::shared_ptr<Caps> caps%s;\n"
                                                                  "int32_t rRst%s = caps->read(caps%s);\n"
                                                                  "if (rRst%s != CAPS_SUCCESS) return rRst%s;\n"
                                                                  "if (!%s) %s = std::make_shared<%s>();\n"
                                                                  "rRst%s = %s->deserializeForCapsObj(caps%s);\n"
                                                                  "if (rRst%s != CAPS_SUCCESS) return rRst%s;\n";

      RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_User_Define, head_up_name.c_str(), head_up_name.c_str(),
                        head_up_name.c_str(), head_up_name.c_str(), head_up_name.c_str(), camel_name.c_str(),
                        camel_name.c_str(), user_define_type_name.c_str(), head_up_name.c_str(), camel_name.c_str(),
                        head_up_name.c_str(), head_up_name.c_str(), head_up_name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_Deserialize_String =
              "if (!%s) %s = std::make_shared<std::string>();\n"
              "int32_t rRst%s = caps->read_string(*%s);\n"
              "if (rRst%s != CAPS_SUCCESS) return rRst%s;\n";

      RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_String, camel_name.c_str(), camel_name.c_str(),
                        head_up_name.c_str(), camel_name.c_str(),
                        head_up_name.c_str(), head_up_name.c_str());
    } else {
      static const char *const Template_Deserialize_Inner = "int32_t rRst%s = caps->read(%s);\n"
                                                            "if (rRst%s != CAPS_SUCCESS) return rRst%s;\n";

      RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Inner, head_up_name.c_str(), camel_name.c_str(),
                        head_up_name.c_str(), head_up_name.c_str());
    }
  }
}

const string CppFieldDefine::create_class_member(const string &tab) {
  if (repeated) {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Member_Array_User_Define = "std::shared_ptr<std::vector<%s>> %s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_User_Define, user_define_type_name.c_str(), camel_name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_Member_Array_String = "std::shared_ptr<std::vector<std::string>> %s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_String, name.c_str());
    } else {
      static const char *const Template_Member_Array_String = "std::shared_ptr<std::vector<%s>> %s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_String, FieldTypeStr[static_cast<int>(field_type)],
                        camel_name.c_str());
    }
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Member_User_Define = "std::shared_ptr<%s> %s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define, user_define_type_name.c_str(), name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_Member_String = "std::shared_ptr<std::string> %s;\n";
      static const char *const Template_Member_String_Default = "std::string %s = \"%s\";\n";
      if (defualt_value != "") {
        RETURN_CODEFORMAT(tab.c_str(), Template_Member_String, camel_name.c_str());
      } else {
        RETURN_CODEFORMAT(tab.c_str(), Template_Member_String_Default, camel_name.c_str(), defualt_value.c_str());
      }
    } else {
      static const char *const Template_Member_Inner = "%s %s;\n";
      static const char *const Template_Member_Inner_Default = "%s %s = %s;\n";
      if (defualt_value != "") {
        RETURN_CODEFORMAT(tab.c_str(), Template_Member_Inner, FieldTypeStr[static_cast<int>(field_type)], camel_name.c_str());
      } else {
        RETURN_CODEFORMAT(tab.c_str(), Template_Member_Inner_Default, FieldTypeStr[static_cast<int>(field_type)],
                          camel_name.c_str(), defualt_value.c_str());
      }
    }
  }
}

const string CppFieldDefine::create_to_json_function(const string &tab) {
  if (repeated) {
    static const char *const Template_Array_Readable = "\"%s\": [\n"
                                                       TB"%s"
                                                       "]\n";
    static const char *const Template_Array_Compress = "\"%s\":[%s];";
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Member_Array_User_Define = "std::vector<%s> %s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_User_Define, user_define_type_name.c_str(), name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_Member_Array_String = "std::vector<std::string> _%s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_String, name.c_str());
    } else {
      static const char *const Template_Member_Array_String = "std::vector<%s> _%s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_String, FieldTypeStr[static_cast<int>(field_type)],
                        name.c_str());
    }
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Member_User_Define = "%s _%s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define, user_define_type_name.c_str(), name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_Member_String = "std::string _%s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_String, name.c_str());
    } else {
      static const char *const Template_Member_String = "%s _%s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_String, FieldTypeStr[static_cast<int>(field_type)], name.c_str());
    }
  }
}

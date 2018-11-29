//
// Created by 周飞宇 on 2018/11/20.
//

#include "JavaFieldDefine.h"
#include "Common.h"

JavaFieldDefine::~JavaFieldDefine() {

}

const string JavaFieldDefine::create_get_function(const string &tab) {
  string getter_comment = comment.length() == 0 ? "" : "//getter " + comment;
  string func_name = "get_";
  func_name += name;
  func_name = Common::camel_case(func_name.c_str());
  string result_type;
  if (repeated) {
    result_type = "List<";
    if (field_type == FieldType::USERDEFINE)
      result_type += user_define_type_name;
    else
      result_type += FieldJavaTypeStr[static_cast<int>(field_type)];
    result_type += ">";
  } else {
    if (field_type == FieldType::USERDEFINE)
      result_type = user_define_type_name;
    else
      result_type = FieldJavaTypeStr[static_cast<int>(field_type)];
  }
  string camel_name = Common::camel_case(name.c_str());
  static const char *const Template = "\n%s\npublic %s %s() {\n"
                                      TB"return %s;\n"
                                      "}\n";
  RETURN_CODEFORMAT(tab.c_str(), Template, getter_comment.c_str(), result_type.c_str(), func_name.c_str(),
                    camel_name.c_str());
}

const string JavaFieldDefine::create_set_function(const string &tab) {
  string setter_comment = comment.length() == 0 ? "" : "//setter " + comment;
  string func_name = "set_";
  func_name += name;
  func_name = Common::camel_case(func_name.c_str());
  string param_type;
  if (repeated) {
    param_type = "List<";
    if (field_type == FieldType::USERDEFINE)
      param_type += user_define_type_name;
    else
      param_type += FieldJavaTypeStr[static_cast<int>(field_type)];
    param_type += ">";
  } else {
    if (field_type == FieldType::USERDEFINE)
      param_type = user_define_type_name;
    else
      param_type = FieldJavaTypeStr[static_cast<int>(field_type)];
  }
  string camel_name = Common::camel_case(name.c_str());
  static const char *const Template = "\n%s\npublic void %s (%s %s) {\n"
                                      TB"this.%s = %s;\n"
                                      "}\n";
  RETURN_CODEFORMAT(tab.c_str(), Template, setter_comment.c_str(), func_name.c_str(), param_type.c_str(),
                    camel_name.c_str(), camel_name.c_str(), camel_name.c_str());
}

const string JavaFieldDefine::create_serialize_function(const string &tab) {
  string camel_name = Common::camel_case(name.c_str());
  if (repeated) {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Serialize_Array_User_Define = "caps.write(this.%s.size());\n"
                                                                      "for(%s v : this.%s)\n"
                                                                      TB"caps.write(v.serializeForCapsObj());\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_User_Define, camel_name.c_str(),
                        user_define_type_name.c_str(), camel_name.c_str());
    } else {
      static const char *const Template_Serialize_Array_Inner = "caps.write(this.%s.size());\n"
                                                                "for(%s v : this.%s)\n"
                                                                TB"caps.%s((%s)v);\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_Inner, camel_name.c_str(),
                        FieldJavaTypeStr[static_cast<int>(field_type)], camel_name.c_str(),
                        FieldJavaWriteFuncStr[static_cast<int>(field_type)], camel_name.c_str());
    }
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Serialize_User_Define = "caps.write(this.%s.serializeForCapsObj());\n";

      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_User_Define, camel_name.c_str());
    } else {
      static const char *const Template_Serialize_Inner = "caps.%s(this.%s);\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner, FieldJavaWriteFuncStr[static_cast<int>(field_type)],
                        camel_name.c_str());
    }
  }
}

const string JavaFieldDefine::create_deserialize_function(const string &tab) {
  if (repeated) {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Deserialize_Array_User_Define = "int arraySize%s = caps.readInt();\n"
                                                                        "this.%s = new ArrayList<%s>();\n"
                                                                        "for(int i = 0; i < arraySize%s; ++i) {\n"
                                                                        TB"Caps c = caps.readCaps();\n"
                                                                        TB"%s tmpObj = new %s();\n"
                                                                        TB"tmpObj.deserializeForCapsObj(c);\n"
                                                                        TB"this.%s.add(tmpObj);\n"
                                                                        "}\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_User_Define, head_up_name.c_str(), camel_name.c_str(),
                        user_define_type_name.c_str(), head_up_name.c_str(), user_define_type_name.c_str(),
                        user_define_type_name.c_str(), camel_name.c_str());
    } else {
      static const char *const Template_Deserialize_Array_Inner = "int arraySize%s = caps->readInt();\n"
                                                                  "this.%s = new ArrayList<%s>();\n"
                                                                  "for(int i = 0; i < arraySize%s; ++i)\n"
                                                                  TB"this.%s.add(caps.%s());\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_Inner, head_up_name.c_str(),
                        FieldJavaTypeStr[static_cast<int>(field_type)], camel_name.c_str(), head_up_name.c_str(),
                        camel_name.c_str(), FieldJavaReadFuncStr[static_cast<int>(field_type)]);
    }
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Deserialize_User_Define = "if (this.%s == null)\n"
                                                                  TB"this.%s = new %s();\n"
                                                                  "this.%s.deserializeForCapsObj(caps.readCaps());\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_User_Define, camel_name.c_str(), camel_name.c_str(),
                        user_define_type_name.c_str(), camel_name.c_str());
    } else {
      static const char *const Template_Deserialize_Inner = "this.%s = caps.%s();\n";

      RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Inner, camel_name.c_str(),
                        FieldJavaReadFuncStr[static_cast<int>(field_type)]);
    }
  }
}

const string JavaFieldDefine::create_class_member(const string &tab) {
  string camel_name = Common::camel_case(name.c_str());
  if (repeated) {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Member_Array_User_Define = "//%s\nprivate List<%s> %s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_User_Define, comment.c_str(), user_define_type_name.c_str(),
                        camel_name.c_str());
    } else {
      static const char *const Template_Member_Array_String = "//%s\nprivate List<%s> %s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_String, comment.c_str(),
                        FieldJavaTypeStr[static_cast<int>(field_type)], camel_name.c_str());
    }
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Member_User_Define = "//%s\nprivate %s %s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define, comment.c_str(), user_define_type_name.c_str(),
                        camel_name.c_str());
    } else {
      static const char *const Template_Member_String = "//%s\nprivate %s %s;\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_String, comment.c_str(),
                        FieldJavaTypeStr[static_cast<int>(field_type)], camel_name.c_str());
    }
  }
}

const string JavaFieldDefine::create_to_json_function(const string &tab) {
  return std::string();
}

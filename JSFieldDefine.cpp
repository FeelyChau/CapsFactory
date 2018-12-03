//
// Created by 周飞宇 on 2018/11/21.
//

#include "JSFieldDefine.h"
#include "Common.h"

JSFieldDefine::~JSFieldDefine() {

}

const string JSFieldDefine::create_get_function(const string &tab, CodeType ct) {
  string getter_comment = comment.length() == 0 ? "" : "// getter " + comment;
  static const char *const Template_Getter = "\n%s\nget %s () {\n"
                                             TB"return this.%s\n"
                                             "}\n";
  RETURN_CODEFORMAT(tab.c_str(), Template_Getter, getter_comment.c_str(), head_up_name.c_str(), camel_name.c_str());
}

const string JSFieldDefine::create_set_function(const string &tab, CodeType ct) {
  string setter_comment = comment.length() == 0 ? "" : "// setter " + comment;
  static const char *const Template_Getter = "\n%s\nset %s (v) {\n"
                                             TB"this.%s = v\n"
                                             "}\n";
  RETURN_CODEFORMAT(tab.c_str(), Template_Getter, setter_comment.c_str(), head_up_name.c_str(), camel_name.c_str());
}

const string JSFieldDefine::create_serialize_function(const string &tab, CodeType ct) {
  if (repeated) {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Serialize_Array_User_Define =
              "caps.writeInt32(this.%s.length)\n"
              "for(var i = 0; i < this.%s.length; ++i)\n"
              TB"caps.writeCaps(this.%s[i].serializeForCapsObj())\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_User_Define,
                        camel_name.c_str(), camel_name.c_str(), camel_name.c_str());
    } else if (field_type == FieldType::STRING) {
      static const char *const Template_Serialize_Array_String =
              "caps.writeInt32(this.%s.length)\n"
              "for(var i = 0; i < this.%s.length; ++i)\n"
              TB"caps.write(%s[i])\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_String,
                        camel_name.c_str(), camel_name.c_str(), camel_name.c_str());
    } else {
      static const char *const Template_Serialize_Array_Inner =
              "caps.writeInt32(this.%s.length)\n"
              "for(var i = 0; i < this.%s.length; ++i)\n"
              TB2"caps.%s(%s[i])\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_Inner,
                        camel_name.c_str(), camel_name.c_str(),
                        FieldJSWriterFunction[static_cast<int>(field_type)], camel_name.c_str());
    }
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Serialize_User_Define =
              "caps.writeCaps(this.%s.serializeForCapsObj())\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_User_Define, camel_name.c_str()
      );
    } else {
      static const char *const Template_Serialize_Inner = "caps.%s(this.%s)\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner,
                        FieldJSWriterFunction[static_cast<int>(field_type)], camel_name.c_str());
    }
  }
}

const string JSFieldDefine::create_deserialize_function(const string &tab, CodeType ct) {
  if (repeated) {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Serialize_Array_User_Define =
              "count%s = caps.readInt32()\n"
              "for(var i = 0; i < count%s; ++i) {\n"
              TB"var tmpObj = new %s()\n"
              TB"tmpObj.deserializeForCapsObj(caps.readCaps())\n"
              TB"this.%s.push(tmpObj)\n"
              "}\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_User_Define,
                        head_up_name.c_str(), head_up_name.c_str(),
                        user_define_type_name.c_str(), camel_name.c_str());
    } else {
      static const char *const Template_Serialize_Array_String =
              "count%s = caps.readInt32()\n"
              "for(var i = 0; i < count%s; ++i)\n"
              TB"this.%s.push(caps.%s())\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_String,
                        head_up_name.c_str(), head_up_name.c_str(), camel_name.c_str(),
                        FieldJSReadFunction[static_cast<int>(field_type)]);
    }
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Serialize_User_Define =
              "this.%s.deserializeForCapsObj(caps.readCaps())\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_User_Define,
                        camel_name.c_str());
    } else {
      static const char *const Template_Serialize_Inner = "this.%s = caps.%s()\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner, camel_name.c_str(),
                        FieldJSReadFunction[static_cast<int>(field_type)]);
    }
  }
}

const string JSFieldDefine::create_class_member(const string &tab, CodeType ct) {
  if (repeated) {
    static const char *const Template_Member_Array_User_Define = "this.%s = []\n";
    RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_User_Define, camel_name.c_str());
  } else {
    if (field_type == FieldType::USERDEFINE) {
      static const char *const Template_Member_User_Define = "this.%s = new %s()\n";
      RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define,
                        camel_name.c_str(), user_define_type_name.c_str());
    } else {
      static const char *const Template_Member_Inner = "this.%s = %s;\n";
      static const char *const Template_Member_Inner_Default_Int = "this.%s = %lld;\n";
      static const char *const Template_Member_Inner_Default_Double = "this.%s = %f;\n";
      if (!defualt_value.is_set) {
        RETURN_CODEFORMAT(tab.c_str(), Template_Member_Inner, camel_name.c_str(), FieldJSTypeDefault[static_cast<int>(field_type)]);
      } else {
        switch(defualt_value.ft) {
          case FieldType::INT32:
          case FieldType::INT64:
          case FieldType::UINT32:
          case FieldType::UINT64:
          RETURN_CODEFORMAT(tab.c_str(), Template_Member_Inner_Default_Int,
                            camel_name.c_str(), defualt_value.value.i);
          case FieldType::DOUBLE:
          case FieldType::FLOAT:
          RETURN_CODEFORMAT(tab.c_str(), Template_Member_Inner_Default_Double,
                            camel_name.c_str(), defualt_value.value.d);
          default:
          RETURN_CODEFORMAT(tab.c_str(), Template_Member_Inner_Default_Int,
                            camel_name.c_str(), (uint64_t)0);
        }
      }
    }
  }
}

const string JSFieldDefine::create_to_json_function(const string &tab, CodeType ct) {
  return "";
}


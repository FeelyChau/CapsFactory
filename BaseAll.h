//
// Created by 周飞宇 on 2018/11/20.
//

#ifndef CAPSFACTORY_BASEALL_H
#define CAPSFACTORY_BASEALL_H

#include "cJSON.h"
#include <string>
#include <vector>

using namespace std;

enum class FieldType { STRING = 0, INT32, UINT32, INT64, UINT64, FLOAT, DOUBLE, USERDEFINE};

class BaseFieldDefine {
public:
    BaseFieldDefine() = default;
    virtual ~BaseFieldDefine() {};

    bool parse(cJSON* root);

    void set_name(const string &name);

    void set_field_type(FieldType field_type);

    void set_optional(bool optional);

    void set_repeated(bool repeated);

    const string &get_name() const;

    FieldType get_field_type() const;

    bool is_optional() const;

    bool is_repeated() const;

    virtual const string create_get_function(const string &tab) = 0;

    virtual const string create_set_function(const string &tab) = 0;

    virtual const string create_serialize_function(const string &tab) = 0;

    virtual const string create_deserialize_function(const string &tab) = 0;

    virtual const string create_class_member(const string &tab) = 0;

protected:
    string name;
    FieldType field_type;
    string user_define_type_name;
    bool optional = false;
    bool repeated = false;
};


class BaseMsgDefine {
protected:
    string msg_name;
public:
    const string &get_msg_name() const;

    const vector<shared_ptr<BaseFieldDefine>> &get_fields() const;

protected:
    vector<std::shared_ptr<BaseFieldDefine>> fields;
protected:
    virtual string create_serialize(const string &tab) = 0;
    virtual string create_deserialize(const string &tab) = 0;
    virtual string create_serialize_for_caps_obj(const string &tab) = 0;
    virtual string create_deserialize_for_caps_obj(const string &tab) = 0;
    virtual std::shared_ptr<BaseFieldDefine> new_field_define() = 0;
public:
    BaseMsgDefine() = default;
    virtual ~BaseMsgDefine(){};
    bool parse(cJSON *root);
    virtual string create_code_string(const string &tab) = 0;
};

class BaseMsgGroup {
protected:
    string ns;
    vector<std::shared_ptr<BaseMsgDefine>> msg_define;
    virtual std::shared_ptr<BaseMsgDefine> new_msg_define() = 0;
public:
    BaseMsgGroup() = default;
    virtual ~BaseMsgGroup(){};

    bool parse(cJSON *root);
    virtual string create_code_string() = 0;
};


#endif //CAPSFACTORY_BASEALL_H

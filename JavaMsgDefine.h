//
// Created by 周飞宇 on 2018/11/20.
//

#ifndef CAPSFACTORY_JAVAMSGDEFINE_H
#define CAPSFACTORY_JAVAMSGDEFINE_H
#include "BaseAll.h"

class JavaMsgDefine: public BaseMsgDefine {
public:
    ~JavaMsgDefine() override;

    string create_code_string(const string &tab, CodeType ct) override;

protected:
    string create_serialize(const string &tab, CodeType ct) override;

    string create_deserialize(const string &tab, CodeType ct) override;

    string create_serialize_for_caps_obj(const string &tab, CodeType ct) override;

    string create_deserialize_for_caps_obj(const string &tab, CodeType ct) override;

    shared_ptr<BaseFieldDefine> new_field_define() override;

};

#endif //CAPSFACTORY_JAVAMSGDEFINE_H

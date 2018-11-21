//
// Created by 周飞宇 on 2018/11/21.
//

#ifndef CAPSFACTORY_JSMSGDEFINE_H
#define CAPSFACTORY_JSMSGDEFINE_H
#include "BaseAll.h"

class JSMsgDefine: public BaseMsgDefine {
public:
    ~JSMsgDefine() override;

    string create_code_string(const string &tab) override;

protected:
    string create_serialize(const string &tab) override;

    string create_deserialize(const string &tab) override;

    string create_serialize_for_caps_obj(const string &tab) override;

    string create_deserialize_for_caps_obj(const string &tab) override;

    shared_ptr<BaseFieldDefine> new_field_define() override;
};


#endif //CAPSFACTORY_JSMSGDEFINE_H

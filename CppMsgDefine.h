//
// Created by 周飞宇 on 2018/11/17.
//

#ifndef CAPSFACTORY_MSGDEFINE_H
#define CAPSFACTORY_MSGDEFINE_H
#include "cJSON.h"
#include "CppFieldDefine.h"
#include <string>
#include <vector>
#include "BaseAll.h"

using namespace std;


class CppMsgDefine: public BaseMsgDefine {
private:
    virtual string create_serialize(const string &tab, CodeType ct) override;
    virtual string create_deserialize(const string &tab, CodeType ct) override;
    virtual string create_serialize_for_caps_obj(const string &tab, CodeType ct) override;
    virtual string create_deserialize_for_caps_obj(const string &tab, CodeType ct) override;
    virtual std::shared_ptr<BaseFieldDefine> new_field_define() override;
public:
    CppMsgDefine() = default;
    virtual string create_code_string(const string &tab, CodeType ct) override;
};


#endif //CAPSFACTORY_MSGDEFINE_H

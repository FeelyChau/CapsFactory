//
// Created by 周飞宇 on 2018/11/17.
//

#ifndef CAPSFACTORY_MSGDEFINE_H
#define CAPSFACTORY_MSGDEFINE_H
#include "cJSON.h"
#include "FieldDefine.h"
#include <string>
#include <vector>

using namespace std;

class MsgDefine {
private:
    string msg_name;
public:
    const string &getMsg_name() const;

    const vector<FieldDefine> &getFields() const;

private:
    vector<FieldDefine> fields;
    string createSerialize(const string &tab);
    string createDeserialize(const string &tab);
    string createSerializeForCapsObj(const string &tab);
    string createDeserializeForCapsObj(const string &tab);
public:
    MsgDefine() = default;
    bool Parse(cJSON* root);
    string CreateCpp(string tab);
};


#endif //CAPSFACTORY_MSGDEFINE_H

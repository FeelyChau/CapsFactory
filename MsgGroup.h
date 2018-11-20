//
// Created by 周飞宇 on 2018/11/17.
//

#ifndef CAPSFACTORY_MSGGROUP_H
#define CAPSFACTORY_MSGGROUP_H
#include "cJSON.h"
#include "MsgDefine.h"
#include "FieldDefine.h"
#include <string>
#include <vector>

using namespace std;

class MsgGroup {
public:
    MsgGroup() = default;
    bool Parse(cJSON* root);
    string CreateCpp();
private:
    string ns;
    vector<MsgDefine> msg_define;
};


#endif //CAPSFACTORY_MSGGROUP_H

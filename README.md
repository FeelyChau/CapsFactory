基于[caps](https://github.com/Rokid/aife-mutils)的消息类生成工具，支持序列化，反序列化，访问器，支持的语言：c++， js，java;
js需要[node-caps](https://github.com/shadow-node/node-caps)支持;
java需要[caps-jni](https://github.com/Rokid/yoda-caps-java)支持;

# 编译
linux，mac测试通过
`mkdir build; cd build; cmake../; make`

# Usage
c++代码生成：
`/PATH/TO/CapsFactory -o ./message_define.json -l cpp`
将在当前目录创建cpp文件夹，`include "MessageCommon.h"`即可

js代码生成：
`/PATH/TO/CapsFactory -o ./message_define.json -l js`

java代码生成：
`/PATH/TO/CapsFactory -o ./message_define.json -l java`

# 消息定义

`{
    "NameSpace": "rokid",
    "Comment": "demo message namespace",
    "Message": [
    {
      "MsgName": "DemoMessage",
      "Comment": "demo mesage define",
      "Fields": [
        {
          "Name": "msg_id",
          "Type": "uint32",
          "Comment": "message id with default value 123",
          "Default": 123
        },
        {
          "Name": "content_list",
          "Type": "string",
          "Repeated": true,
          "Comment": "array of message content"
        },{
          "Name": "msg_time",
          "Type": "string",
          "Comment": "message datetime string"
        }
      ]
    },
    {
      "MsgName": "MessageContainAnotherMessage",
      "Comment": "a message contains other messsages",
      "Fields": [
        {
          "Name": "id",
          "Type": "uint32",
          "Comment": "id"
        },
        {
          "Name": "AnotherMessage",
          "Type": "DemoMessage",
          "Comment": "user-defined message"
        }
      ]
    }
    ]
}`

`NameSpace`
命名空间，c++对应namespace， java对应package， nodejs对应文件名

`Comment`
注释（NameSpace，Message，Field都可以添加注释）

`Message`
消息定义

`Message.MsgName`
消息名称，每个消息对应一个消息类，MsgName为类名

`Message.Fields`
消息类的成员变量

`Message.Fields.Name`
变量名称

`Message.Fields.Type`
变量类型， 目前支持的类型为`int32` `uint32` `int64` `uint64` `float` `double` `string`和自定义数据；
自定义数据就是当前定义文件内定义的其他Message

`Message.Fields.Repeated`
可选，bool，为true表示当前字段为数组

`Message.Fields.Default`
可选，当前字段默认值，int32, uint32, int64, uint64, float, double等类型字段必须为number；
string型数据输入必须为string
自定义类型暂不支持默认值设置






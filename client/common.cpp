#include "common.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QJsonParseError>
User global_user;

QString User:: ctreat_json_data(){      //注册、登录、登出
    QJsonObject json;
    json.insert("name",this->user_name);
    json.insert("passwd",this->passwd);
    json.insert("operation",this->operation);
    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    return json_str;
}
QString  User::ctreat_json_data(QString message){   //聊天
    QJsonObject json;
    json.insert("name",this->user_name);
//    json.insert("name",this->user_name);
//    json.insert("passwd",this->passwd);
    json.insert("operation","2");
    json.insert("friend_id",this->friend_id);
    json.insert("message",message);
    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    return json_str;
}

QString User::get_data_from_json(QString json ){
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(json.toStdString().data(), &jsonError);
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)){  // 解析未发生错误

            QJsonObject json = doucment.object();  // 转化为对象
            if(json.value("operation")=="friend_data"){
                this->is_update_friend_list=true;
                return json.value("friend_id").toString();
            }
            return json.value("name").toString()+":"+json.value("message").toString();
    }
    return QString();
}

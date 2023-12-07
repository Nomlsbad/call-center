#ifndef MACROS_H
#define MACROS_H

#define DECLARE_EXCEPTION(ExceptionName, ParentName) \
class ExceptionName : public ParentName \
{\
using ParentName::ParentName;\
}\

#define JSON_CONFIG(ConfigName, JsonPtr) \
class ConfigName : public JsonConfig \
{\
public:\
    ConfigName();\
};\
\
inline ConfigName::ConfigName()\
    : JsonConfig(JsonPtr##_json_pointer)\

#endif //MACROS_H

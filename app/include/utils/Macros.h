#ifndef MACROS_H
#define MACROS_H

#define GET_CONFIG(T) \
template<> \
T& Configuration::get<T>() \
{ \
static T config; \
return config; \
}

#define DECLARE_EXCEPTION(ExceptionName, ParentName) \
class ExceptionName : public ParentName \
{\
using ParentName::ParentName;\
}\

#endif //MACROS_H

#ifndef CM_ENGINE_API_H
#define CM_ENGINE_API_H

void ENGINE_API_MESSAGE_BEGIN(int, int, const float*, edict_t*);
void ENGINE_API_MESSAGE_END();
void ENGINE_API_WRITE_BYTE(int);
void ENGINE_API_WRITE_CHAR(int);
void ENGINE_API_WRITE_SHORT(int);
void ENGINE_API_WRITE_LONG(int);
void ENGINE_API_WRITE_ANGLE(float);
void ENGINE_API_WRITE_COORD(float);
void ENGINE_API_WRITE_STRING(const char*);

#endif //CM_ENGINE_API_H

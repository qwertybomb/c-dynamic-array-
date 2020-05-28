//
// Created by qwerty-bomb on 5/27/20.
//

#ifndef CLIST_H
#define CLIST_H

#include <string.h>
#include <stdlib.h>

#define UNIQUE_NAME(prefix, name) JOIN( prefix, name )
#define JOIN(symbol1, symbol2) _DO_JOIN( symbol1, symbol2)
#define _DO_JOIN(symbol1, symbol2) symbol1##symbol2


/*needed for avoiding warning*/
#define DO_PRAGMA(x) _Pragma (#x)
/*needed for resizing*/
static inline size_t upper_power_of_two(size_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    v++;
    return v;
}


#define clist(type) \
struct { \
    type *data; \
    size_t capacity;\
    size_t size; \
    void(*push_back)(void* this, type item); \
    void(*resize)(void* this, size_t size); \
    void(*reserve)(void* this, size_t size); \
    void(*insert)(void* this, size_t pos, type item); \
    void(*remove)(void* this, size_t pos); \
    void*(*copy)(void* this); \
    void(*delete)(void* this); \
}* \

/*nested function magic*/
#define create_clist(name, type) \
   DO_PRAGMA(GCC diagnostic push) \
   DO_PRAGMA(GCC diagnostic ignored "-Wincompatible-pointer-types") \
   clist(type) name; \
   name = (clist(type))malloc(sizeof(*name)); \
   { \
   name->data = (type*)malloc(sizeof(type)); \
   name->size = 0; \
   name->capacity = 0; \
   void resize(typeof(name) this, size_t size) { \
   this->size = size; \
   this->reserve(this,size); \
   } \
   void push_back(typeof(name) this, type item)  {\
   if(++this->size > this->capacity) { \
       this->reserve(this,upper_power_of_two(this->size)); \
    } \
    this->data[this->size-1] = item; \
   } \
   void reserve(typeof(name) this, size_t size) { \
   if(size > this->capacity) { \
   this->capacity = size; \
   this->data = realloc(this->data,size*sizeof(type)); \
    } \
   } \
   void insert(typeof(name) this, size_t pos, type item) { \
    if(++this->size > this->capacity) { \
       this->reserve(this,upper_power_of_two(this->size)); \
     } \
     memcpy(this->data+pos+1,this->data+pos,sizeof(type)*(this->size-pos-1)); \
     this->data[pos] = item; \
    } \
    void remove(typeof(name) this, size_t pos) { \
    memcpy(this->data+pos,this->data+pos+1,sizeof(type)*(this->size-pos-1)); \
    this->size--; \
    }\
    void delete(typeof(name) this) { \
        if(!name) return; \
        if(this->data) free(this->data); \
        free(this); \
    } \
    void* copy(typeof(name) this) { \
         typeof(name) to_return = (clist(type))malloc(sizeof(*this)); \
         to_return->resize = name->resize; \
         to_return->push_back = name->push_back; \
         to_return->reserve = name->reserve; \
         to_return->insert = name->insert; \
         to_return->remove = name->remove; \
         to_return->copy = name->copy; \
        to_return->delete = delete; \
        to_return->size = this->size; \
        to_return->capacity = this->capacity; \
        to_return->data = (type*)(malloc(sizeof(type)*this->capacity)); \
        memcpy(to_return->data,this->data,sizeof(type)*this->size); \
        return to_return; \
    } \
   name->resize = resize; \
   name->push_back = push_back; \
   name->reserve = reserve; \
   name->insert = insert; \
   name->remove = remove; \
   name->copy = copy; \
   name->delete = delete; \
} \
DO_PRAGMA(GCC diagnostic pop) \

#endif //CLIST_H

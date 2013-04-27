// Copyright (c) 2011 Robin Zhong.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <v8.h>
#include <string.h>
#include <sstream>
#include <node_buffer.h>
#include "city.h"
#ifdef __SSE4_2__
#include "citycrc.h"
#endif

#define MAX_64_HASH_LEN 21
#define MAX_128_HASH_LEN MAX_64_HASH_LEN*2

using namespace v8;
using node::Buffer;

inline uint32 Uint64High32(const uint64 &x) {
    return (uint32)((x >> 32) & 0xFFFFFFFF);
}

inline uint32 Uint64Low32(const uint64 &x) {
    return (uint32)x;
}

inline uint64 HighLow32ToUint64(const uint32 &low, const uint32 &high) {
    return ((uint64_t)low) + ((((uint64_t)high) << 32) & 0xFFFFFFFF00000000ll);
}


uint64
to_uint64(const char* data, size_t len) {
    std::stringstream str;
    str.write(data, len);

    uint64 v;
    str >> v;
    return v;
}

inline uint64
to_uint64(const Local<Object> &obj) {
    Local<Uint32> low = obj->Get(String::New("low"))->ToUint32();
    Local<Uint32> high = obj->Get(String::New("high"))->ToUint32();
    return HighLow32ToUint64(low->Value(), high->Value());
}

inline uint64
to_uint64(const Local<Value> &obj) {
    if(obj->IsObject()) {
        return to_uint64(obj->ToObject());
    } else {
        String::AsciiValue strObj(obj->ToString());
        return to_uint64(*strObj, strObj.length());
    }
}

void
to_uint128(uint128* v, const char* data, size_t len) {
    std::stringstream str;
    uint64 i;

    char* sep = strchr((char*)data, (int)',');
    if(sep == NULL) {
        str.write(data, len);
        str >> i;
        v->first = i;
        v->second = 0;

    } else {
        int pos = (int)(sep - data);
        str.write(data, pos);
        str >> i;
        v->first = i;

        str.clear();
        str.write(sep+1, len - pos - 1);
        str >> i;
        v->second = i;
    }
}

inline void
to_uint128(uint128 *v, const Local<Object> &obj) {
    v->first = to_uint64(obj->Get(String::New("low"))->ToObject());
    v->second = to_uint64(obj->Get(String::New("high"))->ToObject());
}

inline void
to_uint128(uint128 *v, const Local<Value> &obj) {
    if(obj->IsObject()) {
        to_uint128(v, obj->ToObject());
    } else {
        String::AsciiValue strObj(obj->ToString());
        to_uint128(v, *strObj, strObj.length());
    }
}

Local<String>
stringify_hash(const uint64 &hash) {
    // hash to char
    char buf[MAX_64_HASH_LEN];
    memset(buf, 0, MAX_64_HASH_LEN);
    snprintf(buf, MAX_64_HASH_LEN, "%llu", hash);

    return String::New(buf);
}

Local<String>
stringify_hash(const uint128 &hash) {
    // hash to char
    char buf[MAX_128_HASH_LEN];
    memset(buf, 0, MAX_128_HASH_LEN);
    snprintf(buf, MAX_128_HASH_LEN, "%llu,%llu", Uint128Low64(hash), Uint128High64(hash));

    return String::New(buf);
}

Local<String>
stringify_hash(Local<Object> obj) {
    Local<Value> lowObject = obj->Get(String::New("low"));
    if(lowObject->IsObject()) {
        uint128 hash128;
        to_uint128(&hash128, obj);
        return stringify_hash(hash128);
    } else {
        return stringify_hash(to_uint64(obj));
    }
}

Local<Object>
objectify_hash(const uint64 &hash) {
    uint32 low = Uint64Low32(hash);
    uint32 high = Uint64High32(hash);

    Local<Object> ret = Object::New();
    ret->Set(String::New("low"), Integer::NewFromUnsigned(low));
    ret->Set(String::New("high"), Integer::NewFromUnsigned(high));
    ret->Set(String::New("value"), stringify_hash(hash));
    ret->Set(String::New("uint64"), Boolean::New(true));

    return ret;
}

Local<Object>
objectify_hash(const uint128 &hash) {
    Local<Object> ret = Object::New();
    ret->Set(String::New("low"), objectify_hash(Uint128Low64(hash)));
    ret->Set(String::New("high"), objectify_hash(Uint128High64(hash)));
    ret->Set(String::New("value"), stringify_hash(hash));
    ret->Set(String::New("uint128"), Boolean::New(true));

    return ret;
}

Local<Object>
objectify_hash(const String::AsciiValue &hash) {
    const char *data = *hash;
    size_t len = hash.length();

    char *ch = strchr((char*)data, ',');
    if(ch == NULL) {
        return objectify_hash(to_uint64(data, len));
    } else {
        uint128 hash128;
        to_uint128(&hash128, data, len);
        return objectify_hash(hash128);
    }
}

Local<Object>
objectify_hashs(const uint64 hashs[], size_t size) {
    Local<Object> ret = Array::New(size);

    for(size_t i=0; i<size; i++) {
        uint64 hash = hashs[i];
        ret->Set(i, objectify_hash(hash));
    }

    return ret;
}

Handle<Value>
node_Stringify(const Arguments& args) {
    HandleScope scope;

    int args_len = args.Length();
    if(args_len != 1 || !args[0]->IsObject()) {
        return ThrowException(String::New("Invalid arguments."));
    }

    Local<Object> obj = args[0]->ToObject();
    return scope.Close(stringify_hash(obj));
}

Handle<Value>
node_Objectify(const Arguments& args) {
    HandleScope scope;

    int args_len = args.Length();
    if(args_len != 1) {
        return ThrowException(String::New("Invalid arguments."));
    }

    String::AsciiValue obj(args[0]->ToString());
    return scope.Close(objectify_hash(obj));
}

Handle<Value>
node_CityHash64(const Arguments& args) {
    HandleScope scope;

    int args_len = args.Length();
    if(args_len == 0 || args_len > 3) {
        return ThrowException(String::New("Invalid arguments."));
    }

    String::Utf8Value data(args[0]->ToString());
    const char* str = *data;
    size_t len = data.length();

    if (Buffer::HasInstance(args[0])) {
      Local<Object> obj = args[0]->ToObject();
      str = Buffer::Data(obj);
      len = Buffer::Length(obj);
    }

    uint64 hash;

    if(args_len == 1) {
        hash = CityHash64(str, len);
    } else if(args_len == 2) {
        uint64 seed = to_uint64(args[1]);

        hash = CityHash64WithSeed(str, len, seed);
    } else if(args_len == 3) {
        uint64 seed0 = to_uint64(args[1]);
        uint64 seed1 = to_uint64(args[2]);
        hash = CityHash64WithSeeds(str, len, seed0, seed1);
    }

    return scope.Close(objectify_hash(hash));
}

Handle<Value>
node_CityHash128(const Arguments& args) {
    HandleScope scope;

    int args_len = args.Length();
    if(args_len == 0 || args_len > 2) {
        return ThrowException(String::New("Invalid arguments."));
    }

    String::Utf8Value data(args[0]->ToString());
    const char* str = *data;
    size_t len = data.length();

    if (Buffer::HasInstance(args[0])) {
      Local<Object> obj = args[0]->ToObject();
      str = Buffer::Data(obj);
      len = Buffer::Length(obj);
    }

    uint128 hash;

    if(args.Length() == 2) {
        uint128 seed;
        to_uint128(&seed, args[1]);

        hash = CityHash128WithSeed(str, len, seed);
    } else {
        hash = CityHash128(str, len);
    }

    return scope.Close(objectify_hash(hash));
}

Handle<Value>
node_CityHashCrc128(const Arguments& args) {
    HandleScope scope;
#ifdef __SSE4_2__

    int args_len = args.Length();
    if(args_len == 0 || args_len > 2) {
        return ThrowException(String::New("Invalid arguments."));
    }

    String::Utf8Value data(args[0]->ToString());
    const char* str = *data;
    size_t len = data.length();

    if (Buffer::HasInstance(args[0])) {
      Local<Object> obj = args[0]->ToObject();
      str = Buffer::Data(obj);
      len = Buffer::Length(obj);
    }

    uint128 hash;

    if(args.Length() == 2) {
        if(CityHashCrc128WithSeed == NULL) {
            return ThrowException(String::New("CityHashCrc128WithSeed function does not found."));
        }

        uint128 seed;
        to_uint128(&seed, args[1]);
        hash = CityHashCrc128WithSeed(str, len, seed);
    } else {
        if(CityHashCrc128 == NULL) {
            return ThrowException(String::New("CityHashCrc128 function does not found."));
        }
        hash = CityHashCrc128(str, len);
    }

    return scope.Close(objectify_hash(hash));
#else
    return ThrowException(String::New("SSE4.2 is not supported for your platform"));
#endif
}

Handle<Value>
node_CityHashCrc256(const Arguments& args) {
    HandleScope scope;

#ifdef __SSE4_2__
    int args_len = args.Length();
    if(args_len != 1) {
        return ThrowException(String::New("Invalid arguments."));
    }

    String::Utf8Value data(args[0]->ToString());
    const char* str = *data;
    size_t len = data.length();

    if (Buffer::HasInstance(args[0])) {
      Local<Object> obj = args[0]->ToObject();
      str = Buffer::Data(obj);
      len = Buffer::Length(obj);
    }

    if(CityHashCrc256 == NULL) {
        return ThrowException(String::New("CityHashCrc256 function does not found."));
    }

    uint64 hashs[4];
    CityHashCrc256(str, len, hashs);

    return scope.Close(objectify_hashs(hashs, 4));
#else
    return ThrowException(String::New("SSE4.2 is not supported for your platform"));
#endif
}

Handle<Value>
node_isCrcSupported(const Arguments& args) {
    HandleScope scope;

#ifdef __SSE4_2__
    return scope.Close(Boolean::New(true));
#else
    return scope.Close(Boolean::New(false));
#endif
}


extern "C" void
init (Handle<Object> target) {
    HandleScope scope;
    target->Set(String::New("stringify"), FunctionTemplate::New(node_Stringify)->GetFunction());
    target->Set(String::New("objectify"), FunctionTemplate::New(node_Objectify)->GetFunction());
    target->Set(String::New("hash64"), FunctionTemplate::New(node_CityHash64)->GetFunction());
    target->Set(String::New("hash128"), FunctionTemplate::New(node_CityHash128)->GetFunction());
    target->Set(String::New("crc128"), FunctionTemplate::New(node_CityHashCrc128)->GetFunction());
    target->Set(String::New("crc256"), FunctionTemplate::New(node_CityHashCrc256)->GetFunction());
    target->Set(String::New("isCrcSupported"), FunctionTemplate::New(node_isCrcSupported)->GetFunction());
}

NODE_MODULE(cityhash, init)

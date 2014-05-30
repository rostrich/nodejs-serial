#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <node.h>
#include "serial.h"
#include "util.h"

#define BUFLEN 2048

using namespace v8;

typedef struct Buffer {
  int len;
  char s[BUFLEN];
} Buffer;

typedef struct AsyncData {
  Buffer *buf;
  Persistent<Function> callback;
} AsyncData;

Buffer *blockingRead();
void Work(uv_work_t *req);
void End(uv_work_t *req);
Handle<Value> Read(const Arguments& args);

// -------------------------------------------------------------------
void XLog(const char *fmt, ...) {
  va_list args;
  char *s;

  JSObject glob = Context::GetCurrent()->Global();
  JSObject console = JSVAL_TO_OBJECT(glob->Get(String::New("console")));
  Local<Function> log =
    Local<Function>::Cast(JSVAL_TO_OBJECT(console->Get(String::New("log"))));
  va_start(args, fmt);
  vasprintf(&s, fmt, args);
  va_end(args);
  const unsigned argc = 1;
  Local<Value> argv[argc] = { Local<Value>::New(String::New(s)) };
  log->Call(Context::GetCurrent()->Global(), argc, argv);
}                                                            
                                                             
// -------------------------------------------------------------------
const char* ToCString(const String::Utf8Value &value) {
  return *value ? *value : "<string conversion failed>";
}                                                            
                                                             
// -------------------------------------------------------------------     
char *JSVAL_TO_CSTRING(Local<Value> value) {                 
  if (value->IsString()) {                                   
    String::Utf8Value string(value);                         
    char *str = (char *)malloc(string.length() + 1);         
    strcpy(str, *string);                                    
    return str;                                              
  }                                                          
  return NULL;                                               
}                                                            
                                                             
// -----------------------------------------------------------------
Buffer *blockingRead() {
  Buffer *buf = (Buffer *)malloc(sizeof(Buffer));
  buf->len = 0;
  
  serialWaitForData();
  while (serialAvailable())
    buf->s[buf->len++] = serialRead();
  return buf;
}

// -----------------------------------------------------------------
void Work(uv_work_t *req) {
  AsyncData *data = (AsyncData *)req->data;
  data->buf = blockingRead();
}

// -----------------------------------------------------------------
void End(uv_work_t *req) {
  HandleScope scope;
  AsyncData *data = (AsyncData *)req->data;

  int i, l = data->buf->len;
  Handle<Array> array = Array::New(l);
  for (i=0; i<l; i++)
    array->Set(i, Integer::New((int)data->buf->s[i]));
  Handle<Value> argv[] = { array };

  data->callback->Call(Context::GetCurrent()->Global(), 1, argv);
  data->callback.Dispose();
  delete data;
  delete req;
}

// -----------------------------------------------------------------
Handle<Value> serialRead(const Arguments& args) {
  HandleScope scope;
  uv_work_t *req = new uv_work_t;
  AsyncData *data = new AsyncData;
  req->data = data;

  data->callback = Persistent<Function>::New(Local<Function>::Cast(args[0]));
  uv_queue_work(uv_default_loop(), req, Work, (uv_after_work_cb)End);
  return scope.Close(Undefined());
}

// -------------------------------------------------------------------
Handle<Value> serialOpen(const Arguments& args) {
  HandleScope scope;
  String::Utf8Value ss(args[0]->ToString());                 
  char *s = *ss;                                             
  int baud = (int)args[1]->NumberValue();
  serialOpen(s, baud);
  return scope.Close(Undefined());
}

// -------------------------------------------------------------------
Handle<Value> serialClose(const Arguments& args) {
  HandleScope scope;
  serialClose();
  return scope.Close(Undefined());
}

// -------------------------------------------------------------------
Handle<Value> serialPutchar(const Arguments& args) {
  HandleScope scope;
  unsigned char c = (unsigned char)args[1]->NumberValue();
  serialWriteByte(c);
  return scope.Close(Undefined());
}

// http://stackoverflow.com/questions/10507323/shortest-way-one-liner-to-get-a-default-argument-out-of-a-v8-function

#define DEFINE_FUNCTION(n, f) \
  target->Set(String::NewSymbol((n)), FunctionTemplate::New((f))->GetFunction())

// -------------------------------------------------------------------
void Init(Handle<Object> target) {
  DEFINE_FUNCTION("open",      serialOpen);
  DEFINE_FUNCTION("close",     serialClose);
  DEFINE_FUNCTION("putchar",   serialPutchar);
  DEFINE_FUNCTION("read",      serialRead);
}

NODE_MODULE(serial, Init)


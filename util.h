#define getProperty(o, n) (o)->Get(String::New((n)))
#define setProperty(o, n, v)  (o)->Set(String::New((n)), (v))
#define getElement(o, i) (o)->Get((i))
#define ArrayLen(o) (o)->Length()
#define JSVAL_TO_OBJECT(v) (v)->ToObject()
#define JSVAL_TO_INT(v) (int)(v)->NumberValue()

#define JSObject Local<Object>
#define jsval Local<Value>


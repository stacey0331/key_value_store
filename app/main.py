from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from datetime import timedelta
from key_value_store_module import KeyValueStore, TypeMismatchError

app = FastAPI()
store = KeyValueStore()
type_err = TypeMismatchError

class Value(BaseModel):
    value: str

def handle_request(func, *args):
    try:
        res = func(*args)
        return {"result": res}
    except type_err as e:
        raise HTTPException(status_code=400, detail=str(e))
    
@app.post("/setcapacity/")
async def setcapacity(capacity: int):
    return handle_request(store.setcapacity, capacity)

@app.post("/uselru/")
async def uselru():
    return handle_request(store.uselru)

@app.post("/uselfu/")
async def uselfu():
    return handle_request(store.uselfu)

@app.post("/expire/{key}/")
async def expire(key: str, sec: int):
    return handle_request(store.expire, key, timedelta(seconds=sec))

@app.post("/persist/{key}/")
async def persist(key: str):
    return handle_request(store.persist, key)

@app.put("/set/{key}/")
async def set(key: str, request: Value):
    return handle_request(store.set, key, request.value)

@app.get("/get/{key}/")
async def get(key: str):
    return handle_request(store.get, key)

@app.delete("/del/{key}/")
async def delete(key: str):
    return handle_request(store.delete, key)

@app.post("/lpush/{key}/")
async def lpush(key: str, request: Value):
    return handle_request(store.lpush, key, request.value)

@app.post("/rpush/{key}/")
async def rpush(key: str, request: Value):
    return handle_request(store.rpush, key, request.value)

@app.post("/lpop/{key}/")
async def lpop(key: str):
    return handle_request(store.lpop, key)

@app.post("/rpop/{key}/")
async def rpop(key: str):
    return handle_request(store.rpop, key)

@app.get("/lrange/{key}/")
async def lrange(key: str, start: int, end: int):
    if start < 0 or end < 0:
        raise HTTPException(status_code=400, detail="Start and end must be non-negative.")
    if start > end:
        raise HTTPException(status_code=400, detail="Start cannot be greater than end.")
    
    return handle_request(store.lrange, key, start, end)

@app.get("/llen/{key}/")
async def llen(key: str):
    return handle_request(store.llen, key)

@app.post("/sadd/{key}/")
async def sadd(key: str, request: Value):
    return handle_request(store.sadd, key, request.value)

@app.post("/srem/{key}/")
async def srem(key: str, request: Value):
    return handle_request(store.srem, key, request.value)

@app.get("/smembers/{key}/")
async def smembers(key: str):
    return handle_request(store.smembers, key)

@app.get("/sismember/{key}/")
async def sismember(key: str, value: str):
    return handle_request(store.sismember, key, value)

@app.get("/scard/{key}/")
async def scard(key: str):
    return handle_request(store.scard, key)

if __name__ == '__main__': 
    import uvicorn
    uvicorn.run(app, host='0.0.0.0', port=8000)

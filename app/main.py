from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import key_value_store_module

app = FastAPI()
store = key_value_store_module.KeyValueStore()
type_err = key_value_store_module.TypeMismatchError

class Value(BaseModel):
    value: str

@app.put("/set/{key}/")
async def set(key: str, request: Value):
    store.set(key, request.value)
    return {"result": "OK"}

@app.get("/get/{key}/")
async def get(key: str):
    try: 
        res = store.get(key)
        if res is None:
            return {"result": None}
        return {"result": res}
    except type_err as e: 
        raise HTTPException(status_code=400, detail=str(e))

@app.delete("/del/{key}/")
async def delete(key: str):
    res = store.delete(key)
    return res

@app.post("/lpush/{key}/")
async def lpush(key: str, request: Value):
    try: 
        res = store.lpush(key, request.value)
        return {"result": res}
    except type_err as e:
        raise HTTPException(status_code=400, detail=str(e))

@app.post("/rpush/{key}/")
async def rpush(key: str, request: Value):
    try:
        res = store.rpush(key, request.value)
        return {"result": res}
    except type_err as e:
        raise HTTPException(status_code=400, detail=str(e))

@app.post("/lpop/{key}/")
async def lpop(key: str):
    res = store.lpop(key)
    return {"result": res}

@app.post("/rpop/{key}/")
async def rpop(key: str):
    res = store.rpop(key)
    return {"result": res}

@app.get("/lrange/{key}/")
async def lrange(key: str, start: int, end: int):
    if start < 0 or end < 0:
        raise HTTPException(status_code=400, detail="Start and end must be non-negative.")
    if start > end:
        raise HTTPException(status_code=400, detail="Start cannot be greater than end.")
    
    res = store.lrange(key, start, end)
    return {"result": res}

@app.post("/llen/{key}/")
async def llen(key: str):
    try:
        res = store.llen(key)
        return {"result": res}
    except type_err as e:
        raise HTTPException(status_code=400, detail=str(e))

if __name__ == '__main__': 
    import uvicorn
    uvicorn.run(app, host='0.0.0.0', port=8000)

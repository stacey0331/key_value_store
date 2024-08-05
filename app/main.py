from fastapi import FastAPI
import key_value_store_module

app = FastAPI()

store = key_value_store_module.KeyValueStore()

@app.post("/lpush/")
async def lpush(key: str, value: str):
    result = store.lpush(key, value)
    return {"result": result}

@app.post("/lrange/")
async def lrange(key: str, start: int, end: int):
    result = store.lrange(key, start, end)
    return {"result": result}

if __name__ == '__main__': 
    import uvicorn
    uvicorn.run(app, host='0.0.0.0', port=8000)

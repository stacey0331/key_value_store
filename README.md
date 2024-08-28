# Key Value Store


Python virtual environment


## Run
```
arch -arm64 python3 -m pip install pybind11
pip install "fastapi[standard]"
```

build the shared library .so file. 
```
build.sh: 
```

```
fastapi dev app/main.py
```


BELOW IS MY SCRATCH PAPER DURING DEVELOPMENT
CREATE TABLE strings (
    id SERIAL PRIMARY KEY,
    store_id INT NOT NULL,
    key VARCHAR(255) NOT NULL,
    value VARCHAR(2000),
    expiration BIGINT,
    UNIQUE (store_id, key)
);

CREATE TABLE eviction (
    store_id INT PRIMARY KEY,          -- Unique identifier for the store
    policy VARCHAR(10) NOT NULL,       -- Policy can be 'lru' or 'lfu'
    capacity INT DEFAULT 1000,         -- Default capacity is 1000
    cache TEXT[],                      -- Array to store cache keys in order (use TEXT or VARCHAR based on requirements)

    CHECK (policy IN ('lru', 'lfu'))   -- Constraint to ensure policy is either 'lru' or 'lfu'
);

Tables:
- strings
- sets
- lists
- eviction
store_id, policy, capacity, cache


psql -h 127.0.0.1 -p 5432 -U staceylee -d key_value_store

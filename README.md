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

CREATE TABLE strings (
    id SERIAL PRIMARY KEY,
    user_id INT NOT NULL,
    key VARCHAR(255) NOT NULL,
    value VARCHAR(2000),
    UNIQUE (user_id, key)
);
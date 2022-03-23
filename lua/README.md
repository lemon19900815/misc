# lua-module
读取ini配置lua版（lua ini reader module.）



- 配置ini参考

```ini
[ORG]
Name = buerjia
ID = 1001
Address = ChengDu

[Other]
Build = 1.1.0
```

- 使用示例

```lua
local IniReader = require('IniReader')

local ini_reader = IniReader:New()

if ini_reader:Open('file.ini') then
    print(ini_reader:GetString('ORG', 'Name')) -- buerjia

    print(ini_reader:GetInt('ORG', 'ID')) -- 1001

    -- 对于不存在的节点总会返回 nil

    -- 不存在的key
    print(ini_reader:GetInt('ORG', 'IDDDD')) -- nil

    -- 不存在的key
    print(ini_reader:GetString('ORG', 'IDDDD')) -- nil
end
```




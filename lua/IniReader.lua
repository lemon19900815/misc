
-- ini读工具
local IniReader = {
}

function IniReader:New()
    local obj = {}
    setmetatable(obj, {__index = IniReader})

    obj:Ctor()
    return obj
end

function IniReader:Ctor()
    self.cfg_data_ = {}
end

function IniReader:Open(filename)
    local ini = io.open(filename, 'r')
    if not ini then
        print('IniReader:Open->can not open: '..filename)
        return false
    end

    local node = {}
    local node_name = nil
    for line in ini:lines() do
        -- 去除空格
        line = string.gsub(line, ' ', '')
        -- 去除\n
        line = string.gsub(line, '\\n', '')
        -- 去除\r
        line = string.gsub(line, '\\r', '')

        -- 是否组织节点[xxx]（注：string.find中substr以文本形式识别，不按patern模式）
        if string.find(line, '[', 1, true) and string.find(line, ']', 1, true) then
            if node_name then
                self.cfg_data_[node_name] = node
                node = {}
            end

            -- 去除node节点的[]
            node_name = string.sub(line, 2, #line-1)
        else
            local pos = string.find(line, '=')
            if pos then
                local key = string.sub(line, 1, pos-1)
                local value = string.sub(line, pos+1)
                node[key] = value
            end
        end
    end

    if node_name then
        self.cfg_data_[node_name] = node
    end

    -- 关闭文件
    ini:close()

    return true
end

function IniReader:GetInt(section, key)
    return tonumber(self:GetString(section, key))
end

function IniReader:GetString(section, key)
    return (self.cfg_data_[section] or {})[key]
end

return IniReader

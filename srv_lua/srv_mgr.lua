local srv_typedef = require "shared_table.srv_typedef"
local srv_mgr = {}

local name_2_sid = {}


function srv_mgr:add_srv(type,sid)
    name_2_sid[type] = name_2_sid[type] or {}
    table.insert(name_2_sid[type],sid)
end
return srv_mgr



